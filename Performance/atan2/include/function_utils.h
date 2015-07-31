

typedef double (*Func1d)( double x );

void plotFunc( int n, double x1, double x2, double fscale, Func1d func ){
	double dx = (x2-x1)/n;
	double ox = x1;
	double oy = func(ox); 
	for ( int i=0; i<n; i++ ){
		double x = i*dx + x1;
		double y = func(x); 
		SDL_RenderDrawLine    ( render,  x2i(ox), y2i(-oy*fscale), x2i(x), y2i(-y*fscale) );
		ox=x; oy=y;
	}
}

void funcNorm( int n, double x1, double x2, Func1d func1, double& maxAbs, double& sumAbs, double& sumSq ){
	double dx = (x2-x1)/n;
	maxAbs = 0;
	sumAbs = 0;
	sumSq  = 0;
	for ( int i=0; i<n; i++ ){
		double x    = i*dx + x1;
		double yabs = fabs( func1(x) ); 
		if( yabs > maxAbs ){ maxAbs = yabs; }
		sumAbs += yabs;
		sumSq  += yabs*yabs;
	}
}




