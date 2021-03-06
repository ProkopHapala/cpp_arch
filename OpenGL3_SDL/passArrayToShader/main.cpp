#include <stdlib.h>
#include <stdio.h>

//#define GL3_PROTOTYPES 1
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <SDL2/SDL.h>


const float  INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }


#include "include/utils.h"
#include "include/GLObject.h"
#include "include/Shader.h"

Shader   * shader1;
GLObject * object1;
GLObject * object2;

SDL_Window * window     = NULL;
SDL_GLContext   context = NULL;

GLuint vao;     // vertex array object

/*
GLfloat vertexes[4][2] = {
	{  -0.5f,  -0.5f  }, 			 
	{  -0.5f,   0.5f  }, 		
	{   0.5f,  -0.5f  }, 			
	{   0.5f,   0.5f  } }; 		
*/

GLfloat vertexes[4][2] = {
	{  -1.0f,  -1.0f  }, 			 
	{  -1.0f,   1.0f  }, 		
	{   1.0f,  -1.0f  }, 			
	{   1.0f,   1.0f  } }; 	
	
GLfloat colors[4][3] = {
	{  1.0,  0.0,  0.0  }, 			
	{  0.0,  1.0,  0.0  }, 			
	{  0.0,  0.0,  1.0  }, 			
	{  1.0,  1.0,  1.0  } }; 	



GLfloat vertexes2[4][2] = {
	{  -0.9f,  -0.9f  }, 			 
	{  -0.9f,  -0.6f  }, 		
	{  -0.6f,  -0.9f  }, 			
	{  -0.6f,  -0.6f  } }; 		
	
GLfloat colors2[4][3] = {
	{  1.0,  0.0,  0.0  }, 			
	{  0.0,  1.0,  0.0  }, 			
	{  0.0,  0.0,  1.0  }, 			
	{  1.0,  1.0,  1.0  } };


const int nblobs = 5;
float blobs[2*nblobs] = {  100.0,100.0,   200.0,100.0,   100.0,200.0,   200.0,200.0,   250.0,200.0 }; 	



// ============= FUNCTIONS

int frameCount = 0;
bool STOP = false;

void quit();
void die ( char const *msg );
void inputHanding ();
void init();
void draw();
void loop( int niters );
 
void setup(){

	object1 = new GLObject( );
	object1->nVert   = 4;
	object1->vertDim = 2;
	object1->clrDim  = 3;
	object1->vertexes = &vertexes[0][0];
	object1->colors   = &colors[0][0];
	object1->init();

	object2 = new GLObject( );
	object2->nVert   = 4;
	object2->vertDim = 2;
	object2->clrDim  = 3;
	object2->vertexes = &vertexes2[0][0];
	object2->colors   = &colors2[0][0];
	object2->init();

	shader1=new Shader();
	shader1->init( );

	glUseProgram(shader1->shaderprogram);
}


void draw(){
        glClearColor(0.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
		
		GLuint uloc;

/*	
		uloc = glGetUniformLocation( shader1->shaderprogram, "pos0");
		float x = (float)(0.5*sin( frameCount * 0.01 ));
		float y = (float)(0.5*cos( frameCount * 0.01 ));
		glUniform2f( uloc, x, y );
*/


		for( int i=0; i<nblobs; i++ ){
			int ib = i<<1;
			blobs[ ib   ] += 5.0f*( randf() - 0.5f );
			blobs[ ib+1 ] += 5.0f*( randf() - 0.5f );
		}


		uloc = glGetUniformLocation( shader1->shaderprogram, "blobs");
		glUniform2fv(uloc, nblobs, blobs);

		object1->draw();
		//object2->draw();

		//STOP = true;

        SDL_GL_SwapWindow(window);
  
}

// FUNCTION ======	inputHanding
void inputHanding(){
	SDL_Event event; 
	while(SDL_PollEvent(&event)){ 
		if( event.type == SDL_KEYDOWN ){ 
			if(event.key.keysym.sym == SDLK_ESCAPE ) { quit(); }
			if(event.key.keysym.sym == SDLK_SPACE  ) { STOP=!STOP; }
		} 
		if( event.type == SDL_QUIT){ quit();  };
	}
}

void loop( int nframes ){
    for ( int iframe=1; iframe<nframes; iframe++)    {
 		if( !STOP ) draw();
		inputHanding();
		frameCount++;
        SDL_Delay(100);
    }
}

int main(int argc, char *argv[]){
    init();
	setup();
	loop( 100000 );
    quit();
    return 0;
}

void init(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) die( "Unable to initialize SDL" );   
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Opengl 3.2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); // Opengl 3.2
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    window = SDL_CreateWindow("Tutorial2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if ( !window ) die("Unable to create window"); 
    context = SDL_GL_CreateContext( window );
    SDL_GL_SetSwapInterval(1);
	
	// vertex array object
	glGenVertexArrays(1, &vao);  				// Allocate and assign a Vertex Array Object to our handle 
	glBindVertexArray(vao); 					// Bind our Vertex Array Object as the current used object   
}

void quit(){
	glDeleteVertexArrays(1, &vao);
    if( context != NULL ) SDL_GL_DeleteContext( context );
    if( window  != NULL ) SDL_DestroyWindow   ( window  );
    SDL_Quit();
	exit(0);
};
  
void die( char const *msg ){
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}
