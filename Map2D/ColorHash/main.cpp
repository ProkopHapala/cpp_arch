
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "math2D_double.h"
#include "SDLtest2D.h"

void setup(){

	printf(" PixelSize pixsz %f \n", pixsz );
	draw_Rect2d( {-1,-1}, {1,1} );
	for ( int iy=-200; iy<200; iy++ ){ for ( int ix=-200; ix<200; ix++ ){
		colorHash( ( iy << 16 ) ^ ix  ); 
		//glColor3f( ix*0.005, iy*0.005, 0 );
		draw_Vec2d( { pixsz*ix, pixsz*iy } );
		//print( ps[i] );
	} }


}

// ===== DRAW

void draw(){

};


// FUNCTION ======  main
int main(){
	//setup();
	setupSDLTest2D();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
	//for( frameCount=0; frameCount<1; frameCount++ ){
		if (!STOP){ 
			camera2D( );
			//glClearColor( 0.5, 0.5, 0.5, 0.0);
			glDisable ( GL_DEPTH_TEST );
			glDisable ( GL_LIGHTING);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
			//glTranslatef( -0.5,-0.5,0 );
			setup(); STOP = true;
			//draw(); 
			glFinish();
			SDL_GL_SwapBuffers(); 
		} 
		inputHanding(); 
		SDL_Delay(  100 );
	} 
}


