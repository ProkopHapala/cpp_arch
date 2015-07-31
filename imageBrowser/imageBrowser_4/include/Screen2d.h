
class Screen2d{
	public:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int updated;

	SDL_Window  * window  = NULL;
	SDL_Surface * surface = NULL;

	Screen2d( int W, int H){
		SCREEN_WIDTH  = W;
		SCREEN_HEIGHT = H;
		updated = false;
	}

	bool init(){
		bool success = true;
		window = SDL_CreateWindow( "viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}else{
			surface = SDL_GetWindowSurface( window );
		}
		return success;
	}

	void force_update(){
		SDL_UpdateWindowSurface( window );
		updated = true;
	}

	void update(){
		if( !updated ){
			force_update();
		}
	}

	void close(){
		SDL_DestroyWindow( window );
	}

};
