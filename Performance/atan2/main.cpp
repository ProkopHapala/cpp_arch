



#include <SDL.h>
#include <math.h>
#include <stdio.h>

#include "include/tan2approx.h"

const  float INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }

inline uint64_t rdtsc(){
    uint32_t lo, hi;
    __asm__ __volatile__ (
      "xorl %%eax, %%eax\n"
      "cpuid\n"
      "rdtsc\n"
      : "=a" (lo), "=d" (hi)
      :
      : "%ebx", "%ecx" );
    return (uint64_t)hi << 32 | lo;
}

// ===============================
// ===== GLOBAL VARIABLES
// ===============================

//const int SCREEN_WIDTH  = 1024;
const int SCREEN_WIDTH  = 512;
const int SCREEN_HEIGHT = 512;

const int IY0 = SCREEN_HEIGHT/2;
const int IX0 = IY0;

const double ZOOM  = 60.0;
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


#include "include/function_utils.h"

const int nsamples = 1024;
double * xs = new double[nsamples];
double * ys = new double[nsamples];


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

	SDL_SetRenderDrawColor(render,  255, 255, 255, 255 );   SDL_RenderClear(render);

	SDL_SetRenderDrawColor( render, 128, 128, 128, 255  );
	SDL_RenderDrawLine    ( render, 0, y2i(0    ), SCREEN_WIDTH, y2i(0)  );
	SDL_RenderDrawLine    ( render, x2i(0    ), 0, x2i(0), SCREEN_HEIGHT );



	SDL_SetRenderDrawColor( render, 0, 255, 0, 255  );
	//plotFunc( 100, -3.14159265359, 3.14159265359, 1.0, &atan2_test );
	//plotFunc( 100, -3.14159265359, 3.14159265359, 1.0, &atan2_a1_test );
	//plotFunc( 100, -3.14159265359, 3.14159265359, 1.0, &atan2_a2_test );

	SDL_SetRenderDrawColor( render, 255, 128, 128, 255  ); plotFunc( 1000, -3.14159265359, 3.14159265359, 10000.0, &atan2_a1_err );
	SDL_SetRenderDrawColor( render, 128, 255, 128, 255  ); plotFunc( 1000, -3.14159265359, 3.14159265359, 10000.0, &atan2_a2_err );
	SDL_SetRenderDrawColor( render, 128, 255, 128, 255  ); plotFunc( 1000, -3.14159265359, 3.14159265359, 10000.0, &atan2_a3_err );

	double maxAbs, sumAbs, sumSq;
	funcNorm( 1000, -3.14159265359, 3.14159265359,  atan2_a1_err, maxAbs, sumAbs, sumSq ); printf( "maxAbs, sumAbs, sumSq %g %g %g \n", maxAbs, sumAbs/1000, sqrt(sumSq) );
	funcNorm( 1000, -3.14159265359, 3.14159265359,  atan2_a2_err, maxAbs, sumAbs, sumSq ); printf( "maxAbs, sumAbs, sumSq %g %g %g \n", maxAbs, sumAbs/1000, sqrt(sumSq) );
	funcNorm( 1000, -3.14159265359, 3.14159265359,  atan2_a3_err, maxAbs, sumAbs, sumSq ); printf( "maxAbs, sumAbs, sumSq %g %g %g \n", maxAbs, sumAbs/1000, sqrt(sumSq) );


	for( int i=0; i<nsamples; i++  ){
    	float phi = 15454.154787*i;
		xs[i]=sin( phi );
		ys[i]=cos( phi );
	} 

	int ntimes = 1e+8;
	long t1,t2;
	double sum = 0;

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii   = i & 0x3FF;		double x = xs[ii];		double y = ys[ii];
		sum += x + y;
	}  
	t2 =  rdtsc();
	printf( " x+y : %f ticks/op for n= %i      | %g \n", ( ((double)(t2-t1))/ntimes ),   ntimes, sum );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii   = i & 0x3FF;		double x = xs[ii];		double y = ys[ii];
		sum += atan2_a1( y, x );
	}  
	t2 =  rdtsc();
	printf( " atan2_a1 : %f ticks/op for n= %i      | %g \n", ( ((double)(t2-t1))/ntimes ),   ntimes, sum );


	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii   = i & 0x3FF;		double x = xs[ii];		double y = ys[ii];
		sum += atan2_a2( y, x );
	}  
	t2 =  rdtsc();
	printf( " atan2_a2 : %f ticks/op for n= %i      | %g \n", ( ((double)(t2-t1))/ntimes ),   ntimes, sum );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii   = i & 0x3FF;		double x = xs[ii];		double y = ys[ii];
		sum += atan2_a3( y, x );
	}  
	t2 =  rdtsc();
	printf( " atan2_a3 : %f ticks/op for n= %i      | %g \n", ( ((double)(t2-t1))/ntimes ),   ntimes, sum );



	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii   = i & 0x3FF;		double x = xs[ii];		double y = ys[ii];
		sum += atan2( y, x );
	}  
	t2 =  rdtsc();
	printf( " atan2 : %f ticks/op for n= %i      | %g \n", ( ((double)(t2-t1))/ntimes ),   ntimes, sum );


	STOP = true;

	SDL_RenderPresent( render );
	SDL_UpdateWindowSurface(window);

}

void setup(){
	window          = SDL_CreateWindow( "SDL Tutorial",   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	render        	= SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_UpdateWindowSurface( window );

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
