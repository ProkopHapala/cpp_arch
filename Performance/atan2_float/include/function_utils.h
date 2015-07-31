

typedef float (*Func1d)( float x );

void plotFunc( int n, float x1, float x2, float fscale, Func1d func ){
	float dx = (x2-x1)/n;
	float ox = x1;
	float oy = func(ox); 
	for ( int i=0; i<n; i++ ){
		float x = i*dx + x1;
		float y = func(x); 
		SDL_RenderDrawLine    ( render,  x2i(ox), y2i(-oy*fscale), x2i(x), y2i(-y*fscale) );
		ox=x; oy=y;
	}
}

void funcNorm( int n, float x1, float x2, Func1d func1, float& maxAbs, float& sumAbs, float& sumSq ){
	float dx = (x2-x1)/n;
	maxAbs = 0;
	sumAbs = 0;
	sumSq  = 0;
	for ( int i=0; i<n; i++ ){
		float x    = i*dx + x1;
		float yabs = fabs( func1(x) ); 
		if( yabs > maxAbs ){ maxAbs = yabs; }
		sumAbs += yabs;
		sumSq  += yabs*yabs;
	}
}




