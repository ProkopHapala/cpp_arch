
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "SDLtest2D.h"

#include "GridMap2D_Line.h"

GridMap2D<Line2D> map;

int nlines = 4;
Line2D** lines; 

double xs[] = { 1,3,4,3,1 };
double ys[] = { 2,1,3,4,2 };

void setup(){

	map.init( 100, 100, 0.07 );

	//printf( " %f \n", map.invStep );

	srand(1245);
	lines = new Line2D*[ nlines ];
	for (int i=0; i<nlines; i++){
		Point2D* a = new Point2D( { randf(1,4), randf(1,4) } );
		Point2D* b = new Point2D( { randf(1,4), randf(1,4) } );
		//Point2D* a = new Point2D( { xs[i  ], ys[i  ] } );
		//Point2D* b = new Point2D( { xs[i+1], ys[i+1] } );
		Line2D*  l = new Line2D( i, a, b );
		lines [ i ] = l; 
		map.insertLine( l );
		//printf( " i:i0,ii,i0s[ii]  %i: %i %i %i \n", i, i0, ii, map.i0s[ii] );
	};

	//map.makeStatic();
}

// ===== DRAW

void draw(){
	
	map.paintStorage();
	map.paintGridLines();
	for (int i=0; i<nlines; i++){	lines[ i ]->draw();	}

};


// FUNCTION ======  main
int main(){
	setupSDLTest2D();
	setup();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
	//for( frameCount=0; frameCount<1; frameCount++ ){
		if (!STOP){ 
			camera2D( );
			glClearColor( 0.5, 0.5, 0.5, 0.0);
			glDisable ( GL_DEPTH_TEST );
			glDisable ( GL_LIGHTING);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
			glTranslatef( -2.5,-2.5,0 );
			//setup(); STOP=true;
			draw(); 
			glFinish();
			SDL_GL_SwapBuffers(); 
		} 
		inputHanding(); 
		SDL_Delay(  100 );
	} 

}


