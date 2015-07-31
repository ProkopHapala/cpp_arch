
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "include/fastmath.h"

#include "include/Vec2.h"
#include "include/Vec3.h"
#include "include/Mat3.h"

#include "include/raytrace.h"

#include "include/drawMath.h"
#include "include/drawMath2D.h"

#include "include/drawUtils.h"

#include "include/Body2D.h"
#include "include/AeroSurf2D.h"
#include "include/Ship2D.h"

#include "include/Chain2D.h"

// ===============================
// ===== GLOBAL CONSTAMNTS
// ===============================

const float	VIEW_ZOOM_STEP     = 1.2f;
const float	VIEW_ZOOM_DEFAULT  = 10.0f;
const float	VIEW_DEPTH_DEFAULT = 1000.0;

// ===============================
// ===== GLOBAL VARIABLES
// ===============================

bool  isLib             = false;
bool  STOP          	= false; 
bool  loopEnd           = false;
int   frameCount		=	0;

SDL_Event		 event; 

int perFrame = 10;

double dt = 0.0001;

Vec2d windSpeed, watterSpeed;

Yacht2D yacht1;

Chain2D * chain1;


const int npts = 4;
static double poss[npts*2] = { -1.0, 0.0,   0.0, -0.1,   0.0, +0.1,   +1.0, 0.0  };
static double mass[npts  ] = {  10.0, 50.0, 50.0, 10.0  };

const int nryb = 200;
double rybMaxDist = 100.0;
Vec2d ryby[nryb];
int tvar_ryby;


#include "include/Screen2D.h"
Screen2D* thisScreen;



// ===============================
// ===== FUNCTIONS 
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


void gen_in_circle( double rmax, Vec2d& p ){
	double r2max=rmax*rmax;	
	while( true ){
		double x  = rmax*(randf( )*2.0-1.0);
		double y  = rmax*(randf( )*2.0-1.0);
		double r2 = x*x + y*y;
		if( r2<r2max ){
			p.set(x,y); 
			return;
		}
	}
}


void update(){
	
	//yacht1.applySailForces(  { 0.0, 0.0 },  { 0.0, 1.0 }  );
	//yacht1.move( dt );

/*
	for( int i=0; i<perFrame; i++ ){
		yacht1.clean_temp( );
		yacht1.applySailForces(  windSpeed,  watterSpeed );
		yacht1.move( dt );
	}
*/

	double rybMaxDist2 = rybMaxDist*rybMaxDist;
	for (int i=0; i<nryb; i++){
		double r2 = yacht1.pos.dist2( ryby[i] );
		if( r2>rybMaxDist2 ){ 
			gen_in_circle( rybMaxDist, ryby[i] );
			ryby[i].add( yacht1.pos ) ;
		};
	}

	if( yacht1.phi != yacht1.phi ) STOP = true;

	//STOP = true;
};

void setup(){
	int ifree,igl,nvert,ndiv;




	tvar_ryby = glGenLists(1);
	glNewList( tvar_ryby , GL_COMPILE );
	glBegin   (GL_TRIANGLES);	       
		glNormal3f( 0.0f, 0.0f, 1.0f ); 
		glVertex3f( +0.5,  0.0, 0 );
 		glVertex3f( +0.0, -0.3, 0 );
 		glVertex3f( +0.0, +0.3, 0 );
		glVertex3f( -0.5,  0.0, 0 );
 		glVertex3f( +0.0, -0.3, 0 );
 		glVertex3f( +0.0, +0.3, 0 );
		glVertex3f( -0.5,  0.0, 0 );
 		glVertex3f( -1.0, -0.3, 0 );
 		glVertex3f( -1.0, +0.3, 0 );
	glEnd();
	glEndList();


	windSpeed  .set( -10.0, 0.0 );
	watterSpeed.set(  0.0, 0.0 );

	printf( " >>> Setup  yacht1: \n" );
	yacht1.from_mass_points( 2, mass, (Vec2d*)poss );  printf( " I invI  %f %f \n", yacht1.I, yacht1.invI );
	yacht1.setDefaults();
	yacht1.setAngle( M_PI*0.6 );
	yacht1.pos.set( {0.0, 0.0} );
	yacht1.omega = 0.0;

	int shape = glGenLists(1);
	glNewList( shape , GL_COMPILE );
	glBegin   (GL_TRIANGLE_FAN);	       
		glNormal3f( 0.0f, 0.0f, 1.0f ); 
		glVertex3f( +1.5,  0.0, 0 );
 		glVertex3f( +0.5,  0.2, 0 );
		glVertex3f( -1.0,  0.2, 0 );
 		glVertex3f( -1.0, -0.2, 0 );
		glVertex3f( +0.5, -0.2, 0 );
		glVertex3f( +1.5,  0.0, 0 );
	glEnd();
	glEndList();

	yacht1.shape = shape;

	yacht1.keel  .pos.set ( 0.0, 0.0 );
	yacht1.keel  .setAngle( M_PI/2 );
	yacht1.keel  .area   = 0.05;
	yacht1.keel  .CD0    = 0.04;  
	yacht1.keel  .dCD    = 0.9;  
	yacht1.keel  .dCDS   = 0.9;  
	yacht1.keel  .dCL    = 2.00;
	yacht1.keel  .dCLS   = 2.00;
	yacht1.keel  .sStall = 0.20;
	yacht1.keel  .wStall = 0.40;

	yacht1.rudder.pos.set ( -1.1, 0.0 );
	yacht1.rudder.setAngle(  M_PI/2  );
	yacht1.rudder.area = 0.01;
	yacht1.rudder.CD0    = 0.008;  
	yacht1.rudder.dCD    = 1.5;  
	yacht1.rudder.dCDS   = 0.9;  
	yacht1.rudder.dCL    = 6.28;
	yacht1.rudder.dCLS   = 2.70;
	yacht1.rudder.sStall = 0.16;
	yacht1.rudder.wStall = 0.08;



	yacht1.mast.pos.set ( +0.05, 0.0 );
	yacht1.mast.setAngle( M_PI*0.0 );
	yacht1.mast.area = 3.0;
	yacht1.mast.CD0    = 0.1;  
	yacht1.mast.dCD    = -1.0;  
	yacht1.mast.dCDS   = 0.8;  
	yacht1.mast.dCL    = 3.50;
	yacht1.mast.dCLS   = 2.20;
	yacht1.mast.sStall = 0.20;
	yacht1.mast.wStall = 0.40;

	printf( " >>> Setup  yacht1 DONE \n" );


/*
	for (int i=0; i<nryb; i++){
		ryby[i].set( randf( )*200.0-100.0, randf()*200.0-100.0 );
	}
*/
	for (int i=0; i<nryb; i++){
		gen_in_circle( rybMaxDist, ryby[i] );
		ryby[i].add( yacht1.pos ) ;
	}

	chain1 = new Chain2D( 100, yacht1.pos, {0.3, 0.3}  );	

	printf( " >>> Setup  chain1 DONE \n" );
		
}

void inputHanding(){
	while(SDL_PollEvent(&event)){ // be carefull to include all event handling instide the while loop !!!
		if( event.type == SDL_KEYDOWN ){ 
			switch( event.key.keysym.sym ){
				case SDLK_ESCAPE:   escape(); break;
				case SDLK_SPACE:    STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); break;
				case SDLK_KP_MINUS: thisScreen->zoom*=VIEW_ZOOM_STEP; break;
				case SDLK_KP_PLUS:  thisScreen->zoom/=VIEW_ZOOM_STEP; break;
			}

/*
			if( event.key.keysym.sym == SDLK_ESCAPE   ) { escape(); }
			if( event.key.keysym.sym == SDLK_SPACE    ) { STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); }
			if( event.key.keysym.sym == SDLK_KP_MINUS ) { thisScreen->zoom*=VIEW_ZOOM_STEP; }
			if( event.key.keysym.sym == SDLK_KP_PLUS  ) { thisScreen->zoom/=VIEW_ZOOM_STEP; }
*/
		} 
		if( event.type == SDL_QUIT){ quit();  };

	} // while(SDL_PollEvent(&event))


	const Uint8 *keys = SDL_GetKeyboardState(NULL);
/*
	if( keys[ SDL_SCANCODE_UP   ] ){  yacht1.mast.pos.x += 0.01;   }
	if( keys[ SDL_SCANCODE_DOWN ] ){  yacht1.mast.pos.x -= 0.01;  }

	if( keys[ SDL_SCANCODE_LEFT  ] ){  yacht1.mast.setAngle( yacht1.mast.phi + 0.01 );  }
	if( keys[ SDL_SCANCODE_RIGHT ] ){  yacht1.mast.setAngle( yacht1.mast.phi - 0.01 );  }
*/

	if( keys[ SDL_SCANCODE_KP_8 ] ){  yacht1.mast.pos.x += 0.01;   }
	if( keys[ SDL_SCANCODE_KP_2 ] ){  yacht1.mast.pos.x -= 0.01;  }
	if( keys[ SDL_SCANCODE_KP_4 ] ){  yacht1.mast.setAngle( yacht1.mast.phi + 0.01 );  }
	if( keys[ SDL_SCANCODE_KP_6 ] ){  yacht1.mast.setAngle( yacht1.mast.phi - 0.01 );  }
	if( keys[ SDL_SCANCODE_KP_1 ] ){  yacht1.rudder.setAngle( yacht1.rudder.phi + 0.01 );   }
	if( keys[ SDL_SCANCODE_KP_3 ] ){  yacht1.rudder.setAngle( yacht1.rudder.phi - 0.01 );  }


	SDL_GetMouseState( &thisScreen->mouseX, &thisScreen->mouseY );
	//printf( "frame %i mouseX moyseY  %i %i   \n", frameCount, mouseX, mouseY );
}


void loop(int n ){
	loopEnd = false;
	for( int iframe=0; iframe<n; iframe++ ){
		inputHanding(); 
		if(!STOP){
			update();
			thisScreen->update();
		}  
		//printf(" %i \n", iframe );
		SDL_Delay( 10 );
		frameCount++;
		if(loopEnd) break;
	} 
}

// FUNCTION ======  main
int main(int argc, char *argv[]){

	// creating windows
	SDL_Init(SDL_INIT_VIDEO);  
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1); 
	int sid;
	thisScreen  = new Screen2D( sid, 800,600); 

	setup();

	printf( " setup done \n" );

	//loop( 1 );
	loop( 1000000 );

	return 0;
}


// ==========================================================
// ===== Export this functions to Dynamic library for Python
// ==========================================================

extern "C"{
}


