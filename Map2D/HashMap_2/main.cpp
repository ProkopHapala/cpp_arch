
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "math2D_double.h"
#include "SDLtest2D.h"
#include "HashMap.h"
#include "Map2D.h"

const int power = 8;
const int n     = 1<<power; 

Map2D map( 0.15, power+1, 10 );
Vec2d ps[ n ];

int lastIndex = 0;

/*
HashMap<int> hashmap;
int          ints[ n ]; 
*/

void setup(){

// === Map2D indexing
/*
	for (int i=0; i<10; i++ ){
		double x     = randf( -10,10 );
		double y     = randf( -10,10 );
		short int ix = map.getIx( x );
		short int iy = map.getIx( y );
		unsigned int index  = map.getIndex( ix, iy );
		printf( " %i %f %f %i %i %i \n", i, x, y, ix, iy, index );
	}
*/

// === HashMap test
/*
	hashmap.init( power+1 );
	printf( " n %i capacity %i \n ", n, hashmap.capacity );
	for ( int i=0; i<n; i++ ){
		ints[i] = rand();
		hashmap.insert( &ints[i], i );
	}

	for ( int i=0; i<hashmap.capacity; i++ ){
		if ( hashmap.store[i] != NULL ) { printf( " %i hash %i box %i \n", i,  hashmap.hashs[i],  hashmap.iboxs[i]  ); }
		else                            { printf( " %i NULL\n", i );  }
	}
*/

// === save random points into Map2D
	for ( int i=0; i<n; i++ ){
		ps[i].set( randf( -1.0,1.0 ), randf( -1.0,1.0 ) );
		//colorHash( i ); draw_Vec2d( p );
		//print( ps[i] );
		map.insertPoint( ps[i] );
	}
	printf( "  power %i n %i capacity %i mask %i \n ", power, n, map.points.capacity, map.points.mask );

// === Performance test

	int ntest = 10000000;
    long long t1,t2;
	double checksum = 0;
	int counter;

	counter = 0;
	t1 =  getCPUticks();
	for ( int i =0; i < ntest; i++ ){
		Vec2d p; p.set( randf( -1.0,1.0 ), randf( -1.0,1.0 ) );
		map.loadPointsIn( p );
		counter += map.points.DEBUG_counter;
	}
	t2 =  getCPUticks();
	printf( "  map.loadPointsIn       : checksum  %f  ticks/op  %f  iter/op %f  \n", checksum, (t2-t1)/(double)ntest, counter/(double)ntest );

	counter = 0;
	t1 =  getCPUticks();
	for ( int i =0; i < ntest; i++ ){
		Vec2d p; p.set( randf( -1.0,1.0 ), randf( -1.0,1.0 ) );
		map.loadPointsIn_noHash( p );
		counter += map.points.DEBUG_counter;
	}
	t2 =  getCPUticks();
	printf( "  map.loadPointsIn_noHash : checksum  %f  ticks/op  %f  iter/op %f \n", checksum, (t2-t1)/(double)ntest, counter/(double)ntest );


}

// ===== DRAW

void draw(){

	//	draw Points
	glColor3f( 0.3,0.3,0.3 );
	for ( int i=0; i<n; i++ ){		draw_Vec2d( ps[i] ); 	};
	//	draw map.points.hits
	for ( int i=0; i<map.points.capacity; i++ ){
	   draw_Segment2d( {-0.5+i*pixsz,-0.5}, { -0.5+i*pixsz, -0.5	+ map.points.hits[i] * 3*pixsz } );
	}

	Vec2d pmouse; pmouse.set( (mouseX-WIDTH_HALF)*pixsz, (HEIGHT_HALF-mouseY)*( pixsz*ASPECT_RATIO ) );
	int ix    = map.getIx( pmouse.x );
	int iy    = map.getIy( pmouse.y );
	int index = map.getBoxIndex( ix, iy );
	//colorHash( map.getBoxIndex( ix, iy )  );
	glColor3f( 0.9,0.7,0.8 );
	draw_Rect2d( { map.getX(ix), map.getY(iy) }, { map.getX(ix+1), map.getY(iy+1) } );
	//printf( " rect X Y %i %i %f %f \n", ix, iy, map.getX(ix), map.getY(iy) );
	//printf( " mouse X Y %i %i \n", mouseX, mouseY );
	paint_Vec2d( pmouse, 0.01 );
	int nloaded = map.loadPointsIn( pmouse );
	for ( int i=0; i<nloaded; i++ ){
		int ii = map.tempi[i]; 
		Vec2d* pp = map.points.store[ ii ];
		paint_Vec2d( *pp, 0.0025 );
	};
	if( index != lastIndex ) printf( " ix iy index %i %i %i iters %i \n", ix, iy, index, map.points.DEBUG_counter  );
	lastIndex = index;

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


