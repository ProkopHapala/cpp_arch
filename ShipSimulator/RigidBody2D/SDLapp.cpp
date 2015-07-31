
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "include/fastmath.h"

#include "include/Vec2.h"
#include "include/Vec3.h"
#include "include/Mat3.h"

#include "include/raytrace.h"

#include "include/drawMath.h"
#include "include/drawMath2D.h"

#include "include/drawUtils.h"

#include "include/Body2D.h"

// ===============================
// ===== GLOBAL CONSTAMNTS
// ===============================

const float	VIEW_ZOOM_STEP     = 1.2f;
const float	VIEW_ZOOM_DEFAULT  = 10.0f;
const float	VIEW_DEPTH_DEFAULT = 1000.0;



// ===============================
// ===== GLOBAL VARIABLES
// ===============================

bool  isLib             = false;
bool  STOP          	= false; 
bool  loopEnd           = false;
int   frameCount		=	0;

SDL_Event		 event; 


double dt = 0.003;

RigidBody2D b1,b2,b3;

SpringConstrain2D *spring1,*spring2;


const int npts = 2;
static double poss[npts*2] = { -1.0, -1.0, 1.0, -1.0 };
static double mass[npts  ] = { 1.0, 1.0 };


#include "include/Screen2D.h"
Screen2D* thisScreen;



// ===============================
// ===== FUNCTIONS 
// ===============================

void quit(){
	SDL_Quit(); 
	exit(1);
};

void escape(){
	if(isLib){
		printf( " ending loop \n");
		loopEnd = true;
	}else{
		printf( " exiting \n");
		quit();
	}
};


void update(){
	

	Vec2d force;

	b1.clean_temp ();
	force.set      ( { 0.0, -9.81*b1.mass } );
	force.add_mul  ( b1.vel, -0.5 );
	b1.apply_force ( force, {0.0,0.0} );
 	b1.apply_anchor( 100.0, {1.0,0.0}, {0.0,5.0} );

	b2.clean_temp ();
	force.set      ( { 0.0, -9.81*b2.mass } );
	force.add_mul  ( b2.vel, -0.5 );
	b2.apply_force ( force, {0.0,5.0} );

	b3.clean_temp ();
	force.set      ( { 0.0, -9.81*b3.mass } );
	force.add_mul  ( b3.vel, -0.5 );
	b3.apply_force ( force, {0.0,0.0} );

	spring1->apply();
	spring2->apply();

	b1.move( dt );
	b2.move( dt );
	b3.move( dt );

	//STOP = true;
};

void setup(){
	int ifree,igl,nvert,ndiv;

	b1.from_mass_points( 2, mass, (Vec2d*)poss );
	b1.setDefaults();
	b1.setAngle( 0.0 );
	b1.pos.set( {0.0, 5.0} );

	b2.from_mass_points( 2, mass, (Vec2d*)poss );
	b2.setDefaults();
	b2.setAngle( 0.0 );
	b2.pos.set( {-2.0, 5.0} );

	b3.from_mass_points( 2, mass, (Vec2d*)poss );
	b3.setDefaults();
	b3.setAngle( 0.0 );
	b3.pos.set( {-4.0, 5.0} );

	int kite = glGenLists(1);
	glNewList( kite , GL_COMPILE );
	glBegin   (GL_QUADS);	       
		glNormal3f( 0.0f, 0.0f, 1.0f );   	     
		glVertex3f( -1.0,  0.0, 0 ); 
		glVertex3f( -0.5, +0.2, 0 );
		glVertex3f( +1.0,  0.0, 0 ); 
		glVertex3f( -0.5, -0.2, 0 );
	glEnd();
	glEndList();

	b1.shape = kite;
	b2.shape = kite;
	b3.shape = kite;

	spring1 = new SpringConstrain2D( 100, &b1, &b2, {-1.0, 0.0 }, { 1.0, 0.0 } );
	spring2 = new SpringConstrain2D( 100, &b2, &b3, {-1.0, 0.0 }, { 1.0, 0.0 } );
		
}

void inputHanding(){
	while(SDL_PollEvent(&event)){ // be carefull to include all event handling instide the while loop !!!
		if( event.type == SDL_KEYDOWN ){ 
			if( event.key.keysym.sym == SDLK_ESCAPE   ) { escape(); }
			if( event.key.keysym.sym == SDLK_SPACE    ) { STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); }
			if( event.key.keysym.sym == SDLK_KP_MINUS ) { thisScreen->zoom*=VIEW_ZOOM_STEP; }
			if( event.key.keysym.sym == SDLK_KP_PLUS  ) { thisScreen->zoom/=VIEW_ZOOM_STEP; }
		} 
		if( event.type == SDL_QUIT){ quit();  };

	} // while(SDL_PollEvent(&event))
	SDL_GetMouseState( &thisScreen->mouseX, &thisScreen->mouseY );
	//printf( "frame %i mouseX moyseY  %i %i   \n", frameCount, mouseX, mouseY );
}


void loop(int n ){
	loopEnd = false;
	for( int iframe=0; iframe<n; iframe++ ){
		inputHanding(); 
		if(!STOP){
			update();
			thisScreen->update();
		}  
		//printf(" %i \n", iframe );
		SDL_Delay( 10 );
		frameCount++;
		if(loopEnd) break;
	} 
}

// FUNCTION ======  main
int main(int argc, char *argv[]){

	// creating windows
	SDL_Init(SDL_INIT_VIDEO);  
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1); 
	int sid;
	thisScreen  = new Screen2D( sid, 800,600); 

	setup();

	printf( " setup done \n" );

	//loop( 1 );
	loop( 1000000 );

	return 0;
}


// ==========================================================
// ===== Export this functions to Dynamic library for Python
// ==========================================================

extern "C"{
}


