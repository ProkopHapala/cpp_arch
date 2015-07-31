


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


template <class TYPE> 
inline TYPE trashold_step( TYPE x, TYPE x1 ){
	if   (x<x1){ return 0.0; }
	else       { return 1.0; }
}	

template <class TYPE> 
inline TYPE trashold_lin( TYPE x, TYPE x1, TYPE x2 ){
	if      (x<x1){ return 0.0; }
	else if (x>x2){ return 1.0; }
	else    {       return (x-x1)/(x2-x1); };
}	

template <class TYPE> 
inline TYPE trashold_cub( TYPE x, TYPE x1, TYPE x2 ){
	if      (x<x1){ return 0.0; }
	else if (x>x2){ return 1.0; }
	else    {  double a =(x-x1)/(x2-x1); return a*a*( 3 - 2*a );  };
}	



const  float INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }
