
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "SDLtest2D.h"

#include "GridMap2D.h"

GridMap2D_dynamic<Point2D> map;

void setup(){

	map.init( 16, 14, 0.2 );

	printf( " %f \n", map.invStep );

/*
	Point2D* p = new Point2D( { 10.8, 13.9 } ); 
	printf( " p: %f %f \n",p->x,p->y );
	int ix,iy;
	map.boxIndex( p, ix, iy );
	printf( " ix,iy: %i %i \n",ix,iy );

*/

	for (int i=0; i<1000; i++){
		Point2D* p = new Point2D( { randf(0,2), randf(0,2) } );
		map.insert( p );
		//printf( " i:i0,ii,i0s[ii]  %i: %i %i %i \n", i, i0, ii, map.i0s[ii] );
	};

	map.makeStatic();


}

// ===== DRAW

void draw(){
	glClearColor( 0.5, 0.5, 0.5, 0.0);
	glDisable (GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

/*
	glBegin( GL_POINTS );
		glColor3f (0.9,0.9,0.9); 
		for (int i=0; i<map.nxy; i++){
			auto list = map.store[i];
			for ( auto ci = list->cbegin(); ci != list->cend(); ++ci){
				Point2D* p = *ci;
				glVertex3f( (float)(p->x) , (float)(p->y), 0 );
				printf( " i,px,py: %i %f %f \n", i ,p->x,p->y );
			}
		};
	glEnd( );
*/

	map.paintGridLines();
	map.paintStorage();

};


// FUNCTION ======  main
int main(){
	setupSDLTest2D();
	setup();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
	//for( frameCount=0; frameCount<1; frameCount++ ){
		if (!STOP){ 
			camera2D( );
			draw(); 
			glFinish();
			SDL_GL_SwapBuffers(); 
		} 
		inputHanding(); 
		SDL_Delay(  100 );
	} 

}


