

#include <SDL.h>
#include <math.h>
#include <stdio.h>


const  float INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }

// ===============================
// ===== GLOBAL VARIABLES
// ===============================

#include "lingebra.h"
#include "function2d.h"
#include "potential.h"
#include "optimizer_FIRE.h"


//const int SCREEN_WIDTH  = 1024;
const int SCREEN_WIDTH  = 512;
const int SCREEN_HEIGHT = 512;

const int IY0 = SCREEN_HEIGHT/2;
const int IX0 = IY0;

const double ZOOM  = 30.0;
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

int perFrame = 100;

OptimizerDerivs* opt1;

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

typedef double (*Func1d)( double x );

void plotFunc( int n, double x1, double x2, Func1d func ){
	double dx = (x2-x1)/n;
	double ox = x1;
	double oy = func(ox); 
	for ( int i=0; i<n; i++ ){
		double x = i*dx + x1;
		double y = func(x); 
		SDL_RenderDrawLine    ( render,  x2i(ox), y2i(-oy), x2i(x), y2i(-y) );
		ox=x; oy=y;
	}
}


void draw(){

	SDL_SetRenderDrawColor(render,  255, 255, 255, 255 );   SDL_RenderClear(render);

	SDL_SetRenderDrawColor( render, 128, 128, 128, 255  );
	SDL_RenderDrawLine    ( render, 0, y2i(0    ), SCREEN_WIDTH, y2i(0) );

	for (int i=0; i<perFrame; i++){ opt1->step(); }

	SDL_SetRenderDrawColor( render, 0, 255, 0, 255  );
	plotFunc( 50, -5, 5, &evalInPoint );


	SDL_SetRenderDrawColor( render, 255, 0, 0, 255  );
	for ( int i=0; i<ncontrol; i++ ){
		SDL_RenderDrawPoint( render, x2i(control_x[i]), y2i(-control_val[i]) );
		//printf( " %f %f %i %i \n", control_x[i], control_y[i], x2i(control_x[i]), y2i(control_y[i]) );
	}

	SDL_SetRenderDrawColor( render, 0, 255, 0, 255  );
	for ( int i=0; i<nsource; i++ ){
		//SDL_RenderDrawPoint( render, x2i(source_x[i]), y2i(0) );
		SDL_RenderDrawLine    ( render,  x2i(source_x[i]), y2i(0    ), x2i(source_x[i]), y2i(-source_val[i]    ) );
		//printf( " %f %f %i %i \n", source_x[i], source_y[i], x2i(source_x[i]), y2i(source_y[i]) );
	}

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

	//opt1 = new OptimizerRandom_3( 2, xBest, dxMax, 0.7, 1.2, &fittnessFunc_rosenbrok );

	makeCPoints(-5,5);
	makeCouplings();
	//opt1 = new OptimizerDerivs( nsource, source_val, new double[nsource], new double[nsource], &getVarDerivs_ );
	opt1 = new OptimizerFIRE( nsource, source_val, new double[nsource], new double[nsource], &getVarDerivs_ );

	double ** A = new_matrix( ncontrol, nsource );
	//double *  x = double[ nsource ];
	set( ncontrol, nsource, couplings,  A );

	leastSquareFit_Gauss( nsource, ncontrol, A, control_val, source_val );

	


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
