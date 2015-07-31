

class Map2D{
	public:
	double step, invStep;
	HashMap<Vec2d*> points;
	int* tempi; 

	Map2D( double step_, int pointpower, int ntemp ){
		step    = step_; 
		invStep = 1/step;
		points.init( pointpower );
		tempi = new int[ ntemp ];
	}

	inline short int getIx( double x ){ return (x * invStep)+16384; };
	inline short int getIy( double y ){ return (y * invStep)+16384; };
	inline double getX ( int ix   ){ return (ix-16384) * step;    };
	inline double getY ( int iy   ){ return (iy-16384) * step;    };
	inline int getBoxIndex( short ix, short iy ){ return iy<<16 ^ ix;  };
	inline int getBoxIndex( const Vec2d& p ){ return getBoxIndex( getIx( p.x ), getIx( p.y ) );  };

	inline void insertPoint( Vec2d& p ){
 		int ibox = getBoxIndex( p );
		points.insert( (Vec2d*)&p, ibox );
	}

	inline int loadPointsIn( Vec2d& p ){
		int ibox = getBoxIndex( p );
		return points.getAllInBox( ibox, tempi );
	};

	inline int loadPointsIn_noHash( Vec2d& p ){
		int ibox = getBoxIndex( p );
		return points.getAllInBox_noHash( ibox, tempi );
	};

};

