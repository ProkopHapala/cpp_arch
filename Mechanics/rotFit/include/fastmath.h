


template <class TYPE> 
inline TYPE sin_taylor2( TYPE a ){
	const TYPE c3 = 1.0d/6;        
	const TYPE c5 = 1.0d/120;
	TYPE a2 = a*a;
	return    a * ( 1 - a2*( c3 - c5*a2 ) ); 
}

template <class TYPE> 
inline TYPE cos_taylor2( TYPE a ){
	const TYPE c2 = 1.0d/2;        
	const TYPE c4 = 1.0d/24;  
	TYPE a2 = a*a;
	return    1 - a2*( c2 - c4*a2 );
}

template <class TYPE> 
inline void sincos_taylor2( TYPE a, TYPE& sa, TYPE& ca ){
	const TYPE c2 = 1.0d/2;       
	const TYPE c3 = 1.0d/6;      
	const TYPE c4 = 1.0d/24;     
	const TYPE c5 = 1.0d/120; 
	TYPE a2 = a*a;
	sa   = a * ( 1 - a2*( c3 - c5*a2 ) ) ; 
	ca   =       1 - a2*( c2 - c4*a2 )   ;
}

const float  INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }

template <class TYPE> 
inline void getSpringForce( int n, double * ps, double * p0s, double * fs, double k ){
	for(int i=0; i<n; i++){	fs[i] = k*( ps[i] - p0s[i] ); }
}
