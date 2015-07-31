
class SystemTransformOptimizer{
	public:
	int npoints;
	Vec3d  *  points;
	Vec3d  *  Tpoints;
	Vec3d  *  forces;
	OptimizerDerivs * optimizer;

	void getCOG( Vec3d * points, Vec3d& cog ){
		cog.set(0.0);
		for( int i=0; i<npoints; i++ ){ cog.add( points[i] ); }
		cog.mul( 1.0/npoints );
	}

	void extractCOG( Vec3d * points, Vec3d& cog ){
		getCOG( points, cog );
		for( int i=0; i<npoints; i++ ){ points[i].sub( cog );  }
	}

	void fitCOG( Vec3d * point0s ){
		Vec3d cog0;   getCOG( point0s, cog0 );
		Vec3d  * p = (Vec3d* )&optimizer->pos[0];
		p->set( cog0 );
	}

	SystemTransformOptimizer( int npoints_, Vec3d * points_ ){
		npoints = npoints_;
		Tpoints      = new Vec3d[npoints];
		forces       = new Vec3d[npoints];
		points  = points_;
		int nparams = 3 + 4;
		double * params      = new double[nparams];
		double * vparams     = new double[nparams];
		double * fparams     = new double[nparams];
		//optimizer   = new OptimizerDerivs( nparams, params, vparams, fparams, NULL );
		optimizer   = new OptimizerFIRE( nparams, params, vparams, fparams, NULL );
		optimizer->dt      = 0.2;
		//optimizer->damping = 0.2;
		Vec3d  * p = (Vec3d* )&optimizer->pos[0];   extractCOG( points, *p );
		Quat4d * q = (Quat4d*)&optimizer->pos[3];   q ->setOne( );
	}

	void transformPoints( const Vec3d& p0, const Quat4d& q ){
		Mat3d T;
		//q.toMatrix_unitary2( T );
		q.toMatrix( T);
		//printVec( T.a );
		//printVec( T.b );
		//printVec( T.c );
		//printf( " %f %f %f \n", T.a.dot(T.b), T.a.dot(T.c), T.b.dot(T.c) );
		//printf( " %f %f %f \n", T.a.dot(T.a), T.b.dot(T.b), T.c.dot(T.c) );
		for( int i=0; i<npoints; i++ ){ 
			Vec3d Tp;
			T.dot_to_T(   points[i],  Tp );  
			//T.dot_to(   points[i],  Tp ); 
			Tpoints[i].set_add( p0, Tp  ); 
		}
	}

	void fitStep( double kFit, double kOrt, double kNorm,  Vec3d * pos0s ){
		Vec3d  *  p = (Vec3d* )&optimizer->pos[0];
		Quat4d *  q = (Quat4d*)&optimizer->pos[3]; 
		Vec3d  * fp = (Vec3d* )&optimizer->force[0];
		Quat4d * fq = (Quat4d*)&optimizer->force[3];
		//double rq2 =q.norm2(); if( fabs(rq2-1.00)>0.0001 ) q.mult( 1 / sqrt( rq2 ) ); 
		q->normalize();
		//printf( " %f    %f %f %f %f \n",  q->norm(),  q->x, q->y, q->z, q->w );
		transformPoints( *p, *q );
		getSpringForce<double>( npoints*3, (double*)Tpoints, (double*)pos0s, (double*)forces, kFit );
		fq->set(0.0,0.0,0.0,0.0); fp->set(0.0);
		for( int i=0; i<npoints; i++ ){ 
			q ->addForceFromPoint( points[i], forces[i], *fq ); 
			fp->add( forces[i] );
		}
		double qfq = q->dot( *fq );	fq->add_mul( *q, -qfq );	// remove force component in direction of constrain
		//printArray( 3+4, optimizer->force );
		optimizer->move();
	}

};
