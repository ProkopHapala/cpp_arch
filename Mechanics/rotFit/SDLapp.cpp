
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

typedef void (*Procedure)( );

#include "include/fastmath.h"
#include "include/Vec3.h"
#include "include/Mat3.h"
#include "include/quaternion.h"
#include "include/raytrace.h"


#include "include/drawMath.h"
#include "include/drawUtils.h"
#include "include/trackball.h"
#include "include/ListKeeper.h"

#include "include/optimizerDerivs.h"
#include "include/rotFit.h"

//#include "include/molecule.h"

// ===============================
// ===== GLOBAL CONSTAMNTS
// ===============================

const float	VIEW_ZOOM_STEP     = 1.2f;
const float	VIEW_ZOOM_DEFAULT  = 5.0f;
const float	VIEW_DEPTH_DEFAULT = 1000.0;



// ===============================
// ===== GLOBAL VARIABLES
// ===============================

bool  isLib             = false;
bool  STOP          	= false; 
bool  loopEnd           = false;
int   frameCount		=	0;

SDL_Event		 event; 

ListKeeper * glObjects;



#include "include/Screen.h"
const int nScreensMax=10;
int iScreen;
Screen* screens[nScreensMax];
Screen* thisScreen;


const int npoints = 6;
Vec3d point0s[npoints] = { {-1,0,0}, {+1,0,0},  {0,-1,0}, {0,+1,0},  {0,0,-1}, {0,0,+1} };

SystemTransformOptimizer * optT;


// ===============================
// ===== FUNCTIONS 
// ===============================
void quit();
void escape();
void inputHanding();
void loop(int n );

void update( ){
	optT->fitStep( -1.0, -10.0,-10.0, point0s );

	if( optT->optimizer->getFsqSum() < 0.00001 ) STOP = true;
	//STOP = true;
}

void userDraw(){
	//drawLine( {1,1,1}, {-1,-1,1} );

	//Vec3d * p0  = (Vec3d*)&optT->optimizer->pos[0];
	Mat3d * T   = (Mat3d*)&optT->optimizer->pos[3]; 
	glColor3f(1.0,0.0,0.0); 	drawVecInPos( T->a, {0,0,0} );
	glColor3f(0.0,1.0,0.0); 	drawVecInPos( T->b, {0,0,0} );
	glColor3f(0.0,0.0,1.0); 	drawVecInPos( T->c, {0,0,0} );
	glColor3f(1.0,0.0,1.0);		for (int i=0; i<npoints; i++){	drawLine( point0s[i], optT->Tpoints[i] );	}
	glColor3f(0.0,0.0,0.0);		for (int i=0; i<npoints; i++){	drawLine( {0,0,0}, optT->Tpoints[i] );		}

}

void setup(){
	
	Vec3d * points = new Vec3d[npoints];

	double Rrand = 0.3;
	Vec3d ax; ax.set( 1,2,3 ); ax.normalize();
	Vec3d p0; p0.set( 3,-4,5 );
	//p0.set( 0.0 );
	Mat3d  T; T.setOne( );
	T.a.rotate( 1.0, ax  );	printVec(T.a);
	T.b.rotate( 1.0, ax  ); printVec(T.b);
	T.c.rotate( 1.0, ax  ); printVec(T.c);
	printf( " %f  %f  %f |  %f  %f  %f \n", T.a.dot(T.a), T.b.dot(T.b), T.c.dot(T.c), T.a.dot(T.b), T.a.dot(T.c), T.b.dot(T.c) );
	for( int i=0; i<npoints; i++ ){ 
		Vec3d Tp;
		T.dot_to(  point0s[i], Tp );  
		Tp.add( {randf()*Rrand,randf()*Rrand,randf()*Rrand} );
		points[i].set_add( p0, Tp ); 
	}
	optT = new SystemTransformOptimizer( npoints, points );


	int ifree,igl,nvert,ndiv;

	// make spehre
/*
	ndiv = 10;
	double  sphere_R = 0.5;
	Vec3d sphere_pos; 
	sphere_pos.set(0.5,0.5,0.5);

	igl=glGenLists(1);
	glNewList( igl , GL_COMPILE);
		glShadeModel ( GL_SMOOTH );
		nvert = drawSphere_oct( ndiv, sphere_R, sphere_pos );
	glEndList();
	ifree = glObjects->getFree();
	glObjects->data[ ifree ] = igl;
	printf( " add object %i  %i \n", ifree, igl );
	printf( " sphere %i nvert %i \n", ndiv, nvert );
*/
}


// ===============================
// ===== STANDART FUNCTIONS 
// ===============================

void quit(){
	SDL_Quit(); 
	exit(1);
};

void escape(){
	if(isLib){
		printf( " ending loop \n");
		loopEnd = true;
	}else{
		printf( " exiting \n");
		quit();
	}
};

void inputHanding(){
	while(SDL_PollEvent(&event)){ // be carefull to include all event handling instide the while loop !!!
		if( event.type == SDL_KEYDOWN ){ 
			if( event.key.keysym.sym == SDLK_ESCAPE   ) { escape(); }
			if( event.key.keysym.sym == SDLK_SPACE    ) { STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); }
			if( event.key.keysym.sym == SDLK_KP_MINUS ) { thisScreen->zoom*=VIEW_ZOOM_STEP; }
			if( event.key.keysym.sym == SDLK_KP_PLUS  ) { thisScreen->zoom/=VIEW_ZOOM_STEP; }
		} 
		if( event.type == SDL_QUIT){ quit();  };
		if( event.type == SDL_MOUSEMOTION ){
				thisScreen->mouse_camera( event.motion.x, event.motion.y );
		}
		if( event.type == SDL_MOUSEBUTTONDOWN )    {
			if( event.button.button == SDL_BUTTON_LEFT ) {
				thisScreen->startSpining( event.button.x, event.button.y );
				//printf("SDL_MOUSEBUTTONDOWN\n");
				//mouse_picking();
			}
		} 
		if( event.type == SDL_MOUSEBUTTONUP ){
			if( event.button.button == SDL_BUTTON_LEFT ){ thisScreen->mouse_spinning = 0; }
		}
		if (event.type == SDL_WINDOWEVENT) {
			if( event.window.event ==  SDL_WINDOWEVENT_FOCUS_GAINED ){
            	SDL_Log( "Window %d gained focus",event.window.windowID );
				thisScreen = screens[event.window.windowID];
			}
		}
	} // while(SDL_PollEvent(&event))
	SDL_GetMouseState( &thisScreen->mouseX, &thisScreen->mouseY );
	//printf( "frame %i mouseX moyseY  %i %i   \n", frameCount, mouseX, mouseY );
}


void loop(int n ){
	loopEnd = false;
	for( int iframe=0; iframe<n; iframe++ ){
		inputHanding(); 
		if(!STOP){
			update();
			for( int i=0; i<nScreensMax; i++ )	{
				//printf(" %i %i %i \n", iframe, i, screens[i] );
				if( screens[i]!=NULL ) { screens[i]->update(); }
			}
		}  
		//printf(" %i \n", iframe );
		SDL_Delay( 25 );
		frameCount++;
		if(loopEnd) break;
	} 
}

// FUNCTION ======  main
int main(int argc, char *argv[]){

	glObjects = new ListKeeper( 16 );

	// creating windows
	SDL_Init(SDL_INIT_VIDEO);  
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1); 
	int sid;
	thisScreen  = new Screen( sid, 800,600); screens[sid] = thisScreen; printf( " screen id %i %i \n", sid, screens[sid]  );
	//thisScreen  = new Screen( sid, 600,600); screens[sid] = thisScreen; printf( " screen id %i %i \n", sid, screens[sid]  );
	//thisScreen  = new Screen( sid, 400,600); screens[sid] = thisScreen; printf( " screen id %i %i \n", sid, screens[sid]  );

	setup();

	thisScreen->userDraw = &userDraw;

	printf( "here\n" );

	//loop( 1 );
	loop( 1000000 );

	return 0;
}


// ==========================================================
// ===== Export this functions to Dynamic library for Python
// ==========================================================

extern "C"{
}


