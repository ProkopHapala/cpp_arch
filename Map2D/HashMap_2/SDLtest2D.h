
#include <SDL/SDL.h>
#include <GL/gl.h>



#include <stdint.h>
//#include <iostream>

using namespace std;

// CPU ticks timer
// http://stackoverflow.com/questions/6432669/variance-in-rdtsc-overhead
inline uint64_t getCPUticks(){
    uint32_t lo, hi;
    __asm__ __volatile__ (
      "xorl %%eax, %%eax\n"
      "cpuid\n"
      "rdtsc\n"
      : "=a" (lo), "=d" (hi)
      :
      : "%ebx", "%ecx" );
    return (uint64_t)hi << 32 | lo;
};



#define COLDEPTH 16
const float  VIEW_DEPTH   = 1000;
const int    WIDTH        = 800; const int  WIDTH_HALF = WIDTH/2;
const int    HEIGHT       = 800; const int  HEIGHT_HALF = HEIGHT/2;
const float  ASPECT_RATIO = WIDTH/(float)HEIGHT ;
const float  VIEW_ZOOM_STEP    = 1.2f;
const float  VIEW_ZOOM_DEFAULT = 1.0f;

bool STOP = false; 

// ===============================
// ===== GLOBAL VARIABLES
// ===============================

SDL_Surface *screen;
SDL_Event event; 

int frameCount = 0;
float zoom;
float pixsz;
int            mouseX,mouseY;

// ================
// ===== FUNCTIONS 
// =================

const  float  INV_RAND_MAX = 1.0f/RAND_MAX;
inline float  randf() { return INV_RAND_MAX*rand(); }
inline float  randf( float fmin, float fmax ){ return (INV_RAND_MAX*rand())*(fmax-fmin) + fmin; }

inline int randFunc( int r ){
	r = 1664525*r ^ 1013904223;
	r = 1664525*r ^ 1013904223;
	return r; 
}

inline void setZoom( float zoom_ ){ zoom = zoom_; pixsz = zoom/WIDTH_HALF; }

inline float randFuncf( int r ){ return 0.5f + randFunc( r )*4.6566129e-10; }

void quit(){SDL_Quit(); exit(1);};

void colorHash( int id ){ glColor3f( randFuncf( id + 5987 ), randFuncf( id+ 16874 ), randFuncf( id+ 98774 )  ); }

void draw_Vec2d( const Vec2d& p ){ 
	glBegin( GL_POINTS );
		glVertex3f( (float) p.x, (float) p.y, 0 );
	glEnd();
};

void draw_Segment2d( const Vec2d& a, const Vec2d& b ){
	glBegin( GL_LINES );
		glVertex3f( (float) a.x, (float) a.y, 0 );
		glVertex3f( (float) b.x, (float) b.y, 0 );
	glEnd();
};
void draw_Segment2d( Segment2d* s ){ draw_Segment2d( *(s->a), *(s->b) ); };

void draw_Triangle2d( const Vec2d& a, const Vec2d& b, const Vec2d& c ){ 
	glBegin( GL_LINE_LOOP );
		glVertex3f( (float) a.x, (float) a.y, 0 );
		glVertex3f( (float) b.x, (float) b.y, 0 );
		glVertex3f( (float) c.x, (float) c.y, 0 );
	glEnd();
};
void draw_Triangle2d( Triangle2d* t ){ draw_Triangle2d( *(t->a), *(t->b), *(t->c) ); };

void paint_Triangle2d( const Vec2d& a, const Vec2d& b, const Vec2d& c ){ 
	glBegin( GL_LINE_LOOP );
		glVertex3f( (float) a.x, (float) a.y, 0 );
		glVertex3f( (float) b.x, (float) b.y, 0 );
		glVertex3f( (float) c.x, (float) c.y, 0 );
	glEnd();
};
void paint_Triangle2d( Triangle2d* t ){ paint_Triangle2d( *(t->a), *(t->b), *(t->c) ); };

void draw_Rect2d( const Vec2d& a, const Vec2d& b ){ 
	glBegin( GL_LINE_LOOP );
		glVertex3f( (float) a.x, (float) a.y, 0 );
		glVertex3f( (float) a.x, (float) b.y, 0 );
		glVertex3f( (float) b.x, (float) b.y, 0 );
		glVertex3f( (float) b.x, (float) a.y, 0 );
	glEnd();
};
void draw_Rect2d( Rect2d& rc ){ draw_Rect2d( rc.a, rc.b );  } 

void paint_Rect2d( const Vec2d& a, const Vec2d& b ){ 
	glBegin( GL_POLYGON );
		glVertex3f( (float) a.x, (float) a.y, 0 );
		glVertex3f( (float) a.x, (float) b.y, 0 );
		glVertex3f( (float) b.x, (float) b.y, 0 );
		glVertex3f( (float) b.x, (float) a.y, 0 );
	glEnd();
};
void paint_Rect2d( Rect2d& rc ){ paint_Rect2d( rc.a, rc.b );  } 
void paint_Vec2d ( const Vec2d& p, float sz ){ paint_Rect2d( { p.x - sz, p.y - sz }, {p.x + sz, p.y + sz } ); };

void draw_Axis( ){ 
	glBegin( GL_LINES );
		glColor3f( 1.0f , 0.0f, 0.0f ); glVertex3f( 0.0f, 0.0f, 0 ); glVertex3f( 1.0f, 0.0f, 0 );
		glColor3f( 0.0f , 1.0f, 0.0f ); glVertex3f( 0.0f, 0.0f, 0 ); glVertex3f( 0.0f, 1.0f, 0 );
	glEnd();
};


void print( const Vec2d&      p ){ printf( "( %f %f )\n", p.x, p.y ); }; 
void print( Segment2d*  s ){ printf( "( %i ( %f %f )( %f %f ))\n", s->id, s->a->x, s->a->y, s->b->x, s->b->y                   ); }; 
void print( Triangle2d* t ){ printf( "( %i ( %f %f )( %f %f )( %f %f ))\n", t->id, t->a->x, t->a->y, t->b->x, t->b->y, t->c->x, t->c->y ); }; 










// ======	 camera2D

void camera2D( ){
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	glOrtho      ( -zoom*ASPECT_RATIO, zoom*ASPECT_RATIO, -zoom, zoom, -VIEW_DEPTH, +VIEW_DEPTH );	
}


void glRect2D( float x1, float y1, float x2, float y2 ){
	glBegin(GL_POLYGON);
	glVertex3f( x1, y1, 0);
	glVertex3f( x2, y1, 0);
	glVertex3f( x2, y2, 0);
	glVertex3f( x1, y2, 0);
	glEnd();
}

// ======	inputHanding

void inputHanding(){
	while(SDL_PollEvent(&event)){ 
		if( event.type == SDL_KEYDOWN ){ 
			if(event.key.keysym.sym == SDLK_ESCAPE   ){ quit(); }
			if(event.key.keysym.sym == SDLK_KP_PLUS  ){ setZoom( zoom/VIEW_ZOOM_STEP ); printf("zoom: %f \n", zoom); }
			if(event.key.keysym.sym == SDLK_KP_MINUS ){ setZoom( zoom*VIEW_ZOOM_STEP ); printf("zoom: %f \n", zoom); }
			if(event.key.keysym.sym == SDLK_SPACE    ){ STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); }
		} 
		if( event.type == SDL_QUIT){ quit();  };
	}

	SDL_GetMouseState( &mouseX, &mouseY );
}

// ======	setupSDLTest2D()

void setupSDLTest2D(){
	//---- SDL initialization
	if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {        printf("Unable to initialize SDL: %s\n", SDL_GetError()); getchar() ;    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	//screen = SDL_SetVideoMode( WIDTH, HEIGHT, COLDEPTH, SDL_OPENGL |  SDL_SWSURFACE  ); 
	screen = SDL_SetVideoMode( WIDTH, HEIGHT, COLDEPTH, SDL_OPENGL ); 
    if(!screen)  { printf("Couldn't set %dx%d GL video mode: %s\n", WIDTH,HEIGHT, SDL_GetError()); SDL_Quit();  exit(2); }
	SDL_WM_SetCaption(" Prokop's test SDL+ OpenGL", "SDL+ OpenGL");

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	setZoom( VIEW_ZOOM_DEFAULT );
	srand(1234);

}
