
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "math2D_double.h"
#include "SDLtest2D.h"
#include "Formation.h"


UnitType type1;

Regiment reg1( 6, &type1 );

void setup(){

	for(int i=0; i<reg1.n; i++){
		reg1.bodies[i].vel.set( 0 );
		reg1.bodies[i].pos.set( i, randf(-0.5,0.5) );
		print( reg1.bodies[i].pos );
	}
}

// ===== DRAW

void draw(){
	reg1.cohesionForces(  );
	reg1.draw( );
	reg1.move(0.5);
	printf( " iter %i \n", frameCount );
};


// FUNCTION ======  main
int main(){
	setup();
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
			//setup(); STOP = true;
			draw(); 
			glFinish();
			SDL_GL_SwapBuffers(); 
		} 
		inputHanding(); 
		SDL_Delay(  100 );
	} 
}


