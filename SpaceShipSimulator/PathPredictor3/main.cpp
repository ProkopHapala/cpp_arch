

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


// ===============================
// ===== GLOBAL CONSTAMNTS
// ===============================

const int    WIDTH        = 800; const int  WIDTH_HALF = WIDTH/2;
const int    HEIGHT       = 600; const int  HEIGHT_HALF = HEIGHT/2;
const int    VIEW_DEPTH   = 10000;
const float  ASPECT_RATIO = WIDTH/(float)HEIGHT ;

const float VIEW_ZOOM_STEP    = 1.2f;
const float VIEW_ZOOM_DEFAULT = 200e+9;
float VIEW_ZOOM = VIEW_ZOOM_DEFAULT;


const int   PHYS_STEPS_PER_FRAME = 1;
const float PHYS_TIME_PER_FRAME  = 0.01; 
const float PHYS_dt              = PHYS_TIME_PER_FRAME/((float)PHYS_STEPS_PER_FRAME);


// ===============================
// ===== includes
// ===============================

//#include "include/math3D_double.h"
//#include "include/drawMath3D_double.h"

#include "include/fastmath.h"
#include "include/Vec3.h"
#include "include/Mat3.h"
#include "include/quaternion.h"
#include "include/drawMath.h"

#include "include/ODEintegrator.h"

#include "include/drawUtils.h"
#include "include/testUtils.h"


Vec3d VIEW_CENTER;

#include "include/icosahedron.h"

#include "include/bodies.h"
#include "include/spaceShip.h"

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

int mesh_SphereL1,mesh_SphereL2,mesh_SphereL3;

SpaceBody Sun,Earth;


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

	//glOrtho    ( -VIEW_ZOOM*ASPECT_RATIO, VIEW_ZOOM*ASPECT_RATIO, -VIEW_ZOOM, VIEW_ZOOM, -VIEW_DEPTH, +VIEW_DEPTH );
	glOrtho      ( -ASPECT_RATIO, ASPECT_RATIO, -1, 1, -100, +100 );	
	Mat3d camMat;
	qmouse.toMatrix(camMat);
	float glMat[16];
	toGLMatCam( {0,0,0}, camMat, glMat);
	glMultMatrixf( glMat );

	glMatrixMode (GL_MODELVIEW);

}

double tickSum=0; 
int    stepSum=0;
// FUNCTION ======	physics
void physics(){
	long long nt1 = getCPUticks();

	double dt = PHYS_dt;


	long long nt2  = getCPUticks();
	double perStep = double(nt2-nt1)/PHYS_STEPS_PER_FRAME;
	tickSum+= perStep; stepSum++;
	printf( " PERFORMANCE: %f ticks/step ( in %i steps ) average: %f \n", perStep, PHYS_STEPS_PER_FRAME, tickSum/stepSum );
}


// FUNCTION ======	draw
void draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	camera ();

	Sun.render();
	Earth.render();
	
	glDisable ( GL_LIGHTING );
	glColor3f ( 0.1, 0.1, 0.1 );
	glPushMatrix (        );
	glScale    ( VIEW_ZOOM_DEFAULT/VIEW_ZOOM );
	glBegin(GL_LINE_STRIP);
		glVertex(Sun.pos*(1/VIEW_ZOOM_DEFAULT));
		glVertex(Earth.pos*(1/VIEW_ZOOM_DEFAULT));
	glEnd();
	//printf( " %i %i \n",  Earth.path_list,  Earth.pathAcc_list   );
	if( Earth.path_list    > 0 ){ glColor4f( .1,.2,.3,.1 ); glCallList( Earth.path_list    ); }
	if( Earth.pathAcc_list > 0 ){ glColor4f( .3,.2,.1,.1 ); glCallList( Earth.pathAcc_list ); }
	glPopMatrix();

	glFinish();
	SDL_GL_SwapBuffers();
}


// FUNCTION ======	inputHanding
void inputHanding(){
	while(SDL_PollEvent(&event)){ 
		if( event.type == SDL_KEYDOWN ){ 
			if(event.key.keysym.sym == SDLK_ESCAPE) { quit(); }
			if(event.key.keysym.sym == SDLK_KP_PLUS  ){ VIEW_ZOOM/=VIEW_ZOOM_STEP; printf("zoom: %f \n", VIEW_ZOOM); }
			if(event.key.keysym.sym == SDLK_KP_MINUS ){ VIEW_ZOOM*=VIEW_ZOOM_STEP; printf("zoom: %f \n", VIEW_ZOOM); }
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

	//if      ( keystate[ SDLK_a  ] ){ myCraft.wingLeft.C.y = wingLiftUp;      myCraft.wingRight.C.y = wingLiftDown;    }
	//else if ( keystate[ SDLK_d  ] ){ myCraft.wingLeft.C.y = wingLiftDown;    myCraft.wingRight.C.y = wingLiftUp;      }
	//else                           { myCraft.wingLeft.C.y = wingLiftDefault; myCraft.wingRight.C.y = wingLiftDefault; };



/*
	if ( keystate[ SDLK_DOWN  ] ) { qmouse.pitch2( -0.005 ); }
	if ( keystate[ SDLK_UP    ] ) { qmouse.pitch2( 0.005 ); }
	if ( keystate[ SDLK_RIGHT ] ) { qmouse.yaw2  ( 0.005 ); }
	if ( keystate[ SDLK_LEFT  ] ) { qmouse.yaw2  ( -0.005 ); }
*/

}


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
	float light0_pos  []   = { -1000.0f, -2000.0f, -3000.0f, 1.00f}; glLightfv (GL_LIGHT0, GL_POSITION, light0_pos );
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

	glClearColor( 0.0, 0.0, 0.0, 0.0);                      
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   

	qmouse.setOne();

	srand(1234);

	mesh_SphereL1 = glGenLists(1); glNewList( mesh_SphereL1, GL_COMPILE ); drawIcosaSphere( 1 ); glEndList();
	mesh_SphereL2 = glGenLists(1); glNewList( mesh_SphereL2, GL_COMPILE ); drawIcosaSphere( 2 ); glEndList();
	mesh_SphereL3 = glGenLists(1); glNewList( mesh_SphereL3, GL_COMPILE ); drawIcosaSphere( 3 ); glEndList();

	Sun.radius = 696.342e+6;
	Sun.mass   = 1.98855e+30;
	Sun.pos.set(0.0);
	Sun.vel.set(0.0);
	Sun.color.set( 2.0,1.8,1.5 );
	Sun.LOD[0] = mesh_SphereL1; Sun.LOD[1] = mesh_SphereL2; Sun.LOD[2] = mesh_SphereL3;

	Sun.init();

	Earth.radius = 100*6371.0e+3;
	//Earth.radius = 696.342e+6;
	Earth.mass   = 5.97219e+24;
	Earth.pos.set( 149.598261000e+9, 0, 0 );
	Earth.vel.set( 0, 0.5* 29.78e+3, 0);
    Earth.color.set( .2,.4,.7 );
	Earth.LOD[0] = mesh_SphereL1; Earth.LOD[1] = mesh_SphereL2; Earth.LOD[2] = mesh_SphereL3;
	Earth.nCenters = 1;
	Earth.centers[0] = &Sun;

	Earth.init();
	//Earth.path = Earth.pathToList( 31556926.0/(100*100),  100,100 ); 
	Earth.makePath( 0.0, 31556926.0/(100*100),  100, 100 );
	Earth.pathToList   ( ); 
	Earth.pathToListAcc( 1e+11 ); 



	VIEW_CENTER.set(0.0);

}

// FUNCTION ======  main
int main(){

	printf("aspect ratio %f \n", ASPECT_RATIO );

	//printf( " %f ", PHYS_dt );
	setup();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
	//for( frameCount=0; frameCount<5; frameCount++ ){
		if (!STOP){ draw();  } 
		inputHanding(); 
		SDL_Delay(  int(PHYS_TIME_PER_FRAME*1000) );
	} 

}
