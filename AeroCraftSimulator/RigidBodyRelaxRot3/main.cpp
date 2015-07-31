

#define SPEED_TEST

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glut.h>

const float  INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }

//#include "math3D_double.h"
#include "Vec3.h"
#include "Mat3.h"
#include "quaternion.h"


#include "trackball.h"
#include "drawMath3D_double.h"
#include "drawUtils.h"
#include "testUtils.h"

#include "rigidBody.h"

// ===============================
// ===== GLOBAL CONSTAMNTS
// ===============================

const int    WIDTH        = 800;
const int    HEIGHT       = 600;
const int    VIEW_DEPTH   = 1000;
const float  ASPECT_RATIO = WIDTH/(float)HEIGHT ;

const float VIEW_ZOOM_STEP    = 1.2f;
const float VIEW_ZOOM_DEFAULT = 30.0f;

const int   PHYS_STEPS_PER_FRAME = 10;
const float PHYS_TIME_PER_FRAME  = 0.03; 
const float PHYS_dt              = PHYS_TIME_PER_FRAME/((float)PHYS_STEPS_PER_FRAME);

#define COLDEPTH 16





// ===============================
// ===== GLOBAL VARIABLES
// ===============================

SDL_Surface *screen;
SDL_Event event; 

int frameCount=0;

// mouse rotation
float mouse_spinning = 0;  float mouse_begin_x  = 0; float mouse_begin_y  = 0;
float qCamera[4]; float qCameraOld[4]; 

float zoom = VIEW_ZOOM_DEFAULT;


const int len = 8; 
static double masses[len] = { 1.0,1.0, 1.0,1.0, 1.0,1.0, 1.0,1.0 };
static double poss_[3*len] = { -1.0,-2.0,-3.0,    -1.0,-2.0,+3.0,   -1.0,+2.0,-3.0,    -1.0,+2.0,+3.0,    +1.0,-2.0,-3.0,   +1.0,-2.0,+3.0,    +1.0,+2.0,-3.0,   +1.0,+2.0,+3.0 };
Vec3d * poss  = (Vec3d*) &poss_[0];

PointBody body;
RigidBody rbody, rbody2;

SpringConstrain* constrain1;

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
	glOrtho      ( -zoom*ASPECT_RATIO, zoom*ASPECT_RATIO, -zoom, zoom, -VIEW_DEPTH, +VIEW_DEPTH );
	//glRotatef(	45,   1, 2, 3);

	glMatrixMode (GL_MODELVIEW);
	float camMatrix[4][4];
	build_rotmatrix (camMatrix, qCamera );
	//printf( "qCamera: %f %f %f %f qCameraOld: %f %f %f %f mouse: %f %f \n", qCamera[0], qCamera[1], qCamera[2], qCamera[3],  qCameraOld[0], qCameraOld[1], qCameraOld[2], qCameraOld[3],     mouse_begin_x, mouse_begin_y  );
	//glTranslatef (shift.x, shift.y, 0);
	//glMultMatrixf (&camMatrix[0][0]);
	glLoadMatrixf(&camMatrix[0][0]);
}

double tickSum=0; 
int    stepSum=0;
// FUNCTION ======	physics
void physics(){
	double dt = PHYS_dt;
	long long nt1 = getCPUticks();
	for(int iSubStep=0; iSubStep<PHYS_STEPS_PER_FRAME; iSubStep++ ){

		double Cd = -0.5;
		double g  = -9.81;
		double k  =  20;
		
		rbody.clean_temp  (                               );
		rbody.force.set   ( 0,g*rbody.mass,0              );

/*
		for ( int i=0; i<len; i++ ){
			rbody.apply_anchor( k, poss[i], poss[i] );
		}
		rbody.vel.mul(0.8);
		rbody.L  .mul(0.8);
*/

		Vec3d p0,p1; p1.set(0.0,0.0,0.0); p1.set(1.0,2.0,3.0);

		rbody.apply_force (    rbody.vel*Cd , p0     );
		rbody.apply_anchor( k, p1,       p0     );
		rbody2.clean_temp  (                         );
		rbody2.force.set   ( 0,g*rbody2.mass,0       );
		rbody2.apply_force (    rbody2.vel*Cd, p0    );
		constrain1->apply();

/*
		rbody.apply_force (    rbody.vel*Cd , {0,0,0}     );
		rbody.apply_anchor( k, {1,2,3},       {0,0,0}     );
		rbody2.clean_temp  (                              );
		rbody2.force.set   ( 0,g*rbody2.mass,0            );
		rbody2.apply_force (    rbody2.vel*Cd, {0,0,0}    );
		constrain1->apply();
*/

	
		//printVec(rbody.force  );   printf("force\n");
		//printVec(rbody.torq  );    printf("torq\n");

		rbody .move(dt);
		rbody2.move(dt);

	} 
	long long nt2  = getCPUticks();
	double perStep = double(nt2-nt1)/PHYS_STEPS_PER_FRAME;
	tickSum+= perStep; stepSum++;
	printf( " PERFORMANCE: %f ticks/step ( in %i steps ) average: %f \n", perStep, PHYS_STEPS_PER_FRAME, tickSum/stepSum );
}


// FUNCTION ======	draw
void draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

	Vec3d oldPos; oldPos.set( body.pos );

	//printVec( oldPos ); printf(" // oldPos \n");

/*
	printf("invIbody\n");
	printMat(rbody.invIbody);
	printf("invI\n");
	printMat(rbody.invI);
	printf("rotMat\n");
	printMat(rbody.rotMat);
	printQuat(rbody.qrot  ); printf("qrot\n");
	printVec(rbody.L    );   printf("L\n");
	printVec(rbody.omega);   printf("omega\n");
	printVec(rbody.pos  );   printf("pos\n");
*/

	physics();  // ALL PHYSICS COMPUTATION DONE HERE 

	
	camera ();

	//drawLine( oldPos, body.pos );
	glEnable (GL_LIGHTING);
	glShadeModel(GL_FLAT);
	rbody.render();

	rbody2.render();
	glColor3f( 0, 0, 0);  
	constrain1->render();

/*
	printf("invI\n");
	printMat(rbody.invI);
	printf("rotMat\n");
	printMat(rbody.rotMat);
	printQuat(rbody.qrot  ); printf("qrot\n");
	printVec(rbody.L    );   printf("L\n");
	printVec(rbody.omega);   printf("omega\n");
	printVec(rbody.pos  );   printf("pos\n");
	quit(); 	
*/

	drawAxis( 5 );


	glFinish();
	SDL_GL_SwapBuffers();
}

// FUNCTION ======	mouse_motion
void mouse_camera (int x, int y){
	if (mouse_spinning){
		//printf( "SPINNING \n");
		trackball ( 
			qCameraOld,
			(2.0f*mouse_begin_x-WIDTH)/WIDTH,  (HEIGHT-2.0f*mouse_begin_y)/HEIGHT, 
			(2.0f*x-WIDTH            )/WIDTH,  (HEIGHT-2.0f*y            )/HEIGHT 
		);
		add_quats ( qCameraOld, qCamera, qCamera );
		mouse_begin_x = x; mouse_begin_y = y;
		//glutPostRedisplay ();   
	}
}

// FUNCTION ======	inputHanding
void inputHanding(){
	while(SDL_PollEvent(&event)){ 
		if( event.type == SDL_KEYDOWN ){ 
			if(event.key.keysym.sym == SDLK_ESCAPE) { quit(); }
			if(event.key.keysym.sym == SDLK_KP_PLUS  ){ zoom/=VIEW_ZOOM_STEP; printf("zoom: %f \n", zoom); }
			if(event.key.keysym.sym == SDLK_KP_MINUS ){ zoom*=VIEW_ZOOM_STEP; printf("zoom: %f \n", zoom); }
		} 
		if( event.type == SDL_MOUSEMOTION ){
			mouse_camera( event.motion.x, event.motion.y );
		}
		if( event.type == SDL_MOUSEBUTTONDOWN )    {
		    if( event.button.button == SDL_BUTTON_LEFT ) {
				mouse_spinning = 1;
		        mouse_begin_x  = event.button.x;
		        mouse_begin_y  = event.button.y;
				//printf("SDL_MOUSEBUTTONDOWN\n");
		    }
		} 
		if( event.type == SDL_MOUSEBUTTONUP ){
		    if( event.button.button == SDL_BUTTON_LEFT ){ mouse_spinning = 0; }
		}
		if( event.type == SDL_QUIT){ quit();  };
	}
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
	float ambient  [] = { 0.10f, 0.15f, 0.25f, 1.0f}; glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
	float diffuse  [] = { 0.50f, 0.50f, 0.50f, 1.0f}; glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
	float specular [] = { 0.00f, 0.00f, 0.00f, 1.0f}; glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
	float shininess[] = { 0.0f                     }; glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glEnable  (GL_LIGHT0);
	float light0_pos  [] = { 1000.0f, 0.0f, 0.0f, 1.00f}; glLightfv (GL_LIGHT0, GL_POSITION, light0_pos  );
	float light0_color[] = { 0.8f, 0.5f, 0.5f, 1.00f}; glLightfv (GL_LIGHT0, GL_DIFFUSE,  light0_color);
	glEnable  (GL_LIGHT1);
	float light1_pos  [] = { 0.0f, 1000.0f, 0.0f, 1.00f}; glLightfv (GL_LIGHT1, GL_POSITION, light1_pos  );
	float light1_color[] = { 0.5f, 0.8f, 0.5f, 1.00f}; glLightfv (GL_LIGHT1, GL_DIFFUSE,  light1_color);
	glEnable  (GL_LIGHT2);
	float light2_pos  [] = { 0.0f, 0.0f, 1000.0f, 1.00f}; glLightfv (GL_LIGHT2, GL_POSITION, light2_pos  );
	float light2_color[] = { 0.5f, 0.5f, 0.8f, 1.00f}; glLightfv (GL_LIGHT2, GL_DIFFUSE,  light2_color);
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

	srand(1234);

	trackball (qCamera, 0, 0, 0, 0); // qCameraOld[3] = 1.0;


	printf( "1\n" );

	body.pos.set( 1,1,1 ); 
	body.vel.set( 1,-1,1 );

	//const int len = 6; 
	//double masses[len]  = { 1,1, 1,1, 1,1 };
	//Vec3d  poss[len]    = { {-1,0,0}, {+1,0,0}, {0,-2,0}, {0,+2,0}, {0,0,-3}, {0,0,+3} }
	//printf( " %f %f \n", poss[0].x, poss_[0]  );

	printf( "2\n" );

	int boxList = makeBoxList( -1,1, -2,2, -3, 3,    0.9, 0.9, 0.9  );
	//rbody.shape = boxList;
	rbody.from_mass_points( len, masses, poss );
	//rbody.qrot.setOne();

	printf( "3\n" );

	rbody.qrot.set(4,-3,2,-1);	rbody.qrot.normalize();
	//rbody.pos.add( 5.0d,5.0d,5.0d );

	printf( "4\n" );

	rbody.vel.set(0.0);
	//rbody.L  .set(10,10,10);
	rbody.init( );
	rbody.shape = shapePointsCenter( len, poss, rbody.pos, 0.5 );

	printf( "5\n" );


	rbody2.shape = boxList;
	rbody2.from_mass_points( len, masses, poss );
	rbody2.qrot.setOne();
	rbody2.vel.set(0.0);
	//rbody2.L  .set(10,10,10);
	rbody2.init( );
	constrain1 = new SpringConstrain( 20, &rbody, &rbody2, {-1,-2,-3}, {1,2,3} );
	//Vec3d p1,p2; p1.set(-1,-2,-3); p2.set(1,2,3);
	//constrain1 = new SpringConstrain( 20, &rbody, &rbody2, p1, p2 );


	printf("mass: %f invMass %f \n", rbody.mass, rbody.invMass  );
	printf("Ibody\n");
	printMat(rbody.Ibody);
	printf("invIbody\n");
	printMat(rbody.invIbody);
}

// FUNCTION ======  main
int main(){


/*	
	Quat4d q, dq; 
	Vec3d  axis;  axis.set( 1,2,3 ); axis.normalize(); 
	Mat3d  mat;
	
	double alfa = M_PI*0.125d;

	// 0.000000 0.000000 0.382683 0.923880  // dq
	// 0.000000 0.000000 0.191342 0.923880  // dq

	printf("\n");
	printf("Rotation method: q*dq \n");
	q .setOne(); 
	dq.fromAngleAxis( alfa, axis );
	printQuat( dq ); printf( " // dq \n" );
	q .qmul(dq);
	q .toMatrix( mat );
	printQuat( q ); printf( " // q \n" );
	printf( " mat:\n" );
	printMat( mat );

	printf("\n");
	printf("Rotation method: dRot_exact \n");
	q.setOne();
	q.dRot_exact( alfa, axis );
	q.toMatrix( mat );
	printQuat( q ); printf( " // q \n" );
	printf( " mat:\n" );
	printMat( mat );

	printf("\n");
	printf("Rotation method: dRot_taylor2 \n");
	q.setOne();
	q.dRot_taylor2( alfa, axis );
	q.toMatrix( mat );
	printQuat( q ); printf( " // q \n" );
	printf( " mat:\n" );
	printMat( mat );
	q.fromMatrix( mat );
	printQuat( q ); printf( " // q \n" );
*/

	//printf( " %f ", PHYS_dt );
	setup();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
		draw();  
		inputHanding(); 
		SDL_Delay(  int(PHYS_TIME_PER_FRAME*1000) );
	} 


}
