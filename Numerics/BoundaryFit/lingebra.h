

// ===============================================
// ======                                  =======
// ======              vector              =======
// ======                                  =======
// ===============================================

inline double dot(	int n,  double* a, double* b ){
	double sum = 0;
	for (int i=0; i<n; i++ ){		sum+= a[i]*b[i];	} 
	return sum;
}

inline void set( int n, double  f,            double* out ){  	for (int i=0; i<n; i++ ){ out[i] = f;	      } }
inline void add( int n, double  f, double* b, double* out ){  	for (int i=0; i<n; i++ ){ out[i] = f+b[i];    } }
inline void mul( int n, double  f, double* b, double* out ){  	for (int i=0; i<n; i++ ){ out[i] = f*b[i];    } }

inline void set( int n, double* a,            double* out ){  	for (int i=0; i<n; i++ ){ out[i] = a[i];      } }
inline void add( int n, double* a, double* b, double* out ){  	for (int i=0; i<n; i++ ){ out[i] = a[i]+b[i]; } }
inline void sub( int n, double* a, double* b, double* out ){  	for (int i=0; i<n; i++ ){ out[i] = a[i]-b[i]; } }
inline void mul( int n, double* a, double* b, double* out ){  	for (int i=0; i<n; i++ ){ out[i] = a[i]*b[i]; } }
inline void div( int n, double* a, double* b, double* out ){  	for (int i=0; i<n; i++ ){ out[i] = a[i]/b[i]; } }

void random_vector ( int n, double xmin, double xmax, double * out ){
	double xrange = xmax - xmin;
	for (int i=0; i<n; i++ ){		out[i] = xmin + xrange*randf();	} 
}

void print_vector( int n, double * a ){
	for (int i=0; i<n; i++ ){	printf( "%f ", a[i] );	} 
	printf( "\n" );
}

// ===============================================
// ======                                  =======
// ======              Matrix              =======
// ======                                  =======
// ===============================================


// creates double** from any continuous memory block folowing *p
double ** from_continuous( int m, int n, double *p ){
	double ** A = new double*[m];
	for (int i=0; i<m; i++ ){ A[i] = &(p[i*n]); }
	return A;
}

// allocates new matrix 
double ** new_matrix( int m, int n ){
	double ** A = new double*[m];
	for (int i=0; i<m; i++ ){ A[i] = new double[n]; }
	return A;
}

// delete matrix
double ** delete_matrix( int m, double** A ){
	for (int i=0; i<m; i++ ){ delete A[i]; }
	delete [] A;
}


// transpose matrix
void transpose( int m, int n, double** A, double** TA ){
	for (int i=0; i<m; i++ ){
		for (int j=0; j<n; j++ ){
			TA[i][j] = A[j][i];
		} 
	} 
}

// dot product
void dot( int m, int n, double** A, double* x, double* out ){
	for (int i=0; i<m; i++ ){
		out[i] = dot( n, A[i], x );
	} 
}

inline void set( int m, int n, double   f,             double** out ){  	for (int i=0; i<m; i++ ){ set( n, f,       out[i] );  } }
inline void add( int m, int n, double   f, double** B, double** out ){  	for (int i=0; i<m; i++ ){ add( n, f, B[i], out[i] );  } }
inline void mul( int m, int n, double   f, double** B, double** out ){  	for (int i=0; i<m; i++ ){ mul( n, f, B[i], out[i] );  } }

inline void set( int m, int n, double** A,             double** out ){  	for (int i=0; i<m; i++ ){ set( n, A[i],       out[i] );   } }
inline void add( int m, int n, double** A, double** B, double** out ){  	for (int i=0; i<m; i++ ){ add( n, A[i], B[i], out[i] );   } }
inline void sub( int m, int n, double** A, double** B, double** out ){  	for (int i=0; i<m; i++ ){ sub( n, A[i], B[i], out[i] );   } }
inline void mul( int m, int n, double** A, double** B, double** out ){  	for (int i=0; i<m; i++ ){ mul( n, A[i], B[i], out[i] );   } }
inline void div( int m, int n, double** A, double** B, double** out ){  	for (int i=0; i<m; i++ ){ div( n, A[i], B[i], out[i] );   } }

void random_matrix( int m, int n, double xmin, double xmax, double** out ){
	double xrange = xmax - xmin;
	for (int i=0; i<m; i++ ){  random_vector ( n, xmin, xmax, out[i] ); } 
}

void print_matrix( int m, int n, double ** A ){
	for (int i=0; i<m; i++ ){	print_vector( n, A[i] );	} 
}


//  Q =  Sum_i |a_i> k_i <a_i|
void makeQuadricFormMatrix( int m, int n, double * ks, double ** A, double ** Q ){
	for (int im=0; im<m; im++ ){
		double kim   = ks[im];
		double * Aim =  A[im]; 
		for (int i=0; i<n; i++ ){
			for (int j=0; j<i; j++ ){
				double Qij = kim*Aim[i]*Aim[j];
				Q[i][j] += Qij;
				Q[j][i] += Qij;
			}
			Q[i][i] += kim*Aim[i]*Aim[i];
		}
	}
}

//  y =  < x | Q.x >
double evalQudraticForm( int n, double* x, double** Q ){
	double y =  0;
	for (int i=0; i<n; i++ ){	y += x[i] * dot( n, Q[i], x );   } 
	return y;
}

//  y = Sum_i k_i < a_i | x >^2
double evalQudraticFormDirs( int m, int n, double* x, double* k, double** A ){
	double y =  0;
	for (int i=0; i<m; i++ ){
		double ri = dot( n, A[i], x );
		y += k[i]*ri*ri;
	} 
	return y;
}
