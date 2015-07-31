
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "include/fastmath.h"
#include "include/Vec3.h"
#include "include/Mat3.h"
#include "include/quaternion.h"
#include "include/raytrace.h"

#include "include/drawMath.h"
#include "include/drawUtils.h"
#include "include/trackball.h"
#include "include/ListKeeper.h"

#include "include/TrinagleMesh.h"



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

ListKeeper * glObjects;

bool OPTIMIZING = true;


#include "include/Screen.h"
const int nScreensMax=10;
int iScreen;
Screen* screens[nScreensMax];
Screen* thisScreen;


Vec3d a,b,c;
Vec3d ray0,hRay;
Vec3d phit;


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
	
	//STOP = true;
};

void setup(){
	int ifree,igl,nvert,ndiv;
	

	ray0.set( 1.0, 1.0, 1.0 );
	hRay.set( -1.0, -1.0, -1.0 );  hRay.normalize();

	a.set( 1.0, 0.0, 0.0 );
	b.set( 0.0, 1.0, 0.0 );
	c.set( 0.0, 0.0, 1.0 );


	bool inside =  false;
	double dist = rayTriangle(  ray0, hRay,   a,b,c,  inside, phit );
	if(inside){
		printf( " triagnle hit: TRUE   in dist: %f ", inside, dist );
	}else{
		printf( " triagnle hit: FALSE  in dist: %f ", inside, dist );
	}

	ifree = glObjects->getFree();
	igl = glGenLists(1);
	glObjects->data[ ifree ] = igl;
	glNewList( igl , GL_COMPILE );
		glColor3f( 0.9,0.9,0.9 );
		drawPointCross( a, 0.1 );
		drawPointCross( b, 0.1 );
		drawPointCross( c, 0.1 );
		drawTriangle( a, b, c );
		glColor3f( 0.1,0.1,0.1 );
		drawVecInPos  ( hRay*10, ray0 );
		drawPointCross( phit, 0.2 );
	glEndList();



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
		if( event.type == SDL_MOUSEMOTION ){
				thisScreen->mouse_camera( event.motion.x, event.motion.y );
		}
		if( event.type == SDL_MOUSEBUTTONDOWN )    {
			if( event.button.button == SDL_BUTTON_LEFT ) {
				thisScreen->startSpining( event.button.x, event.button.y );
				//printf("SDL_MOUSEBUTTONDOWN\n");
				//mouse_picking();
			}
		} 
		if( event.type == SDL_MOUSEBUTTONUP ){
			if( event.button.button == SDL_BUTTON_LEFT ){ thisScreen->mouse_spinning = 0; }
		}
		if (event.type == SDL_WINDOWEVENT) {
			if( event.window.event ==  SDL_WINDOWEVENT_FOCUS_GAINED ){
            	SDL_Log( "Window %d gained focus",event.window.windowID );
				thisScreen = screens[event.window.windowID];
			}
		}
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
			for( int i=0; i<nScreensMax; i++ )	{
				//printf(" %i %i %i \n", iframe, i, screens[i] );
				if( screens[i]!=NULL ) { screens[i]->update(); }
			}
		}  
		//printf(" %i \n", iframe );
		SDL_Delay( 10 );
		frameCount++;
		if(loopEnd) break;
	} 
}

// FUNCTION ======  main
int main(int argc, char *argv[]){

	glObjects = new ListKeeper( 16 );

	// creating windows
	SDL_Init(SDL_INIT_VIDEO);  
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1); 
	int sid;
	thisScreen  = new Screen( sid, 800,600); screens[sid] = thisScreen; printf( " screen id %i %i \n", sid, screens[sid]  );

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


