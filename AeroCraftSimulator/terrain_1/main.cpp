

#define SPEED_TEST

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glut.h>

const  float  INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf() { return INV_RAND_MAX*rand(); }
inline float randf( float fmin, float fmax ){ return (INV_RAND_MAX*rand())*(fmax-fmin) + fmin; }

#include "math3D_double.h"
#include "drawMath3D_double.h"
#include "drawUtils.h"
#include "testUtils.h"

#include "rigidBody.h"
#include "AeroCraft.h"
#include "FieldPatch.h"

// ===============================
// ===== GLOBAL CONSTAMNTS
// ===============================

const int    WIDTH        = 800; const int  WIDTH_HALF = WIDTH/2;
const int    HEIGHT       = 600; const int  HEIGHT_HALF = HEIGHT/2;
const int    VIEW_DEPTH   = 10000;
const float  ASPECT_RATIO = WIDTH/(float)HEIGHT ;

const float VIEW_ZOOM_STEP    = 1.2f;
const float VIEW_ZOOM_DEFAULT = 0.5f;

const int   PHYS_STEPS_PER_FRAME = 1;
const float PHYS_TIME_PER_FRAME  = 0.01; 
const float PHYS_dt              = PHYS_TIME_PER_FRAME/((float)PHYS_STEPS_PER_FRAME);

bool STOP = false; 
#define COLDEPTH 16

// ===============================
// ===== GLOBAL VARIABLES
// ===============================

SDL_Surface *screen;
SDL_Event event; 

int frameCount=0;

// mouse rotation
Quat4d qmouse;

float zoom = VIEW_ZOOM_DEFAULT;

AeroCraft myCraft; 

int buildings=-1;
int terrain=-1;

// Aerocraft handling
double wingLiftDefault = 1.0;
double wingLiftDown    = 0.0;
double wingLiftUp      = 2.0;

// ====================================
// ===== FUNCTION FORWARD DECLARATIONS
// ====================================

void quit(){SDL_Quit(); exit(1);};
void setup();
void camera();
void draw();
void inputHanding();

// ===============================
// ===== FUNCTION IMPLEMENTATION
// ===============================


// FUNCTION ======	camera 
void camera (){
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

/*
	float zoomo = zoom*10;
	glOrtho      ( -zoomo*ASPECT_RATIO, zoomo*ASPECT_RATIO, -zoomo, zoomo, -VIEW_DEPTH, +VIEW_DEPTH );	
	Mat3d camMat;
	qmouse.toMatrix(camMat);
	float glMat[16];
	toGLMatCam( {0,0,0}, camMat, glMat);
	glMultMatrixf( glMat );
*/

	glFrustum ( -zoom*ASPECT_RATIO, zoom*ASPECT_RATIO, -zoom, zoom, 0.5, VIEW_DEPTH);
	Quat4d qcam;
	qcam.setQmul( qmouse, myCraft.qrot );
	//qcam.set(qmouse);
	//qcam.set( myCraft.qrot );
	Mat3d camMat;
	qcam.toMatrix(camMat);
	float glMat[16];
	toGLMatCam( {0,0,0}, camMat, glMat);
	glMultMatrixf( glMat );


	glTranslatef( (float)-myCraft.pos.x, (float)-myCraft.pos.y, (float)-myCraft.pos.z );

	glMatrixMode (GL_MODELVIEW);

}

double tickSum=0; 
int    stepSum=0;
// FUNCTION ======	physics
void physics(){
	long long nt1 = getCPUticks();

	double dt = PHYS_dt;
	double g  = -9.81;
	myCraft.checkStateNormal();
	for(int iSubStep=0; iSubStep<PHYS_STEPS_PER_FRAME; iSubStep++ ){
		myCraft.clean_temp();
		myCraft.force.set( {0,g*myCraft.mass,0}  );
		myCraft.force += myCraft.rotMat.c*50.0;
		myCraft.applyAeroForces( {0,0,0} );
		//myCraft.applyAeroForces( {0,10,0} );
		myCraft.move(dt);
	} 

	long long nt2  = getCPUticks();
	double perStep = double(nt2-nt1)/PHYS_STEPS_PER_FRAME;
	tickSum+= perStep; stepSum++;
	printf( " PERFORMANCE: %f ticks/step ( in %i steps ) average: %f \n", perStep, PHYS_STEPS_PER_FRAME, tickSum/stepSum );
}


// FUNCTION ======	draw
void draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

	glDepthMask(0);
	glDisable (GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	float skysz = VIEW_DEPTH/2;
	glBegin(GL_QUADS);
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( -skysz, skysz, -skysz );   	  
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( -skysz, skysz, +skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( +skysz, skysz, +skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( +skysz, skysz, -skysz );  

		glColor3f( 0.3, 0.5, 0.5 );  glVertex3f( -skysz,     0, -skysz );   	  
		glColor3f( 0.3, 0.5, 0.5 );  glVertex3f( -skysz,     0, +skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( -skysz, skysz, +skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( -skysz, skysz, -skysz ); 
	
		glColor3f( 0.3, 0.5, 0.5 );  glVertex3f( +skysz,     0, -skysz );   	  
		glColor3f( 0.3, 0.5, 0.5 );  glVertex3f( +skysz,     0, +skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( +skysz, skysz, +skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( +skysz, skysz, -skysz ); 	
  
		glColor3f( 0.3, 0.5, 0.5 );  glVertex3f( -skysz,     0, -skysz );   	  
		glColor3f( 0.3, 0.5, 0.5 );  glVertex3f( +skysz,     0, -skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( +skysz, skysz, -skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( -skysz, skysz, -skysz ); 
	
		glColor3f( 0.3, 0.5, 0.5 );  glVertex3f( -skysz,     0, +skysz );   	  
		glColor3f( 0.3, 0.5, 0.5 );  glVertex3f( +skysz,     0, +skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( +skysz, skysz, +skysz );   	 
		glColor3f( 0.1, 0.1, 0.5 );  glVertex3f( -skysz, skysz, +skysz ); 
  
	glEnd();
	glDepthMask(1);
	physics();  // ALL PHYSICS COMPUTATION DONE HERE 

	/*
	printQuat( myCraft.qrot );  printf("qrot\n");
	printVec( myCraft.force );  printf("force\n");
	printVec( myCraft.torq );   printf("torq\n");
	printVec( myCraft.L );      printf("L\n");
	printVec( myCraft.omega );  printf("omega\n");
	printf("invI\n");
	printMat(  myCraft.invI ); 
	printf("rotMat\n");
	printMat(  myCraft.rotMat  ); 
	*/

	camera ();

	glEnable    (GL_LIGHTING);
	glShadeModel(GL_FLAT);

	myCraft.render();
	
	//glDisable (GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	if (buildings>0) glCallList( buildings ); 
	if (terrain>0)  { 
		glCallList( terrain   );
	} else {
	 	// terrain
		float groundsz = VIEW_DEPTH;
		glBegin(GL_QUADS);
			glColor3f( 0.3, 0.6, 0.1 );		          	     
			glNormal3f(0,1,0); 
			glVertex3f( -groundsz, 0, -groundsz ); 
			glVertex3f( +groundsz, 0, -groundsz ); 
			glVertex3f( +groundsz, 0, +groundsz ); 
			glVertex3f( -groundsz, 0, +groundsz ); 
		glEnd();
	};

	drawAxis( 5 );

	glFinish();
	SDL_GL_SwapBuffers();
}


// FUNCTION ======	inputHanding
void inputHanding(){
	while(SDL_PollEvent(&event)){ 
		if( event.type == SDL_KEYDOWN ){ 
			if(event.key.keysym.sym == SDLK_ESCAPE) { quit(); }
			if(event.key.keysym.sym == SDLK_KP_PLUS  ){ zoom/=VIEW_ZOOM_STEP; printf("zoom: %f \n", zoom); }
			if(event.key.keysym.sym == SDLK_KP_MINUS ){ zoom*=VIEW_ZOOM_STEP; printf("zoom: %f \n", zoom); }
			if(event.key.keysym.sym == SDLK_SPACE    ){ STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); }
		} 
		if( event.type == SDL_QUIT){ quit();  };
	}
	// mouse Camera

	int mx,my;
	SDL_GetMouseState(&mx,&my);
	int dmx = mx - WIDTH_HALF; 	int dmy = my - HEIGHT_HALF;
	//printf( " mx: %i  my: %i dmx: %i dmy: %i ",mx, my, dmx, dmy );
	qmouse.pitch( 0.001* dmy );
	qmouse.yaw  ( 0.001* dmx );
	SDL_WarpMouse(WIDTH_HALF,HEIGHT_HALF);


// see http://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html
// http://sdl.beuc.net/sdl.wiki/SDL_GetKeyState

	Uint8 *keystate = SDL_GetKeyState(NULL);

	if ( keystate[ SDLK_DOWN  ] ) { qmouse.pitch( -0.005 );  }
	if ( keystate[ SDLK_UP    ] ) { qmouse.pitch(  0.005  );  }
	if ( keystate[ SDLK_RIGHT ] ) { qmouse.yaw  (  0.005  );  }
	if ( keystate[ SDLK_LEFT  ] ) { qmouse.yaw  ( -0.005 ); }

	if      ( keystate[ SDLK_a  ] ){ myCraft.wingLeft.C.y = wingLiftUp;      myCraft.wingRight.C.y = wingLiftDown;    }
	else if ( keystate[ SDLK_d  ] ){ myCraft.wingLeft.C.y = wingLiftDown;    myCraft.wingRight.C.y = wingLiftUp;      }
	else                           { myCraft.wingLeft.C.y = wingLiftDefault; myCraft.wingRight.C.y = wingLiftDefault; };



/*
	if ( keystate[ SDLK_DOWN  ] ) { qmouse.pitch2( -0.005 ); }
	if ( keystate[ SDLK_UP    ] ) { qmouse.pitch2( 0.005 ); }
	if ( keystate[ SDLK_RIGHT ] ) { qmouse.yaw2  ( 0.005 ); }
	if ( keystate[ SDLK_LEFT  ] ) { qmouse.yaw2  ( -0.005 ); }
*/

}



int makeBuildingsGrid( int nx, int ny, float sx, float sy, float cx, float cy,  float min_height, float max_height ){
	int ilist=glGenLists(1);
	glNewList( ilist, GL_COMPILE );
	for (int ix=-nx; ix<nx; ix++){
		float x = ix*sx;
		for (int iy=-ny; iy<ny; iy++){
			float height = randf() * (max_height-min_height) + min_height;
			float y = iy*sy;
			drawBox( x, x + sx*cx, 0, height, y, y + sy*cy, 0.75f, 0.75f, 0.75f );
		}
	}
	glEndList();
	return( ilist );
}

int makeBuildingsClusters( int nclustest, int nmin, int nmax, float minx, float maxx, float miny, float maxy, float min_dist, float max_dist, float min_size, float max_size, float min_height, float max_height ){
	int ilist=glGenLists(1);
	glNewList( ilist, GL_COMPILE );
	int nboxes = 0;
	for (int icluster=0; icluster<nclustest; icluster++){
		float x0 = randf()*(maxx-minx) + minx;
		float y0 = randf()*(maxy-miny) + miny;
		float nb = round(randf()*(nmax - nmin)) + nmin;
		for (int ib=0; ib<nb; ib++){
			float height = randf() * (max_height-min_height) + min_height;
			float x  = x0 + randf()*(max_dist-min_dist) + min_dist;
			float y  = y0 + randf()*(max_dist-min_dist) + min_dist;
			float dx = randf()*(max_size-min_size) + min_size;
			float dy = randf()*(max_size-min_size) + min_size;
			drawBox( x-dx, x+dx, 0, height, y-dy, y+dy, 0.75f, 0.75f, 0.75f );
			nboxes++;
		};
	};
	printf(" %i buildings \n", nboxes );
	glEndList();
	return( ilist );
}

/*
int makeSinTerrain( float nx, float ny, float szx, float szy, float height ){
	for (int ix=0; ix<nx; ix++){
		for (int iy=0; iy<nx; iy++){
			val = css[ix][iy];
			glEndList();
	}}
}
*/

// FUNCTION ======	setup
void setup(){
	//---- SDL initialization
	if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {        printf("Unable to initialize SDL: %s\n", SDL_GetError()); getchar() ;    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	//screen = SDL_SetVideoMode( WIDTH, HEIGHT, COLDEPTH, SDL_OPENGL |  SDL_SWSURFACE  ); 
	screen = SDL_SetVideoMode( WIDTH, HEIGHT, COLDEPTH, SDL_OPENGL ); 
    if(!screen)  { printf("Couldn't set %dx%d GL video mode: %s\n", WIDTH,HEIGHT, SDL_GetError()); SDL_Quit();  exit(2); }
	SDL_WM_SetCaption(" Prokop's test SDL+ OpenGL", "SDL+ OpenGL");

	//materialy
	float ambient  [] = { 0.25f, 0.25f, 0.25f, 1.0f}; glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
	float diffuse  [] = { 0.50f, 0.50f, 0.50f, 1.0f}; glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
	float specular [] = { 0.00f, 0.00f, 0.00f, 1.0f}; glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
	float shininess[] = { 0.0f                     }; glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glEnable  (GL_LIGHT0);
	float light0_pos  []   = { 1000.0f, 2000.0f, 3000.0f, 1.00f}; glLightfv (GL_LIGHT0, GL_POSITION, light0_pos );
	float light0_color[]   = { 0.9f, 0.8f, 0.7f, 1.00f}; glLightfv (GL_LIGHT0, GL_DIFFUSE,  light0_color  );
	float light0_ambient[] = { 0.5f, 0.6f, 0.7f, 1.00f}; glLightfv (GL_LIGHT0, GL_AMBIENT,  ambient       );
	glEnable  (GL_LIGHTING);
	glEnable (GL_COLOR_MATERIAL);
	glEnable  (GL_NORMALIZE);
	glEnable  (GL_DEPTH_TEST);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND) ;
	//glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glClearColor( 0.9, 0.9, 0.9, 0.0);                      
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   

	qmouse.setOne();

	srand(1234);

	//buildings = makeBuildings( 10, 10, 100, 100, 0.5, 0.5, 50, 100 );

	buildings = makeBuildingsClusters( 30, 3, 10, -VIEW_DEPTH/2, VIEW_DEPTH/2, -VIEW_DEPTH/2, VIEW_DEPTH/2,    0, 500,   20, 100,   10, 100 );

	//buildings = makeBuildingsClusters( 100, 5, 5, -VIEW_DEPTH/2, VIEW_DEPTH/2, -VIEW_DEPTH/2, VIEW_DEPTH/2,    0, 500,   100, 100,   10, 100 );

	float h0    = 1;
	//float tersz = VIEW_DEPTH/2;
	float tersz = 5000;
	terrain   = FieldPatch::makeList( 15, { 0.5,   -tersz,-tersz,h0,   tersz,-tersz,h0,  -tersz,tersz,h0,   tersz,tersz,h0   }   );

	const int len = 5; 
	double masses[len]  = { 4,1,1,0.5f,0.5f };
	Vec3d  poss[len]    = { {0,-0.05,1}, {-2,0,0}, {2,0,0}, {0,0,-3}, {0,0.2,-3} };

	myCraft.from_mass_points( len, masses, poss );
	myCraft.qrot.setOne(); 
	//myCraft.qrot.set(0,0,-0.5,1); myCraft.qrot.normalize();

	printVec(myCraft.pos); printf("pos\n");

	myCraft.wingLeft .craft = &myCraft;
	myCraft.wingRight.craft = &myCraft;
	myCraft.elevator .craft = &myCraft;
	myCraft.rudder   .craft = &myCraft;

	myCraft.wingLeft .lpos.set( poss[1] - myCraft.pos ); 
	myCraft.wingRight.lpos.set( poss[2] - myCraft.pos );
	myCraft.elevator .lpos.set( poss[3] - myCraft.pos ); 
	myCraft.rudder   .lpos.set( poss[4] - myCraft.pos ); 

/*
	myCraft.wingLeft .lpos.set( poss[1] ); 
	myCraft.wingRight.lpos.set( poss[2] );
	myCraft.elevator .lpos.set( poss[3] ); 
	myCraft.rudder   .lpos.set( poss[4] ); 
*/

	myCraft.wingLeft .lrot.set( { 1,0,0, 0,1,0,  0,0,1  } ); myCraft.wingLeft  .lrot.rotate( -0.1, { 0,0,1 } );
	myCraft.wingRight.lrot.set( { 1,0,0, 0,1,0,  0,0,1  } ); myCraft.wingRight .lrot.rotate( +0.1, { 0,0,1 } );
	myCraft.elevator .lrot.set( { 1,0,0, 0,1,0,  0,0,1  } ); myCraft.elevator .lrot.rotate( +0.2, { 1,0,0 } );
	printf("elevator lrot\n");
	printMat( myCraft.elevator.lrot );
 
	myCraft.rudder   .lrot.set( { 0,1,0, 1,0,0,  0,0,1  } );

	myCraft.wingLeft .C.set( 0.05, wingLiftDefault, 0.05 ); 
	myCraft.wingRight.C.set( 0.05, wingLiftDefault, 0.05 ); 
	myCraft.elevator .C.set( 0.05, 1.0, 0.05 ); myCraft.elevator.C *= 0.1;
	myCraft.rudder   .C.set( 0.05, 1.0, 0.05 ); myCraft.rudder  .C *= 0.1;

	myCraft.L.set(0,0,0);
	myCraft.init( );

	myCraft.vel.set(0,0,0);
	myCraft.pos.set(0,500,0);

	printf("Ibody\n");
	printMat(myCraft.Ibody);
	printf("invIbody\n");
	printMat(myCraft.invIbody);

}

// FUNCTION ======  main
int main(){

	//printf( " %f ", PHYS_dt );
	setup();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
	//for( frameCount=0; frameCount<5; frameCount++ ){
		if (!STOP){ draw();  } 
		inputHanding(); 
		SDL_Delay(  int(PHYS_TIME_PER_FRAME*1000) );
	} 

}
