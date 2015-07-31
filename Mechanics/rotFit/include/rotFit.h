
class SystemTransformOptimizer{
	public:
	int npoints;
	Vec3d  *  points;
	Vec3d  *  Tpoints;
	Vec3d  *  forces;
	OptimizerDerivs * optimizer;

	SystemTransformOptimizer( int npoints_, Vec3d * points_ ){
		npoints = npoints_;
		Tpoints      = new Vec3d[npoints];
		forces       = new Vec3d[npoints];
		points  = points_;
		int nparams = 3 + 9;
		double * params      = new double[nparams];
		double * vparams     = new double[nparams];
		double * fparams     = new double[nparams];
		//optimizer   = new OptimizerDerivs( nparams, params, vparams, fparams, NULL );
		optimizer   = new OptimizerFIRE( nparams, params, vparams, fparams, NULL );
		optimizer->dt      = 0.05;
		//optimizer->damping = 0.2;
		Vec3d * p0  = (Vec3d*)&optimizer->pos[0];
		Mat3d * T   = (Mat3d*)&optimizer->pos[3]; 
		p0->set(0.0);
		T->setOne( );
	}

	void transformPoints( const Vec3d& p0, const Mat3d& T ){
/*
		printVec( p0 );
		printVec( T.a );
		printVec( T.b );
		printVec( T.c );
*/
		for( int i=0; i<npoints; i++ ){ 
			Vec3d Tp;
			T.dot_to_T(   points[i],  Tp );  
			Tpoints[i].set_add( p0, Tp  ); 
		}
	}

	// compute derivatives of energy acording to rotation matrix from forces on points
	void getDerivsTransform( Vec3d * forces, Mat3d& fT ){
		// dE/dax = Sum_i dE/dxi * dxi/dax = Sum_i fxi * dxi/dax
		// xi       = ai*ax + bi*bx * ci*cx 
		// dxi/dax  = ai
		for( int i=0; i<npoints; i++ ){ 
			Vec3d p,f; 
			p.set( points[i] );
			f.set( forces[i] );
			//fT.ax += f.x * p.a;  
			//fT.ay += f.y * p.a;  
			//fT.az += f.z * p.a;
			fT.a.add_mul( f, p.a );
			fT.b.add_mul( f, p.b );
			fT.c.add_mul( f, p.c );
		}
	}

	void getDerivsShift( Vec3d * forces, Vec3d& fp0 ){
		for( int i=0; i<npoints; i++ ){  fp0.add( forces[i] );	}
	}

	// compute derivatives of orthonormality contrains acording to rotation matrix
	void getOrtnormDerivs( double kOrt, double kNorm, const Mat3d& T, Mat3d& fT ){
		// E_orth = 0.5*kOrt  * (   (a.b)^2 +   (a.c)^2 +   (b.c)^2  )
		// E_norm = 0.5*kNorm * ( (1-a.a)^2 + (1-b.b)^2 + (1-c.c)^2  )
		// dE/dax = kNorm*( (1-a.a)*ax  ) + kOrt*( (a.b)*bx + (a.c)*cx )
		double kaa = ( T.a.dot( T.a ) - 1 )* kNorm;
		double kbb = ( T.b.dot( T.b ) - 1 )* kNorm;
		double kcc = ( T.c.dot( T.c ) - 1 )* kNorm;
		double kab = T.a.dot( T.b )        * kOrt;
		double kac = T.a.dot( T.c )        * kOrt;
		double kbc = T.b.dot( T.c )        * kOrt;
		fT.ax = kaa*T.ax + kab*T.bx + kbb*T.cx; 
		fT.ay = kaa*T.ay + kab*T.by + kbb*T.cy; 
		fT.az = kaa*T.az + kab*T.bz + kbb*T.cz; 
		fT.bx = kbb*T.bx + kab*T.ax + kbc*T.cx; 
		fT.by = kbb*T.by + kab*T.ay + kbc*T.cy; 
		fT.bz = kbb*T.bz + kab*T.az + kbc*T.cz; 
		fT.cx = kcc*T.cx + kac*T.ax + kbc*T.bx; 
		fT.cy = kcc*T.cy + kac*T.ay + kbc*T.by; 
		fT.cz = kcc*T.cz + kac*T.az + kbc*T.bz; 
	}

	void fitStep( double kFit, double kOrt, double kNorm,  Vec3d * pos0s ){
		Vec3d * p0  = (Vec3d*)&optimizer->pos[0];
		Mat3d * T   = (Mat3d*)&optimizer->pos[3]; 
		Vec3d * fp0 = (Vec3d*)&optimizer->force[0];
		Mat3d * fT  = (Mat3d*)&optimizer->force[3]; 
		transformPoints( *p0, *T );
		getSpringForce<double>( npoints*3, (double*)Tpoints, (double*)pos0s, (double*)forces, kFit );
		fT->set(0.0);
		getOrtnormDerivs( kOrt, kNorm, *T, *fT);
		//getOrtnormDerivs( -1.0, -1.0, *T, *fT);
		getDerivsTransform( forces, *fT );
		fp0->set(0.0);
		getDerivsShift ( forces, *fp0 );
		printf("%i force: ", optimizer->stepsDone ); printArray( 12, optimizer->force );
		//printf("%i vel  : ", optimizer->stepsDone ); printArray( 12, optimizer->vel );
		//optimizer->autoTimeStep( 0.1, 5.0, 5.0 );
		optimizer->move();
	}

};
