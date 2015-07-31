
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "include/fastmath.h"
#include "include/Vec3.h"
#include "include/Mat3.h"
#include "include/quaternion.h"
#include "include/raytrace.h"


#include "include/drawMath.h"
#include "include/drawUtils.h"
#include "include/trackball.h"
#include "include/ListKeeper.h"
#include "include/molecule.h"

// ===============================
// ===== GLOBAL CONSTAMNTS
// ===============================

const int    WIDTH        = 800;
const int    HEIGHT       = 600;
const int    VIEW_DEPTH   = 1000;
const float  ASPECT_RATIO = WIDTH/(float)HEIGHT ;

const float VIEW_ZOOM_STEP    = 1.2f;
const float VIEW_ZOOM_DEFAULT = 5.0f;

#define COLDEPTH 16

// ===============================
// ===== GLOBAL VARIABLES
// ===============================

bool isLib = true;

SDL_Window* displayWindow;
SDL_Renderer* displayRenderer;
SDL_RendererInfo displayRendererInfo;
SDL_Event		event; 

bool  STOP          	= false; 
bool  loopEnd           = false;
int   frameCount		=	0;
int   mouseX=0,mouseY=0;
float zoom = VIEW_ZOOM_DEFAULT;

float qCamera[4];
float qCameraOld[4]; 

float mouse_spinning = 0;
float mouse_begin_x  = 0;
float mouse_begin_y  = 0;


ListKeeper * lists;

Molecule * molecule;

double sphere_R;
Vec3d sphere_pos;

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

void camera(){
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	glOrtho      ( -zoom*ASPECT_RATIO, zoom*ASPECT_RATIO, -zoom, zoom, -VIEW_DEPTH, +VIEW_DEPTH );


	glMatrixMode (GL_MODELVIEW);
	float camMatrix[4][4];
	build_rotmatrix (camMatrix, qCamera );
	glLoadMatrixf(&camMatrix[0][0]);


/*
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( -1.5f+(mouseX*0.005-0.5), 0.0f-(mouseY*0.005-0.5), -6.0f );
*/
}

void initRenderer(){
 	SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
    SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
    if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        printf( " WARNING : NO ACCELEARION !!! \n" ); 
    }
}

void setupRenderer(){
	float ambient  [] = { 0.1f, 0.15f, 0.25f, 1.0f };
	float diffuse  [] = { 0.9f, 0.8f,  0.7f,  1.0f };
	float specular [] = { 1.0f, 1.0f,  1.0f,  1.0f };
	float shininess[] = { 80.0f                    };
	//float lightPos [] = { 1.0f, 10.0f, +10.0f, 10.0f  };
	float lightPos [] = { 1.0f, 1.0f, +1.0f, 0.0f  };
	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
	glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glEnable  (GL_COLOR_MATERIAL);
	glLightfv (GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable  (GL_LIGHTING);
	glEnable  (GL_LIGHT0);
	glEnable  (GL_NORMALIZE);
	glEnable  (GL_DEPTH_TEST);
	// glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_BLEND) ;
	// glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);
	glHint       ( GL_LINE_SMOOTH_HINT, GL_NICEST );	
	glShadeModel ( GL_SMOOTH );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );

/*
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
*/

}

void setup(){
	initRenderer();
	setupRenderer();
	trackball (qCamera, 0, 0, 0, 0); // qCameraOld[3] = 1.0;

	lists = new ListKeeper( 16 );

	int ifree,igl,nvert,ndiv;

/*
	ifree = lists->getFree();
	igl=glGenLists(1);
	glNewList( igl , GL_COMPILE);
		//drawLines( nlinks, links, (Vec3d*) points );
		drawBox( -0.5, +0.5,   -0.5, +0.5,   -0.5, +0.5,   0.5, 0.8, 0.5 );
	glEndList();
	lists->data[ ifree ] = igl;
*/



	// make spehre
	ndiv = 10;
	sphere_R = 0.5;
	sphere_pos.set(0.5,0.5,0.5);

	//Vec3d pos; pos.set(0.0,0.0,0.0);
	ifree = lists->getFree();
	igl=glGenLists(1);
	glNewList( igl , GL_COMPILE);
		glShadeModel ( GL_SMOOTH );
		nvert = drawSphere_oct( ndiv, sphere_R, sphere_pos );
	glEndList();
	lists->data[ ifree ] = igl;
	printf( " sphere %i nvert %i \n", ndiv, nvert );


/*
	// ray sphere
	ifree = lists->getFree();
	igl=glGenLists(1);
	glNewList( igl , GL_COMPILE);
		Vec3d pt,normal, ray0, hRay;
		ray0.set(  5.0, 0.0, 0.0 );
		hRay.set( -5.0, 1.0, 0.5 ); hRay.normalize();
		pt.set( ray0 ); pt.add_mul( hRay, 100 );
		glColor3f( 1.0, 0.5, 0.0 );
		drawLine( ray0, pt );
		double t = raySphere   ( ray0, hRay, sphere_R, sphere_pos );
		if( t<INFINITY ){
			sphereNormal( t, ray0, hRay, sphere_pos, normal );
			pt.set( ray0 ); pt.add_mul( hRay, t );
			glColor3f( 0.0, 1.0, 1.0 );
			drawVecInPos( normal, pt );
			//drawVecInPos( hRay,  ray0 );
		};
		printf( " %f   %f %f %f    %f %f %f \n", t, pt.x, pt.y, pt.z, normal.x, normal.y, normal.z );
	glEndList();
	lists->data[ ifree ] = igl;
*/



	//Vec3f tip;   tip.set(3.00,0,0);
	//Vec3f base;  base.set(0,0.0,0.0); 

/*
	Vec3f tip;   tip.set (0.000,0.00,0.00);
	Vec3f base;  base.set(1.0,2.0,3.0); 

	ndiv = 16;
	Vec3d pos; pos.set(0,0,0);
	ifree = lists->getFree();
	igl=glGenLists(1);
	glNewList( igl , GL_COMPILE);
		glShadeModel ( GL_SMOOTH );
		//int nvert = drawConeFan( ndiv, 0.5, base, tip );
		int nvert = drawCylinderStrip( ndiv, 0.5, 1.0, base, tip );
	glEndList();
	lists->data[ ifree ] = igl;
	printf( " sphere %i nvert %i \n", ndiv, nvert );
*/


	colors= &colors_def[0];
	radii = &radii_def [0];

}

void draw(){
	glEnable (GL_LIGHTING);
	glShadeModel(GL_FLAT);
	//drawBox( -0.5, +0.5,   -0.5, +0.5,   -0.5, +0.5,   0.8, 0.8, 0.8 );

	glColor3f( 0.8f, 0.8f, 0.8f );
	for( int i=0; i<lists->n; i++ ){
		int listi = lists->data[i];  
		if( listi > 0 ){ glCallList(listi); };
	}

	if( molecule != NULL ) if( molecule->viewlist>0 ){
		//printf( "molecule rendered \n" );
		glCallList(molecule->viewlist);
		
	}

	glDisable  (GL_LIGHTING);
	drawAxis( 10 );

/*
	for( int i=0; i<lists->n; i++ ){
		int listi = lists->data[i];  
		if( listi > 0 ){ glCallList(listi); };
	}
*/

    SDL_RenderPresent(displayRenderer);
}

void mouse_camera (int x, int y){
	if (mouse_spinning){
		trackball ( 
			qCameraOld,
			(2.0f*mouse_begin_x-WIDTH)/WIDTH,  (HEIGHT-2.0f*mouse_begin_y)/HEIGHT, 
			(2.0f*x-WIDTH            )/WIDTH,  (HEIGHT-2.0f*y            )/HEIGHT 
		);
		add_quats ( qCameraOld, qCamera, qCamera );
		mouse_begin_x = x; mouse_begin_y = y; 
	}
}

void mouse_picking(){
	glDisable  (GL_LIGHTING);
	Vec3d mouse_p, a, b, c;
	
	float mat[4][4];
	glGetFloatv (GL_MODELVIEW_MATRIX, &mat[0][0]);
	a.set( mat[0][0], mat[1][0], mat[2][0] );
	b.set( mat[0][1], mat[1][1], mat[2][1] );
	c.set( mat[0][2], mat[1][2], mat[2][2] );

	c.mul( -1 ); // for some reason it is inverted

/*
	Vec3d o0; p0.set(0,0,0);
	glColor3f(1,0,0); drawLine( p0, a );
	glColor3f(0,1,0); drawLine( p0, b );
	glColor3f(0,0,1); drawLine( p0, c );
*/
	float mouseA = 2*zoom*( mouseX/float(HEIGHT) - 0.5*ASPECT_RATIO );
	float mouseB = 2*zoom*( 0.5 -mouseY/float(HEIGHT) );
	mouse_p.set_lincomb( mouseA,a,  mouseB,b );   
	//drawLine( p0, mouse_p );
/*
	double t;
	double r2 = rayPointDistance2( mouse_p, c, sphere_pos, t );
	printf( " %f %f %f \n", t, sqrt(r2), sphere_R );
	if( r2<(sphere_R*sphere_R) ) printf( " HIT! \n" );
*/
	int iatom = molecule->rayPickAtom_sphere( mouse_p, c );
	if( iatom>0 ){ 
		printf( "picked atom %i \n", iatom  ); 
		molecule->drawAtom( iatom, 5, 1.2, 0xFFFF00FF );
	}else{ 
		printf( "no atom picked \n" );  
	}
}

void inputHanding(){
	while(SDL_PollEvent(&event)){ // be carefull to include all event handling instide the while loop !!!
		if( event.type == SDL_KEYDOWN ){ 
			if(event.key.keysym.sym == SDLK_ESCAPE) { escape(); }
			if(event.key.keysym.sym == SDLK_SPACE    ){ STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); }
			if(event.key.keysym.sym == SDLK_KP_MINUS ) { zoom*=VIEW_ZOOM_STEP; }
			if(event.key.keysym.sym == SDLK_KP_PLUS  ) { zoom/=VIEW_ZOOM_STEP; }
		} 
		if( event.type == SDL_QUIT){ quit();  };
		if( event.type == SDL_MOUSEMOTION ){
				mouse_camera( event.motion.x, event.motion.y );
		}
		if( event.type == SDL_MOUSEBUTTONDOWN )    {
			if( event.button.button == SDL_BUTTON_LEFT ) {
				mouse_spinning = 1;
				mouse_begin_x  = event.button.x;
				mouse_begin_y  = event.button.y;
				//printf("SDL_MOUSEBUTTONDOWN\n");

				mouse_picking();
			}
		} 
		if( event.type == SDL_MOUSEBUTTONUP ){
			if( event.button.button == SDL_BUTTON_LEFT ){ mouse_spinning = 0; }
		}
	} // while(SDL_PollEvent(&event))
	SDL_GetMouseState( &mouseX, &mouseY );
	//printf( "frame %i mouseX moyseY  %i %i   \n", frameCount, mouseX, mouseY );
}

void loop(int n ){
	loopEnd = false;
	for( int i=0; i<n; i++ ){
		camera();
    	glClearColor( 0.5f, 0.5f, 0.5f, 0.0f );
    	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		inputHanding(); 
		if(!STOP) draw();  
		SDL_Delay( 10 );
		frameCount++;
		if(loopEnd) break;
	} 
}

// FUNCTION ======  main
int main(int argc, char *argv[]){

	isLib = false;
	//printf( " %f ", PHYS_dt );
	setup();

	molecule = new Molecule( );
	molecule->loadFromFile_bas( "answer.bas" );
	molecule->center_minmax();
	molecule->findBonds( 0.5 );
	//molecule->makeViewCPK( 1, 3, 0.1, 0.05 );
	molecule->makeViewCPK( 3, 4, 0.1, 0.05 );
	//molecule->makeViewCPK( 5, 4, 1.0, 0.05 );


	loop( 1000000 );
	return 0;
}


// ==========================================================
// ===== Export this functions to Dynamic library for Python
// ==========================================================


extern "C"{

void _setup(       ){ setup(  );  }
void _quit (       ){ quit (  );  }
void _loop ( int n ){ loop ( n ); }

bool erase( int i ){
	int id = lists->data[i];
	if( id > 0 ){ 
		glDeleteLists(id, 1);
		return true; 
	}
	return false;	
};

int putBox( double * from, double * to, double * color  ){
	int ifree = lists->getFree();
	int id=glGenLists(1);
	glNewList( id , GL_COMPILE);
		drawBox( (float)from[0], (float)to[0], (float)from[1], (float)to[1], (float)from[2], (float)to[2], (float)color[0], (float)color[1], (float)color[2] );
	glEndList();
	lists->data[ ifree ] = id;
	return ifree;
}

int putLines( int nlinks, int * links, double * points_ ){
	Vec3d * points = (Vec3d*) points_;
	int ifree = lists->getFree();
	int id=glGenLists(1);
	glNewList( id , GL_COMPILE);
		drawLines( nlinks, links, (Vec3d*) points );
	glEndList();
	lists->data[ ifree ] = id;
	return ifree;
}

void setAtomTypes( int ntypes_, double * radii_, Uint32 * colors_ ){
	ntypes = ntypes_;
	radii  = radii_;
	colors = colors_;
/*
	for (int i=0; i<ntypes; i++ ){
		printf( " %f   %i %i %i %i   %i \n", radii[i], colors[i] & 0xFF, (colors[i] >> 8)&0xFF, (colors[i] >> 16) & 0xFF, colors[i] >> 24,colors[i] );
	}
*/
}

void addMolecule( int natoms, int * types, double * xyzs ){
	molecule = new Molecule( natoms, types, (Vec3d*) xyzs );
//	printf( " natoms %i \n", molecule->natoms );
/*
	for (int i=0; i<molecule->natoms; i++ ){
		printf( " %i   %f %f %f \n", molecule->types[i], molecule->xyzs[i].x, molecule->xyzs[i].y, molecule->xyzs[i].z  );
	}
*/
	molecule->findBonds( 0.5 );
	//printf( " findBonds DONE \n"  );
	molecule->makeViewCPK( 1, 3, 0.1, 0.05 );

	//molecule->makeViewCPK( 8, 8, 0.25, 0.15 );
	//printf( " makeViewCPK DONE \n"  );
}

}


