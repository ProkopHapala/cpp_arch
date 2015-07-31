
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

#include "include/Screen2d.h"
#include "include/browser.h"


int ierr;
TTF_Font* font;

SDL_Event		 event; 

Browser  * browser;
Screen2d * screen;
char* startDir;


void quit(){
	browser->close();
	screen->close();
	TTF_CloseFont( font );
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(1);
}

bool init(){
	if( TTF_Init() < 0 ){
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}
	//font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 11 );
	font = TTF_OpenFont("source-code-pro.ttf", 11 );
	if( !font ){
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		return false;
	}
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}else{	
		screen  = new Screen2d( 1200,900 );
		if( ! screen->init() ) return false;
	}


	browser = new Browser( startDir );
	browser->screen = screen;
	browser->font = font;
	browser->init();
	browser->setAndReadDir( browser->work_dir );
	browser->setJob_thumbs( );

	return true;
}

void update(){ 
	browser->update(); 
	screen->update(); 
};

void inputHanding(){
	while(SDL_PollEvent(&event)){ // be carefull to include all event handling instide the while loop !!!
		if( event.type == SDL_KEYDOWN ){ 
			if( event.key.keysym.sym == SDLK_ESCAPE   ) { quit(); }
			if( event.key.keysym.sym == SDLK_UP     ) { browser->moveRow(-1); }
			if( event.key.keysym.sym == SDLK_DOWN   ) { browser->moveRow(+1); }
			if( event.key.keysym.sym == SDLK_LEFT   ) { browser->moveCol(-1); }
			if( event.key.keysym.sym == SDLK_RIGHT  ) { browser->moveCol(+1); }
			if( event.key.keysym.sym == SDLK_RETURN ) { browser->enter();     }
			//if( event.key.keysym.sym == SDLK_SPACE    ) { STOP = !STOP; printf( STOP ? " STOPED\n" : " UNSTOPED\n"); }
			//if( event.key.keysym.sym == SDLK_KP_MINUS ) { thisScreen->zoom*=VIEW_ZOOM_STEP; }
			//if( event.key.keysym.sym == SDLK_KP_PLUS  ) { thisScreen->zoom/=VIEW_ZOOM_STEP; }
			//if( event.key.keysym.sym == SDLK_KP_PLUS  ) { thisScreen->zoom/=VIEW_ZOOM_STEP; }
		} 
		if( event.type == SDL_QUIT){ quit();  };
	} 
}

void loop(int n ){
	for( int iframe=0; iframe<n; iframe++ ){
		inputHanding(); 
		update();
		SDL_Delay( 10 );
	} 
}

int main( int argc, char* args[] ){
	if( argc>1 ){ startDir = args[1]; }else{  startDir = (char *)"/home";  }
	printf( " starting in directory %s \n", startDir );
	if(    init()    ){  
		loop( 1000000 );
	}else{    
		printf( "Failed to initialize!\n" );	
	}
	quit();
	return 0;
}
