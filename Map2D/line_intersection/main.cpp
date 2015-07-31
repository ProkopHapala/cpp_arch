
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "math2D_double.h"

#include "SDLtest2D.h"



void setup(){

// ========= TEST 1 -  Points in Rect
	srand(1933);
	draw_Axis( );
	Rect2d rc; rc.set( {randf( 0,1 ), randf( 0,1 )} , {randf( 0,1 ), randf( 0,1 )} ); 
	draw_Rect2d( rc );
	for ( int i = 0; i<101; i++){
		Vec2d p; p.set( randf( 0,1 ), randf( 0,1 ) );
		if( rc.pointIn( p ) ) { colorHash( i ); draw_Vec2d( p ); }
		//print( p );
	}

// ========= TEST 2 -  Points in Triangle
	srand(12);
	colorHash( 16 );
	rc.set( {1.1,0}, {2.1,1} );  draw_Rect2d( rc );
	Vec2d ta,tb,tc;
	ta.set( randf( rc.x0 , rc.x1 ), randf( rc.y0 , rc.y1 )  );
	tb.set( randf( rc.x0 , rc.x1 ), randf( rc.y0 , rc.y1 ) );
	tc.set( randf( rc.x0 , rc.x1 ), randf( rc.y0 , rc.y1 ) );
	Triangle2d* t = new Triangle2d ( 1, &ta, &tb, &tc );
	print( t );
	draw_Triangle2d( t );
	paint_Vec2d( *t->a, 0.02 );
	for ( int i = 0; i<1000; i++){
		Vec2d p; p.set( randf( rc.x0 , rc.x1 ), randf( rc.y0 , rc.y1 ) );
		if( t->pointIn( p ) ) { colorHash( i ); draw_Vec2d( p ); }
		//print( p );
	}

// ========= TEST 3 -  LineIntersection
	srand(178);
	rc.set( {0.0,1.1}, {1.0,2.1} );  draw_Rect2d( rc );
	for ( int i = 0; i<3; i++){
		colorHash( i );
		Vec2d l1a, l1b, l2a, l2b, p;
		l1a.set( randf( rc.x0 , rc.x1 ), randf( rc.y0 , rc.y1 ) );
		l1b.set( randf( rc.x0 , rc.x1 ), randf( rc.y0 , rc.y1 ) );
		l2a.set( randf( rc.x0 , rc.x1 ), randf( rc.y0 , rc.y1 ) );
		l2b.set( randf( rc.x0 , rc.x1 ), randf( rc.y0 , rc.y1 ) );
 		draw_Segment2d( l1a, l1b );
		draw_Segment2d( l2a, l2b );
		// == version using Lin2d
		//Lin2d l1, l2;
		//l1.set( l1a, l1b );
		//l2.set( l2a, l2b );
		//intersection ( l1, l2, p);  paint_Vec2d( p, 0.02 );
		// == version using Vec2d
		unsigned char mask = intersection_point( l1a, l1b, l2a, l2b, p);
		printf( " %i %i \n", i, mask );
		if (mask==0) { paint_Vec2d( p, 0.02 ); } else {   paint_Vec2d( p, 0.01 );  }
		//print( p );
	}
}

// ===== DRAW

void draw(){
	
};


// FUNCTION ======  main
int main(){
	setupSDLTest2D();
	//setup();
	for( frameCount=0; frameCount<1000000; frameCount++ ){
	//for( frameCount=0; frameCount<1; frameCount++ ){
		if (!STOP){ 
			camera2D( );
			glClearColor( 0.5, 0.5, 0.5, 0.0);
			glDisable ( GL_DEPTH_TEST );
			glDisable ( GL_LIGHTING);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
			glTranslatef( -1.0,-1.0,0 );
			setup(); STOP=true;
			draw(); 
			glFinish();
			SDL_GL_SwapBuffers(); 
		} 
		inputHanding(); 
		SDL_Delay(  100 );
	} 

}


