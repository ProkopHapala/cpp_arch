
#include <forward_list>


class Point2D{
	public:
	double x, y;
	bool equals( const Point2D& p ){ return ( x == p.x ) && ( y == p.y ); };
};

// ========== GridMap



template <class TYPE1 > class GridMap2D_dynamic {
	public:
	double step, invStep;
	int nx, ny, nxy;
	bool isStatic = false;
	
	std::forward_list<TYPE1*>** store;
	TYPE1*** 					store_static;
	int*						ns;
	
	void init( int nx_, int ny_, double step_ ){
		step    = step_; 
		invStep = 1/step;
		nx = nx_; ny=ny_;
		nxy = nx*ny;
		ns     = new int[ nxy ];
		store  = new std::forward_list<TYPE1*>*[ nxy ];
		for (int i=0; i<nxy; i++){ store[i] = new std::forward_list<TYPE1*>(); ns[i] = 0; }
	};

	inline void boxIndex( TYPE1* p, int& ix, int& iy ){	
		ix = (int)( invStep * p->x );
		iy = (int)( invStep * p->y );
	}

	inline int getIndex( int ix, int iy ){ return nx*iy + ix; }
	inline int getIndex( TYPE1* p ){ int ix,iy; boxIndex( p, ix, iy ); return getIndex( ix, iy ); }

	void makeStatic( ){
		store_static = new TYPE1**[nxy];
		for (int i=0; i<nxy; i++){ 
			int ni = ns[i];
			if ( ni>0 ){ 
				//printf( " ni %i \n", ni );
				TYPE1** arr = new TYPE1*[ ni ];
				store_static[ i ] = arr;
				auto list = store [ i ];
				int j = 0;
				for ( auto it = list->cbegin(); it != list->cend(); ++it){
					arr[ j ] = *it;
					j++;
				}  
			}
		}
		isStatic = true;
		// dealocation
		for (int i=0; i<nxy; i++){ delete store[i]; }
		delete [] store;
	}

	int insert(	TYPE1* p ){
		int i = getIndex( p );
		//printf( "i %i \n", i );
		store  [ i ] -> push_front( p );
		ns     [ i ] ++;
	};

// ======= just testing

	void paintGridLines(){
		glBegin( GL_LINES );
		glColor3f ( 0.1,0.1,0.1 );
		for (int i=0; i<=nx; i++){ glVertex3f( (float)(i*step), 0 , 0 ); glVertex3f( (float)(i*step),  (float)(ny*step), 0 ); };
		for (int i=0; i<=ny; i++){ glVertex3f( 0, (float)(i*step) , 0 ); glVertex3f( (float)(nx*step), (float)(i*step), 0 ); };
		glEnd( );
	}

	void paintStorage(){
		glBegin( GL_POINTS );
		//glEnable( GL_PROGRAM_POINT_SIZE );
		//glPointSize( 5.0f);
		for (int i=0; i<nxy; i++){
			glColor3f ( randFuncf( i ) , randFuncf( i+ 16874 ), randFuncf( i+ 98774 )  ); 
			if( isStatic ){
				int ni = ns[i];
				for ( int j = 0; j < ni; j++ ){
					TYPE1* p = store_static[ i ][ j ];
					glVertex3f( (float)(p->x) , (float)(p->y), 0 );
					//printf( " i,px,py: %i %f %f \n", i ,p->x,p->y );
				}
			} else {
				auto list = store[i];
				for ( auto it = list->cbegin(); it != list->cend(); ++it ){
					TYPE1* p = *it;
					glVertex3f( (float)(p->x) , (float)(p->y), 0 );
					//printf( " i,px,py: %i %f %f \n", i ,p->x,p->y );
				}
			}
		};
		glEnd( );
	}

};




/*
template <class TYPE1 > class GridMapStatic{
	public:
	double step, invStep;
	int nx, ny, nxy;

	TYPE1*** store;
	int* ns;
	int* filled;
	
	void init(	int nx, int ny, double step, int n0 ){
		dynamic = true;
		invStep = 1/step;
		nxy = nx*ny;
		stored = new TYPE1**[ nxy ];
		int filled; 
		for (int i=0; i<nxy; i++){ 
			store  [i] = new TYPE1*[ n0 ]; for( int j = 0; j<n0; j++ ){ store[i][j] = NULL; };
			ns     [i] = n0;
			filled [i] = 0;
		}
	}

	inline getIndex( int ix, int iy ){ return nx*iy + ix; }

	inline void boxIndex( const TYPE1* p, int& ix, int& iy ){	
		ix = (int)( invStep * p->x	);
		iy = (int)( invStep * p->y	);
	}

	void remove( int i, int i0 ){
		
	};

	int insert(	TYPE1* p ){
		getIndex( );
		
	};

};

*/



