
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
#include "include/drawUtils.h"

#include "include/MapCell2D.h"
#include "include/MapTile2D.h"
#include "include/Map2D_grid.h"

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
MapTile2D* tile;



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
	
	//STOP = true;
};

void setup(){
	int ifree,igl,nvert,ndiv;
	
	tile = new MapTile2D( 4 );
	for( int iy=0; iy<tile->n; iy++ ){
		int icelly = iy<<tile->pow;
		for( int ix=0; ix<tile->n; ix++ ){
			int icell = icelly + ix;
			//tile->cells[ icell ].val = randf( );
			//tile->cells[ icell ].val = randf( ) - 0.5;
			tile->cells[ icell ].val = ( rand()&1 * 2 ) - 1;
		}
	}

	tile->make_render( -3.0, -3.0, 0.5, 0.5, NULL, NULL, NULL );
	//tile->make_render_smooth( -3.0, -3.0, 0.5, 0.5, NULL, NULL, NULL );

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
		SDL_Delay( 200 );
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


