

int nx = 300;
int ny = 300;

double * psi;
double * pot;

double dx = 0.1;
double dy = 0.1;

double idx = 1/dx;
double idy = 1/dy;


//////// Derivation 
//
// E<psi|psi> = < psi | d2x + d2y  + pot | psi >
// E(x,y) = 0 =  psi(x,y) * ( d2x + d2y  + pot )
//           =  psi(x,y) * ( psi(x+1,y) + psi(x-1,y) - 2*psi(x,y) + psi(x,y+1) + psi(x,y-1) - 2*psi(x,y)  +  pot(x,y) * psi(x,y) ) 
//	       =  psi(x,y) * ( psi(x+1,y) + psi(x-1,y) - psi(x,y+1) + psi(x,y-1) )    +  ( -2*idx -2*idy + pot(x,y) ) * psi(x,y) )
// psi(x,y)   =  psi(x+1,y) + psi(x-1,y) - psi(x,y+1) + psi(x,y-1) ) / ( 2*idx + 2*idy - pot(x,y) ) 
//
//////////

double solve_step( double bmix, double renorm ){
	double amix = 1 - bmix;
	double rho = 0.0;
	for (int iy=1; iy<ny-1; iy++){
		int i0y = iy*nx; 
		for (int ix=1; ix<nx-1; ix++){
			int ixy = i0y + ix;
			double psixy   = psi[ ixy ];
			double potxy   = pot[ ixy ];
			//float dfdy  = ( psi[ixy + nx] + psi[ixy - nx] - psixy ) * idx;
			//float dfdx  = ( psi[ixy + 1 ] + psi[ixy - 1 ] - psixy ) * idy;
			double d2xoff  = psi[ ixy + 1  ] + psi[ ixy - 1  ];
			double d2yoff  = psi[ ixy + nx ] + psi[ ixy - nx ];
			double psixy_  = ( d2xoff*idx + d2yoff*idy )/( 2*idx + 2*idy - potxy );   //   E(x,y)[ psixy_ ] = 0; if assuming constant neighbors
			double new_psi = ( amix * psixy  +  bmix * psixy_ ) * renorm; 
			psi[ixy]       = new_psi;
			rho           += new_psi * new_psi;
		}
	}
	return rho;
}




