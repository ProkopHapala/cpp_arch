
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <GL/gl.h>﻿
#include <GL/glut.h>



const float  INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }

#include "math3D_float.h"
#include "trackball.h"
#include "drawUtils.h"

// ===============================
// ===== GLOBAL CONSTAMNTS
// ===============================

const int    WIDTH        = 800;
const int    HEIGHT       = 600;
const int    VIEW_DEPTH   = 1000;
const float  ASPECT_RATIO = WIDTH/(float)HEIGHT ;

const float VIEW_ZOOM_STEP    = 1.2f;
const float VIEW_ZOOM_DEFAULT = 3.0f;


#define COLDEPTH 16

// ===============================
// ===== GLOBAL VARIABLES
// ===============================

SDL_Surface *screen;
SDL_Event event; 

int frameCount=0;

float mouse_spinning = 0;
float mouse_begin_x  = 0;
float mouse_begin_y  = 0;
// mouse rotation
float qCamera[4];
float qCameraOld[4]; 
//float mx,my;        //Actual mouse position in natural coordinate
//float mwx,mwy;      //Actual mouse position in scene coordinate


float zoom = VIEW_ZOOM_DEFAULT;

int boxList;
int objList;
int engine, nose, tank, hull, cockpit, elevator, rudder, wing;


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
	glRotatef(	45,   1, 2, 3);

	glMatrixMode (GL_MODELVIEW);
	float camMatrix[4][4];
	build_rotmatrix (camMatrix, qCamera );
	//printf( "qCamera: %f %f %f %f qCameraOld: %f %f %f %f mouse: %f %f \n", qCamera[0], qCamera[1], qCamera[2], qCamera[3],  qCameraOld[0], qCameraOld[1], qCameraOld[2], qCameraOld[3],     mouse_begin_x, mouse_begin_y  );
	//glTranslatef (shift.x, shift.y, 0);
	//glMultMatrixf (&camMatrix[0][0]);
	glLoadMatrixf(&camMatrix[0][0]);
}

// FUNCTION ======	draw
void draw(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	camera ();

	//glShadeModel(GL_FLAT);
	//glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable    (GL_LIGHTING);
	//glDisable   (GL_LIGHTING);
	float fsz = 5;
	//drawBox( 1,-1, -1,1, -1,1, 1,0.5,0.2 );
	//glCallList( boxList );
	//glCallList( objList );
	glCallList( engine   );
	glCallList( nose     );
	glCallList( tank     );
	glCallList( hull     );
	glCallList( cockpit  );
	glCallList( elevator );
	glCallList( rudder   );
	glCallList( wing     );
	glScalef( -1.0f, 1.0f, 1.0f );
	glCallList( elevator );
	glCallList( wing     );
	drawAxis( 5 );

	//printf( " frameCount: %i \n",frameCount );
	glFinish();
	SDL_GL_SwapBuffers();
}

// FUNCTION ======	mouse_motion
void mouse_camera (int x, int y){
	//mx=(x/(float)WIDTH);
	//my=1-(y/(float)HEIGHT);
	//mwx=(mx-0.5)*2*winx; 
	//mwy=(my-0.5)*2*winy;
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
	float ambient  [] = { 0.1f, 0.15f, 0.25f, 1.0f};
	float diffuse  [] = { 0.9f, 0.8f, 0.7f, 1.0f};
	float specular [] = { 1.0f, 1.0f, 1.0f, 1.0f};
	float shininess[] = { 80.0                  };
	float lightPos [] = { 1.0, 1.0, +1.0, 1.0};
	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
	glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glEnable (GL_COLOR_MATERIAL);
	glLightfv (GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable  (GL_LIGHTING);
	glEnable  (GL_LIGHT0);
	glEnable  (GL_NORMALIZE);
	glEnable  (GL_DEPTH_TEST);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND) ;
	// glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glClearColor( 0.4, 0.4, 0.8, 0.0);                      
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   

	srand(1234);

	trackball (qCamera, 0, 0, 0, 0); // qCameraOld[3] = 1.0;

	boxList = makeBoxList( 1,-1, -1,1, -1,1, 1,0.5,0.2 );

	//objList = loadObjToList( true, 1000, "./geometry/lo_engine_radial.duct.obj" );

	engine   = loadObjToList( true, 1000, "./geometry/hi_engine_radial.duct.obj" );
	nose     = loadObjToList( true, 1000, "./geometry/hi_engine_radial.nose.obj" );
	tank     = loadObjToList( true, 1000, "./geometry/hi_Hi.tank.obj"            );
	hull     = loadObjToList( true, 1000, "./geometry/hi_Hi.hull.obj"            );
	cockpit  = loadObjToList( true, 1000, "./geometry/hi_Hi.cockpit.obj"         );
	rudder   = loadObjToList( true, 1000, "./geometry/hi_Hi.rudder.obj"          );
	elevator = loadObjToList( true, 1000, "./geometry/hi_wings1.elevator.obj"    );
	wing     = loadObjToList( true, 1000, "./geometry/hi_wings1.main.obj"        );

}

// FUNCTION ======  main
int main(){
	setup();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
		draw();  
		inputHanding(); 
		SDL_Delay(10);
	} 
}
