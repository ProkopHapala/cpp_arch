

/*

pocitac v praci:
cat /proc/cpuinfo: Intel(R) Core(TM) i5 CPU  750  @ 2.67GHz	cache size	: 8192 KB
 ========= TESTING : 100000000 8 256 255 
 ground sum+=i :  4.281655 ticks/op  | 5.1e+10 
 one [ i+1 ]   :  3.997244 ticks/op  | 1.33369e+06 
 way 1 [ i+1 ] :  6.815794 ticks/op  | 2.70021e+06 
 way 2 i++     :  8.197547 ticks/op  | 2.70021e+06 
 way 3 struct  :  5.328529 ticks/op  | 2.70021e+06 
 ========= TESTING : 100000000 16 65536 65535
 ground sum+=i :  4.491579 ticks/op  | 1.3107e+13 
 one [ i+1 ]   :  6.618622 ticks/op  | -13378.8 
 way 1 [ i+1 ] : 12.666463 ticks/op | 3333.2 
 way 2 i++     : 12.885315 ticks/op | 3333.2 
 way 3 struct  : 12.594434 ticks/op | 3333.2 
 ========= TESTING : 100000000 24 16777216 16777215 
 ground sum+=i :  4.328206 ticks/op | 3.35545e+15 
 one [ i+1 ]   : 58.047087 ticks/op | -9077.7 
 way 1 [ i+1 ] : 67.505121 ticks/op | 7743.63 
 way 2 i++     : 67.585773 ticks/op | 7743.63 
 way 3 struct  : 66.777933 ticks/op | 7743.63 
 ========= TESTING : 100000000 26 67108864 67108863 
 ground sum+=i : 4.182916 ticks/op | 1.34218e+16 
 one [ i+1 ]   : 91.305321 ticks/op | -6009.1 
 way 1 [ i+1 ]  : 112.112395 ticks/op | -1844.56 
 way 2 i++    : 111.976128 ticks/op | -1844.56 
 way 3 struct : 112.066090 ticks/op | -1844.56 

pocitac na koleji:
model name      : Intel(R) Core(TM)2 Quad  CPU   Q9450  @ 2.66GHz


 ========= TESTING : 100000000 8 256 255                                                                                                   
 ground sum+=i : 5.200008 ticks/op | 5.1e+10                                                                                               
 one [ i+1 ]   : 4.526244 ticks/op | 1.33369e+06                                                                                           
 way 1 [ i+1 ]  : 6.326529 ticks/op | 2.70021e+06                                                                                          
 way 2 i++    : 6.850952 ticks/op | 2.70021e+06                                                                                            
 way 3 struct : 6.429329 ticks/op | 2.70021e+06
 ========= TESTING : 100000000 16 65536 65535
 ground sum+=i : 5.298187 ticks/op | 1.3107e+13                                                                                            
 one [ i+1 ]   : 6.579840 ticks/op | -13378.8                                                                                              
 way 1 [ i+1 ]  : 16.386131 ticks/op | 3333.2                                                                                              
 way 2 i++    : 16.347636 ticks/op | 3333.2                                                                                                
 way 3 struct : 16.550085 ticks/op | 3333.2 
 ========= TESTING : 100000000 24 16777216 16777215 
 ground sum+=i : 5.208846 ticks/op | 3.35545e+15 
 one [ i+1 ]   : 40.014621 ticks/op | -9077.7 
 way 1 [ i+1 ]  : 75.483584 ticks/op | 7743.63 
 way 2 i++    : 75.370764 ticks/op | 7743.63 
 way 3 struct : 75.439036 ticks/op | 7743.63
 ========= TESTING : 100000000 26 67108864 67108863 
 ground sum+=i : 5.057475 ticks/op | 1.34218e+16 
 one [ i+1 ]   : 43.389161 ticks/op | -6009.1 
 way 1 [ i+1 ]  : 83.976206 ticks/op | -1844.56 
 way 2 i++    : 85.266193 ticks/op | -1844.56 
 way 3 struct : 85.305102 ticks/op | -1844.56






*/

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <cstdlib>

//#define hash( int r ) 1664525*r ^ 1013904223
//inline uint32_t hash( uint32_t r ){ return 1664525*r ^ 1013904223; }
//inline uint32_t hash( uint32_t r ){ return r ^ 1013904223; }

inline uint32_t hash( uint32_t r ){	r = 1664525*r ^ 1013904223;	r = 1664525*r ^ 1013904223;	return r; }

const  float INV_RAND_MAX = 1.0f/RAND_MAX;
inline float randf(){ return INV_RAND_MAX*rand(); }

inline uint64_t rdtsc(){
    uint32_t lo, hi;
    __asm__ __volatile__ (
      "xorl %%eax, %%eax\n"
      "cpuid\n"
      "rdtsc\n"
      : "=a" (lo), "=d" (hi)
      :
      : "%ebx", "%ecx" );
    return (uint64_t)hi << 32 | lo;
}

// ===============================
// ===== GLOBAL VARIABLES
// ===============================


int ntimes = 1e+8;

const uint8_t npower = 8;

const uint32_t nsamples = ( 1 << npower );
const uint32_t mask     = nsamples-1;

const int nsamptot = 4*nsamples+3;
double * doubles   = new double[ nsamptot];

int main( int argc, char* args[] ){

	long  t1, t2;
	double sum = 0;

	printf( " ========= GENERATE SAMPLES : %i %i %i \n", npower, nsamples, mask );

	sum = 0;
	for( int i=0; i< nsamptot; i++ ){
		doubles[i] = randf()-0.5d; 
		sum += doubles[i];
		//printf( " %i %f \n", i, doubles[i] );
	}
	printf( " sum = : %f \n", sum );

	printf( " ========= TESTING : %i %i %i %i \n", ntimes, npower, nsamples, mask );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		sum += ii;
	}  
	t2 =  rdtsc();
	printf( " ground sum+=i : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ), sum );
	

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		double a1 = doubles[ ii   ]; 
		sum += a1;
		//printf( " %i %i %f %f %f %f \n", i, ii,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " one [ i+1 ]   : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ),  sum );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		double a1 = doubles[ ii   ]; 
		double a2 = doubles[ ii+1 ]; 
		double a3 = doubles[ ii+2 ]; 
		double a4 = doubles[ ii+3 ]; 
		sum += a1 + a2 + a3 + a4;
		//printf( " %i %i %f %f %f %f \n", i, ii,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " way 1 [ i+1 ]  : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ),  sum );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		double a1 = doubles[ ii ]; ii++;
		double a2 = doubles[ ii ]; ii++;
		double a3 = doubles[ ii ]; ii++;
		double a4 = doubles[ ii ];
		sum += a1 + a2 + a3 + a4;
		//printf( " %i %i %f %f %f %f \n", i, ii-3,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " way 2 i++    : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ), sum );


	sum = 0;
	t1 =  rdtsc();
	struct double4{	double a1,a2,a3,a4;	};
	double4* double4s = ( double4* )&doubles[0];
	for (int i=0; i<ntimes; i++  ){
		int ii    = hash( i ) & mask;
		double4 u  = double4s[ ii ];
		sum += u.a1 + u.a2 + u.a3 + u.a4;
		//printf( " %i %i %f %f %f %f \n", i, ii,  u.a1, u.a2, u.a3, u.a4 );
	}  
	t2 =  rdtsc();
	printf( " way 3 struct : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ),  sum );

}
