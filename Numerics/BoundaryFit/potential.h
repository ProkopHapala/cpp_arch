

//const int nsource = 14;
//const int ncontrol = 30;


const int nsource = 30;
const int ncontrol = 50;

double * source_val;
double * source_x  ;
double * source_y  ;

double * control_val;
double * control_x  ;
double * control_y  ;

double ** couplings;



double profile( double x ){
	double f = sqrt( x*x + 1 );
	f = f<3.5?f:3.5;
	return f;
}

void makeCPoints( double x1, double x2 ){
	control_x    = new double[ncontrol];
	control_y    = new double[ncontrol];
	control_val  = new double[ncontrol];
	int nhalf = ncontrol/2;
	double dx = (x2-x1)/nhalf;
	for ( int i=0; i<nhalf; i++ ){
		double x = (i+0.5)*dx + x1;
		control_y  [ i ] = profile( x ); 
		control_x  [ i ] = x; 
		control_val[ i ] = 1;
		control_y  [ i + nhalf ] = - control_y[ i ]; 
		control_x  [ i + nhalf ] =   control_x[ i ]; 
		control_val[ i + nhalf ] = - control_val[ i ];
	}
	source_x    = new double[nsource];
	source_y    = new double[nsource];
	source_val  = new double[nsource];
	nhalf = nsource/2;
	dx = (x2-x1)/nhalf;
	for ( int i=0; i<nhalf; i++ ){
		double x = (i+0.5)*dx + x1;
		source_y  [ i ] = profile( x ) + dx; 
		source_x  [ i ] = x; 
		source_val[ i ] = 1;
		source_y  [ i + nhalf ] = - source_y[ i ]; 
		source_x  [ i + nhalf ] =   source_x[ i ]; 
		source_val[ i + nhalf ] = - source_val[ i ];
	}
}


double basisFunc( double x, double y ){
	// basis function is Green's function of poisson equation 1/r
	double r2 = x*x + y*y;
	return 1/sqrt(r2);
	//return exp(-r2);
	//return -log( r2 );
}

double evalInPoint( double x, double y  ){
	double f=0;
	for ( int i=0; i<nsource; i++ ){
		f+= basisFunc( x-source_x[i], y-source_y[i] )*source_val[i];
	}
	//double f=sin(x)*sin(y);
	return f;
}

void makeCouplings( 
	//int nsource, int ncontrol, 
	//double * source_x,  double * source_y, 
	//double * control_x, double * control_y,
	//double ** couplings
){
	couplings = new_matrix( ncontrol, nsource );
	for (int i=0; i<ncontrol; i++){
		double control_xi   = control_x[i];
		double control_yi   = control_y[i];
		for (int j=0; j<nsource; j++){
			double dx = control_xi - source_x[j];
			double dy = control_yi - source_y[j];
			couplings[i][j] = basisFunc( dx, dy ); 
		}
	}
}

void getVarDerivs( int nsource, int ncontrol, double * source_val, double * control_val, double ** couplings, double * varDerivs ){
	//printf(" here \n" );
	for (int i=0; i<nsource; i++){  varDerivs[i] = 0; }
	for (int i=0; i<ncontrol; i++){
		double erri = dot( nsource, couplings[i], source_val ) - control_val[i];  // residual
		for (int j=0; j<nsource; j++){
			varDerivs[j] -= 2*erri * couplings[i][j]; 
		}
	}
}


void getVarDerivs_ ( int n, double * xs, double * dfs ){
	getVarDerivs( n, ncontrol, xs, control_val, couplings, dfs );
	//printf( " HERE \n" );
}






