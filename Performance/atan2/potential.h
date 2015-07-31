

const int nsource = 14;
const int ncontrol = 30;

const double gwidth = 0.5;

//const int nsource = 4;
//const int ncontrol = 7;

double * source_val;
double * source_x  ;

double * control_val;
double * control_x  ;

double ** couplings;


double profile( double x ){
	return sqrt( x*x + 1 );
}

void makeCPoints( double x1, double x2 ){
	control_x    = new double[ncontrol];
	control_val  = new double[ncontrol];
	double dx = (x2-x1)/ncontrol;
	for ( int i=0; i<ncontrol; i++ ){
		double x = (i+0.5)*dx + x1;
		control_x  [ i ] = x; 
		control_val[ i ] = (1.5+sin(x));
	}
	source_x    = new double[nsource];
	source_val  = new double[nsource];
	dx = (x2-x1)/nsource;
	for ( int i=0; i<nsource; i++ ){
		double x = (i+0.5)*dx + x1;
		source_x  [ i ] = x; 
		source_val[ i ] = 0.1;
	}
}

double basisFunc( double x ){
	return exp( -gwidth*x*x );
}

double evalInPoint( double x  ){
	double f=0;
	for ( int i=0; i<nsource; i++ ){
		f+= basisFunc( x-source_x[i] )*source_val[i];
	}
	return f;
}

void makeCouplings( ){
	couplings = new_matrix( ncontrol, nsource );
	for (int i=0; i<ncontrol; i++){
		double control_xi   = control_x[i];
		for (int j=0; j<nsource; j++){
			double dx = control_xi - source_x[j];
			couplings[i][j] = basisFunc( dx ); 
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




