/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include <lua.hpp>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool quit       = false;
bool renderText = false;

lua_State *L;

std::string inputText;
SDL_Color textColor;

//============== FUNCTIONS


bool init();
bool loadMedia();
void close();
SDL_Window*    gWindow   = NULL;
TTF_Font*      gFont     = NULL;
SDL_Renderer*  gRenderer;

#include "include/Texture.h"

LTexture gPromptTextTexture;
LTexture gInputTextTexture;



void changeColor( int c ){
	printf( " changeColor : %i \n", c );
	SDL_SetRenderDrawColor( gRenderer, c, c, c, 0xFF );
}

extern "C" {

static int lua_changeColor( lua_State *L ) {
	double d = lua_tonumber(L, 1);
	changeColor( (int)d );
	return 0;                    
}

}



void draw(){
	//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
	gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );
	SDL_RenderPresent( gRenderer );
}


void inputHandling(){
	renderText = false;
	SDL_Event e;
	//SDL_StartTextInput();
	while( SDL_PollEvent( &e ) != 0 )				{
		if( e.type == SDL_QUIT )					{
			quit = true;
		}else if( e.type == SDL_KEYDOWN ){
			if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )	{
				inputText.pop_back();
				renderText = true;
			}else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )	{
				SDL_SetClipboardText( inputText.c_str() );
			}else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )	{
				inputText = SDL_GetClipboardText();
				renderText = true;
			}else if( (e.key.keysym.sym == SDLK_RETURN) || ( e.key.keysym.sym == SDLK_KP_ENTER) ){
				printf( " lua do : %s \n", inputText.c_str() );
				luaL_dostring(L, inputText.c_str() );
				inputText = " ";  
				renderText = true;
			}
		}else if( e.type == SDL_TEXTINPUT )	{
			if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) ){
				inputText += e.text.text;
				renderText = true;
			}
		}
	}
	if( renderText ){
		if( inputText != "" ){ 	gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );  }
		else                 {	gInputTextTexture.loadFromRenderedText( " ", textColor ); 				 }
	}
	//SDL_StopTextInput();
};






bool init(){
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}else{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}else{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 )				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia(){
	bool success = true;
	//gFont = TTF_OpenFont( "lazy.ttf", 28 );
	//gFont = TTF_OpenFont( "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 12 );
	gFont = TTF_OpenFont( "04B_03__.TTF", 8 );    // great case sensitive !!! 
	//gFont = TTF_OpenFont( "DroidSans.ttf", 10 );  // very nice
	//gFont = TTF_OpenFont( "DroidSansMono.ttf", 10 );
	//gFont = TTF_OpenFont( "FRUCS___.TTF", 8 );
	//gFont = TTF_OpenFont( "slkscr.ttf", 8 );  // trochu moc siroke
	//gFont = TTF_OpenFont( "MARKEN__.TTF", 8 );
	if( gFont == NULL )	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}	else	{
		SDL_Color textColor = { 0, 0, 0, 0xFF };
		if( !gPromptTextTexture.loadFromRenderedText( "clr(128) change color to gray:", textColor ) ) {
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}
	return success;
}

void close(){
	gPromptTextTexture.free();
	gInputTextTexture.free();
	TTF_CloseFont( gFont );
	gFont = NULL;
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	lua_close (L);
}

int main( int argc, char* args[] ){

	L = luaL_newstate();   
	luaL_openlibs(L);               

	lua_pushcfunction(L, lua_changeColor );
	lua_setglobal(L, "clr");         
		              
	//luaL_dostring(L, "print \" Hello Lua ! \"");        
	//luaL_dostring(L, "print(  10*10)");         
	//luaL_dostring(L, "print( mysin(10) )");         
	//luaL_dostring(L, "printsin(10)");          

	if( !init() )	{
		printf( "Failed to initialize!\n" );
	}	else	{
		if( !loadMedia() )		{
			printf( "Failed to load media!\n" );
		}else{	
			textColor = { 0, 0, 0, 0xFF };
			inputText = "Some Text";
			gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
			SDL_StartTextInput();
			while( !quit )			{
				inputHandling();
				draw();
			}
			SDL_Delay(20);
			SDL_StopTextInput();
		}
	}

	close();
	return 0;
}
