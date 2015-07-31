

#include <SDL.h>
#include <math.h>
#include <stdio.h>


const  float INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }

// ===============================
// ===== GLOBAL VARIABLES
// ===============================

#include "include/function2d.h"
#include "include/solve2D.h"


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


double rho = nx*ny;

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

typedef double (*Function2d)( double x, double y);


void setPixelsRGW_array( SDL_Surface* surface, int ix0, int iy0, int ix1, int iy1,   int ny, double * arr, double fscale ){
	//printf( "  setPixels_array \n" );
	SDL_LockSurface( surface  );
	for (int iy=iy0; iy<iy1; iy++){
		Uint32 * pixel  = ((Uint32*)surface->pixels) + iy*surface->w + ix0;
		int i0y = ny * iy;
		for (int ix=ix0; ix<ix1; ix++){
			//printf( " %i %i %i \n ", ix, iy, i0y+ix );
			double f     = arr[ i0y + ix ] * fscale;
			Uint32 color;
			if( f>0 ){
				Uint8  fc    = ( (Uint8)( 255.0*f) ) & 0xFF; 
				color = ((255-fc)<<16)|((255-fc)<<8)|0xFF0000FF;
			}else{
				Uint8  fc    = ( (Uint8)(-255.0*f) ) & 0xFF; 
				color = ((255-fc)<<8)|(255-fc) | 0xFFFF0000;
			}
			*pixel = color;
			pixel++;
		}
	}
	SDL_UnlockSurface( surface  );
}


void draw(){

	rho = solve_step( 0.3, sqrt((nx*ny)/rho) );

	if(frameCount%10==0){
		printf( " %i %f \n", frameCount, rho/(nx*ny) );
		SDL_DestroyTexture(tempTex);
		setPixelsRGW_array( tempSurf, 0, 0, nx, ny,  ny, psi, 1.0 );
		tempTex   = SDL_CreateTextureFromSurface( render, tempSurf  );
	}
	SDL_RenderCopy( render, tempTex, &SrcR, &DestR );


	SDL_RenderPresent( render );
	SDL_UpdateWindowSurface(window);

}

void setup(){
	window          = SDL_CreateWindow( "SDL Tutorial",   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	render        	= SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_UpdateWindowSurface( window );

	pot = new double[ nx*ny ];
	psi = new double[ nx*ny ];

	double sigma2 = 0.0002;
	for (int iy=0; iy<ny; iy++){
		int i0y = iy*nx; 
		for (int ix=0; ix<nx; ix++){
			int ixy = i0y + ix; 
			double x  = ix - (nx>>1);
			double y  = iy - (ny>>1);
			double r2 = x*x + y*y;
			pot[ ixy ] = exp( -r2*sigma2 );
			psi[ ixy ] = randf() - 0.5;
			if( (iy == 0)||(iy == ny-1)||(ix == 0)||(ix == nx-1) ){
				psi[ ixy ] = 0;
			}
		}
	}

	tempSurf  = SDL_CreateRGBSurface(0,SCREEN_WIDTH,SCREEN_HEIGHT,32,0,0,0,0 );
	//setPixels_array( tempSurf, 0, 0, nx, ny,     ny, pot, 1.0 );
	tempTex   = SDL_CreateTextureFromSurface( render, tempSurf  );
	SrcR.x  = 0; SrcR.y  = 0; SrcR.w  = tempSurf->w; SrcR.h  = tempSurf->h;
	DestR.x = 0; DestR.y = 0; DestR.w = tempSurf->w; DestR.h = tempSurf->h;
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
		SDL_Delay( 20 );
	} 

	return 0;
}
