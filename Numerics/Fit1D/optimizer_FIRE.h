
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

		double dt=0.2;
		double damping=0.1;

		FunctionDerivatives func;

	OptimizerDerivs(int n_, double* pos_, double* vel_, double* force_, FunctionDerivatives func_  ){
		n = n_;
		pos   = pos_;
		vel   = vel_;
		force = force_;
		func  = func_;
		for(int i=0; i<n; i++){
			vel  [i]=0;
			force[i]=0;
		}
	};

	virtual void step(){
		func( n , pos, force );
		double cdamp = 1 - damping;
		for ( int i=0; i<n; i++ ){
			vel[i]  = cdamp*vel[i] + dt*force[i];
			pos[i] += dt*vel[i];
		}
	};
};



class OptimizerFIRE : public OptimizerDerivs {
	public:
		double dt=0.1;
		double damping=0.1;
		double finc     = 1.1; 
		double fdec     = 0.5;
		double falpha   = 0.99;
		double dt_var   = dt; 
		double damp_var = damping;

	OptimizerFIRE(int n_, double* pos_, double* vel_, double* force_, FunctionDerivatives func_  ):
	OptimizerDerivs  ( n_, pos_, vel_, force_, func_   ){
	};

	virtual void step(){
		func( n , pos, force );
		double ff=0,vv=0,vf=0;
		for(int i=0; i<n; i++){
			double fi = force[i];
			double vi = vel[i];
			ff += fi*fi;
			vv += vi*vi;
			vf += vi*fi;
		}
		if( vf < 0 ){
			for(int i=0; i<n; i++){ vel[i] = 0.0d; }
			dt_var   = dt * fdec;
		  	damp_var = damping;
		}else{
			double cf  =     damp_var * sqrt(vv/ff);
			double cv  = 1 - damp_var;
			for(int i=0; i<n; i++){
				vel[i]      = cv * vel[i]  + cf * force[i];
			}
			dt_var    = fmin( dt_var * finc, dt );
			damp_var  = damp_var * falpha;
		}
		for ( int i=0; i<n; i++ ){
			vel[i]  = dt*force[i];
			pos[i] += dt*vel[i];
		}
	}
};



