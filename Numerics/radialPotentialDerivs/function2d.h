

typedef double (*Function1d)( double x);
typedef double (*DiffFunc1d)( double x, double& dx );


typedef double (*Function2d)( double x, double y);
typedef double (*DiffFunc2d)( double x, double y, double& dfdx, double& dfdy );



const double   r0   = 0.75;
const double   rMax =  1.2;

const double   r0_sq   = r0*r0;
const double   rMax_sq = rMax*rMax; 


double radial_1( double x, double y, double& dfdx, double& dfdy ){
	double r2 = x*x + y*y;
	double r  = sqrt(r2);
	double dr =  r - r0; 
	double fr = dr/r;
	dfdx = x*fr;
	dfdy = y*fr;
	return dr*dr;
}






