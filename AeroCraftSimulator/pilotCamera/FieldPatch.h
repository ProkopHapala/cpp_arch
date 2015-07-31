

inline double trinagle_area( double x12, double y12, double x13, double y13 ) {  return x12*y13 - x13*y12; };

class Rect{
	public:
	double value;
	union{
		struct{ 
			double x1,y1,z1; 
			double x2,y2,z2; 
			double x3,y3,z3;
			double x4,y4,z4;
		};
		struct{ Vec3d p1,p2,p3,p4; }; // order does matter for default initialized 
	};

	Rect(  double value_, const Vec3d& p1_, const Vec3d& p2_, const Vec3d& p3_, const Vec3d& p4_ ){
		printVec(p1_); printVec(p2_); printVec(p3_); printVec(p4_); printf("ps_\n");
		value = value_;
		p1.set(p1_); p2.set(p2_); p3.set(p3_); p4.set(p4_);
		printVec(p1); printVec(p2); printVec(p3); printVec(p4); printf(" ps \n");
	}

	void draw() const {
		glBegin(GL_QUADS);	     
			glNormal3f( 0, 1, 0 ); 
			glColor3f ( 0.5+z1*0.01, 0.5, 0.2 );	 glVertex3d( x1, z1, y1 ); 
			glColor3f ( 0.5+z2*0.01, 0.5, 0.2 );	 glVertex3d( x2, z2, y2 ); 
			glColor3f ( 0.5+z4*0.01, 0.5, 0.2 );	 glVertex3d( x4, z4, y4 ); 
			glColor3f ( 0.5+z3*0.01, 0.5, 0.2 );	 glVertex3d( x3, z3, y3 ); 
		glEnd();
	};

	inline double area() const {
		double x12 = x1 - x2; double y12 = y1 - y2;
		double x13 = x1 - x3; double y13 = y1 - y3;
		double x42 = x4 - x2; double y42 = y4 - y2;
		double x43 = x4 - x3; double y43 = y4 - y3;
		return abs( trinagle_area( x12, y12, x13, y13 ) ) + abs(trinagle_area( x42, y42, x43, y43 ) );
	};
};

namespace FieldPatch {  
	//double thresh[] = { 0.5, 0.9 };
	double thresh[] = { 0.6 };
	int   startkill = 4;
	double dval      = 0.2;
	double dmax      = 0.3;
	double minarea   = 1000;
	double dheight    =  100; 
	double dmheight   =   50;

	int quadCount;
	
	void divide_1( int nlevels, int level, const Rect& rc   );
	void divide  ( int nlevels, int level, const Rect& rc );

	void divide_1( int nlevels, int level, const Rect& rc ){
		//printVec(rc.p1); printVec(rc.p2); printVec(rc.p3); printVec(rc.p4); printf("\n");
		int dlevel = nlevels - level;
		//rc.paint();
		//double dmax= FIELD_PATCH_dmax;
		//double dval= FIELD_PATCH_dval;
		double fx = 0.5+randf(-dmax,dmax); double mfx = 1-fx;
		double fy = 0.5+randf(-dmax,dmax); double mfy = 1-fy;
		Vec3d top,bot,lft,rgt,ctr;
		top.set( (rc.p1+rc.p2)*0.5 ); 
		bot.set( (rc.p3+rc.p4)*0.5 );
		lft.set( (rc.p1+rc.p3)*0.5 );
		rgt.set( (rc.p2+rc.p4)*0.5 ); 
		ctr.set(  (rc.p1*mfx+rc.p2*fx)*mfy + (rc.p3*mfx+rc.p4*fx)*fy );
		double szscale = ( 4.0/( dlevel + 0.5 ) );
		ctr.z += randf( -dmheight, dheight )*szscale;

//		divide( nlevels, level-1, { rc.value + randf(-dval,dval),   rc.x1, rc.y1,  rc.z1,    top.x,  top.y,  top.z,    lft.x,  lft.y,  lft.z,    ctr.x,  ctr.y,  ctr.z  } );
//		divide( nlevels, level-1, { rc.value + randf(-dval,dval),   top.x, top.y,  top.z,    rc.x2,  rc.y2,  rc.z2,    ctr.x,  ctr.y,  ctr.z,    rgt.x,  rgt.y,  rgt.z  } );
//		divide( nlevels, level-1, { rc.value + randf(-dval,dval),   lft.x, lft.y,  lft.z,    ctr.x,  ctr.y,  ctr.z,    rc.x3,  rc.y3,  rc.z3,    bot.x,  bot.y,  bot.z  } );
//		divide( nlevels, level-1, { rc.value + randf(-dval,dval),   ctr.x, ctr.y,  ctr.z,    rgt.x,  rgt.y,  rgt.z,    bot.x,  bot.y,  bot.z,    rc.x4,  rc.y4,  rc.z4  } );

/*
		divide( nlevels, level-1, { rc.value + randf(-dval,dval),   rc.p1,  top,   lft,    ctr   } );
		divide( nlevels, level-1, { rc.value + randf(-dval,dval),   top,    rc.p2, ctr,    rgt   } );
		divide( nlevels, level-1, { rc.value + randf(-dval,dval),   lft,    ctr,   rc.p3,  bot   } );
		divide( nlevels, level-1, { rc.value + randf(-dval,dval),   ctr,    rgt,   bot,    rc.p4 } );
*/

		divide( nlevels, level-1, Rect( rc.value + randf(-dval,dval),   rc.p1,  top,   lft,    ctr   ) );
		divide( nlevels, level-1, Rect( rc.value + randf(-dval,dval),   top,    rc.p2, ctr,    rgt   ) );
		divide( nlevels, level-1, Rect( rc.value + randf(-dval,dval),   lft,    ctr,   rc.p3,  bot   ) );
		divide( nlevels, level-1, Rect( rc.value + randf(-dval,dval),   ctr,    rgt,   bot,    rc.p4 ) );

	};

	void divide( int nlevels, int level, const Rect& rc ){
		printVec(rc.p1); printVec(rc.p2); printVec(rc.p3); printVec(rc.p4); printf(" rc.ps \n");
		double rnd  = randf(); 
		double area = rc.area();
		if((level>0)&&( area>minarea )){
			if      ( rnd < thresh[0] ){ divide_1( nlevels, level, rc ); }
			//else if ( rnd < FIELD_PATCH_thresh[1] ){ divide_2( nlevels, level, rect ); }
			else{ 
				if( level>(nlevels-startkill) ){ divide_1( nlevels, level, rc );  }
				else                           { rc.draw(); quadCount++;    }; 
			}; 
		}else{ rc.draw(); quadCount++; };
	};

	int makeList( int nlevels, const Rect& rect ){
		int ilist=glGenLists(1);
		quadCount = 0;
		glNewList( ilist, GL_COMPILE );
			divide( nlevels, nlevels, rect );
		glEndList();
		printf(" FieldPatch made %i quads \n", quadCount );
		return( ilist );
	}

/*
	void divide_2( int nlevels, int level, Rect rc ){
	  //rc.paint();
	  double rnd = randf(1);
	  if(rnd>0.5){
		double f1 = 0.5+randf(-maxdev,maxdev); double mf1 = 1-f1;
		double f2 = 0.5+randf(-maxdev,maxdev); double mf2 = 1-f2;
		double topx = (mf1*rc.x1+f1*rc.x2);   double topy = (mf1*rc.y1+f1*rc.y2); double topz = (mf1*rc.z1+f1*rc.z2);
		double botx = (mf2*rc.x3+f2*rc.x4);   double boty = (mf2*rc.y3+f2*rc.y4); double botz = (mf2*rc.z3+f2*rc.z4);
		divide( nlevels, level-1, new Rect( rc.value + randf(-dval,dval),   rc.x1, rc.y1, rc.y2,   topx,  topy,  topz,    rc.x3, rc.y3, rc.z3,   botx,  boty,  botz  ) );
		divide( nlevels, level-1, new Rect( rc.value + randf(-dval,dval),   topx,  topy,  topz,    rc.x2, rc.y2, rc.z2,   botx,  boty,  botz,    rc.x4, rc.y4, rc.y4 ) );
	  }else{
		double f1 = 0.5+randf(-maxdev,maxdev); double mf1 = 1-f1;
		double f2 = 0.5+randf(-maxdev,maxdev); double mf2 = 1-f2;
		double lftx = (mf1*rc.x1+f1*rc.x3);   double lfty = (mf1*rc.y1+f1*rc.y3); double lftz = (mf1*rc.z1+f1*rc.z3);
		double rgtx = (mf2*rc.x2+f2*rc.x4);   double rgty = (mf2*rc.y2+f2*rc.y4); double rgtz = (mf2*rc.z2+f2*rc.z4);
		divide( nlevels, level-1, new Rect( rc.value + randf(-dval,dval),   rc.x1, rc.y1, rc.z1,   rc.x2, rc.y2, rc.z2,   lftx,  lfty,  lftz,     rgtx,  rgty,  rgtz   ) );
		divide( nlevels, level-1, new Rect( rc.value + randf(-dval,dval),   lftx,  lfty,  lftz,    rgtx, rgty, rgtz,      rc.x3, rc.y3, rc.z3,    rc.x4, rc.y4, rc.y4  ) );
	  }
	};
*/

}






