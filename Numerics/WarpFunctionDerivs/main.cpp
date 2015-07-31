

#include <SDL.h>
#include <math.h>
#include <stdio.h>


const  float INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }



// ===============================
// ===== GLOBAL VARIABLES
// ===============================

#include "warpFunction.h"

//const int SCREEN_WIDTH  = 1024;
const int SCREEN_WIDTH  = 512;
const int SCREEN_HEIGHT = 512;

const int IY0 = SCREEN_HEIGHT/2;
const int IX0 = IY0;

const double ZOOM  = 100.0;
const double IZOOM = 1.0d/ZOOM;

double i2x( int ix ){ return (ix - IX0)*IZOOM;  };
double i2y( int iy ){ return (iy - IY0)*IZOOM;  };

int x2i( double  x ){ return x*ZOOM + IX0;     };
int y2i( double  y ){ return y*ZOOM + IY0;     };



SDL_Renderer*	render			= NULL;  
SDL_Window*		window        	= NULL;
SDL_Surface*	screenSurface 	= NULL;
SDL_Surface*	tempSurf;
SDL_Texture*	tempTex;

SDL_Rect SrcR;
SDL_Rect DestR;

SDL_Event		event; 
bool 			STOP          	= false; 
int 			frameCount		=	0;


// ====================================
// ===== FUNCTION FORWARD DECLARATIONS
// ====================================

void quit(){SDL_Quit(); exit(1);};
void setup();
void draw();
void inputHanding();

// ===============================
// ===== FUNCTION IMPLEMENTATION
// ===============================

void draw(){
	//SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
    //SDL_RenderClear(render);
	SDL_RenderCopy( render, tempTex, &SrcR, &DestR);

	int mouseX,mouseY;
	SDL_GetMouseState( &mouseX, &mouseY );

	double dfdx=0,dfdy=0;
	double x = i2x( mouseX );
	double y = i2y( mouseY );
	double f = warped_function( x, y, dfdx, dfdy );
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderDrawLine    ( render, x2i( x ), y2i( y ), x2i( x - dfdx ), y2i( y - dfdy ) ); 
	//printf( " %f %f %f %f \n", x, y, dfdx, dfdy );

	SDL_RenderPresent( render );
	SDL_UpdateWindowSurface(window);
}

void setPixelsFunction( SDL_Surface* surface, int ix0, int iy0, int ix1, int iy1, Function2d func ){
	SDL_LockSurface( surface  );
	int nx = ix1-ix0;
	int ny = iy1-iy0;
	for (int iy=iy0; iy<iy1; iy++){
		Uint32 * pixel  = ((Uint32*)surface->pixels) + iy*surface->w + ix0;
		double y = i2y( iy );
		for (int ix=ix0; ix<ix1; ix++){
			double x = i2x( ix );
			double f = func( x, y );
			Uint8  fc    = ( (Uint8)(255.0*f) ) & 0xFF; 
			Uint32 color = (fc<<16)|(fc<<8)|fc | 0xFF000000;
			*pixel = color;
			pixel++;
		}
	}
	SDL_UnlockSurface( surface  );
}


void setup(){
	window          = SDL_CreateWindow( "SDL Tutorial",   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	render        	= SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_UpdateWindowSurface( window );

	tempSurf        = SDL_CreateRGBSurface(0,SCREEN_WIDTH,SCREEN_HEIGHT,32,0,0,0,0 );

	setPixelsFunction( tempSurf, 0, 0, tempSurf->w,    tempSurf->h, &warped_function_noDiff );

	tempTex   = SDL_CreateTextureFromSurface( render, tempSurf  );

	SrcR.x  = 0; SrcR.y  = 0; SrcR.w  = tempSurf->w; SrcR.h  = tempSurf->h;
	DestR.x = 0; DestR.y = 0; DestR.w = tempSurf->w; DestR.h = tempSurf->h;

	SDL_RenderCopy( render, tempTex, &SrcR, &DestR);

	SDL_SetRenderDrawBlendMode( render, SDL_BLENDMODE_BLEND );
	
}

void inputHanding(){
	while(SDL_PollEvent(&event)){ 
		if( event.type == SDL_KEYDOWN ){ 
			if(event.key.keysym.sym == SDLK_ESCAPE) { quit(); }
			if(event.key.keysym.sym == SDLK_SPACE    ){ STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); }
		} 
		if( event.type == SDL_QUIT){ quit();  };
	}
}

int main( int argc, char* args[] ){

	setup();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
		if (!STOP){ draw();  } 
		inputHanding(); 
		SDL_Delay( 100 );
	} 

	return 0;
}
