
typedef void (*FunctionDerivatives)( int n, double * xs, double * dfs );

/*
const double RELAX_damping   = 0.1;
const double RELAX_dt        = 0.1;
const double RELAX_convF2    = 0.000001;
const int    RELAX_maxIters  = 1000;
*/



class OptimizerDerivs{
	public:
		int n;
		double * pos;
		double * vel;
		double * force;

		double dt=0.05;
		double damping=0.1;

		FunctionDerivatives func;

	OptimizerDerivs(int n_, double* pos_, double* vel_, double* force_, FunctionDerivatives func_  ){
		n = n_;
		pos   = pos_;
		vel   = vel_;
		force = force_;
		func  = func_;
	};

	virtual void step(){
		func( n , pos, force );
		double cdamp = 1 - damping;
		for ( int i=0; i<n; i++ ){
			vel[i]  = cdamp*vel[i] + dt*force[i];
			pos[i] += dt*vel[i];
			//force[i] = 0;
		}
	};
};


/*
class OptimizerFIRE : public OptimizerDerivs {
	public:
	const double FIRE_finc    = 1.1; 
	const double FIRE_fdec    = 0.5;
	const double FIRE_falpha  = 0.99;
	const double FIRE_dtmax   = RELAX_dt; 
	const double FIRE_acoef0  = RELAX_damping;
	double FIRE_dt    = RELAX_dt;
	double FIRE_acoef = FIRE_acoef0;

	inline void  move_FIRE( const Vec3d& f, Vec3d& r, Vec3d& v ){
		double ff = f.norm2();
		double vv = v.norm2();
		double vf = f.dot(v);
		if( vf < 0 ){
			v.set( 0.0d );
			FIRE_dt    = FIRE_dt * FIRE_fdec;
		  	FIRE_acoef = FIRE_acoef0;
		}else{
			double cf  =     FIRE_acoef * sqrt(vv/ff);
			double cv  = 1 - FIRE_acoef;
			v.mul( cv ); v.add_mul( f, cf );	// v = cV * v  + cF * F
			FIRE_dt     = fmin( FIRE_dt * FIRE_finc, FIRE_dtmax );
			FIRE_acoef  = FIRE_acoef * FIRE_falpha;
		}
		v.add_mul( f , FIRE_dt );
		r.add_mul( v , FIRE_dt );	
	}
}
*/


