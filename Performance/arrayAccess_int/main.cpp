

/*

pocitac v praci:
cat /proc/cpuinfo: Intel(R) Core(TM) i5 CPU  750  @ 2.67GHz	cache size	: 8192 KB

 ========= TESTING : 100000000 8 256 255 
 ground sum+=i :  4.318948 ticks/op | 5.1e+10 
 one [ i+1 ]   :  3.888368 ticks/op | 6.13852e+10 
 way 1 [ i+1 ] :  7.572973 ticks/op | 2.14213e+11 
 way 2 i++     :  7.859971 ticks/op | 2.14213e+11 
 way 3 struct  :  6.905404 ticks/op | 2.14213e+11 
 ========= TESTING : 100000000 16 65536 65535 
 ground sum+=i :  4.440580 ticks/op | 1.3107e+13 
 one [ i+1 ]   : 6.811674 ticks/op | 4.10086e+09 
 way 1 [ i+1 ] : 13.463355 ticks/op | -4.68451e+09 
 way 2 i++     : 13.327268 ticks/op | -4.68451e+09 
 way 3 struct  : 12.635988 ticks/op | -4.68451e+09 
 ========= TESTING : 100000000 24 16777216 16777215 
 ground sum+=i :  4.187810 ticks/op | 3.35545e+15 
 one [ i+1 ]   : 53.077809 ticks/op | -5.26693e+08 
 way 1 [ i+1 ] : 58.800582 ticks/op | 4.83801e+08 
 way 2 i++     : 59.478881 ticks/op | 4.83801e+08 
 way 3 struct  : 58.479640 ticks/op | 4.83801e+08 
 ========= TESTING : 100000000 26 67108864 67108863 
 ground sum+=i :  4.241079 ticks/op | 1.34218e+16 
 one [ i+1 ]   : 67.190805 ticks/op | -4.41804e+07 
 way 1 [ i+1 ] : 85.879024 ticks/op | -3.36516e+08 
 way 2 i++     : 85.976966 ticks/op | -3.36516e+08 
 way 3 struct  : 85.602729 ticks/op | -3.36516e+08 


 ========= TESTING : 100000000 8 256 255 
 ground sum+=i :  5.222715 ticks/op | 5.1e+10 
 one [ i+1 ]   :  4.728665 ticks/op | 6.13852e+10 
 way 1 [ i+1 ] :  8.498448 ticks/op | 2.14213e+11 
 way 2 i++     :  9.563759 ticks/op | 2.14213e+11 
 way 3 struct  :  8.477970 ticks/op | 2.14213e+11 
 ========= TESTING : 100000000 16 65536 65535 
 ground sum+=i :  6.347432 ticks/op | 1.3107e+13 
 one [ i+1 ]   :  5.477054 ticks/op | 4.10086e+09 
 way 1 [ i+1 ] : 10.763894 ticks/op | -4.68451e+09 
 way 2 i++     : 11.071083 ticks/op | -4.68451e+09 
 way 3 struct  : 11.000542 ticks/op | -4.68451e+09
 ========= TESTING : 100000000 24 16777216 16777215 
 ground sum+=i :  6.461140 ticks/op | 3.35545e+15 
 one [ i+1 ]   : 42.748097 ticks/op | -5.26693e+08 
 way 1 [ i+1 ] : 74.735265 ticks/op | 4.83801e+08 
 way 2 i++     : 70.533496 ticks/op | 4.83801e+08 
 way 3 struct  : 72.511763 ticks/op | 4.83801e+08 
 ========= TESTING : 100000000 26 67108864 67108863 
 ground sum+=i :  6.515340 ticks/op | 1.34218e+16 
 one [ i+1 ]   : 43.443067 ticks/op | -4.41804e+07 
 way 1 [ i+1 ] : 78.263870 ticks/op | -3.36516e+08 
 way 2 i++     : 77.468205 ticks/op | -3.36516e+08 
 way 3 struct  : 78.206328 ticks/op | -3.36516e+08 




*/



#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <cstdlib>


//#define hash( int r ) 1664525*r ^ 1013904223

//inline uint32_t hash( uint32_t r ){ return 1664525*r ^ 1013904223; }

//inline uint32_t hash( uint32_t r ){ return r ^ 1013904223; }

inline uint32_t hash( uint32_t r ){	r = 1664525*r ^ 1013904223;	r = 1664525*r ^ 1013904223;	return r; }

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

const uint8_t npower = 26;

const uint32_t nsamples = ( 1 << npower );
const uint32_t mask     = nsamples-1;

const int nsamptot = 4*nsamples+3;
int * ints   = new int[ nsamptot];

int main( int argc, char* args[] ){

	long  t1, t2;
	double sum = 0;

	printf( " ========= GENERATE SAMPLES : %i %i %i \n", npower, nsamples, mask );

	sum = 0;
	for( int i=0; i< nsamptot; i++ ){
		ints[i] = ( rand()&0xFFFF ) - 0x8000; 
		sum += ints[i];
		//printf( " %i %f \n", i, ints[i] );
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
		int a1 = ints[ ii   ]; 
		sum += a1;
		//printf( " %i %i %f %f %f %f \n", i, ii,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " one [ i+1 ]   : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ),  sum );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		int a1 = ints[ ii   ]; 
		int a2 = ints[ ii+1 ]; 
		int a3 = ints[ ii+2 ]; 
		int a4 = ints[ ii+3 ]; 
		sum += a1 + a2 + a3 + a4;
		//printf( " %i %i   %i %i %i %i \n", i, ii,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " way 1 [ i+1 ]  : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ),  sum );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		int a1 = ints[ ii ]; ii++;
		int a2 = ints[ ii ]; ii++;
		int a3 = ints[ ii ]; ii++;
		int a4 = ints[ ii ];
		sum += a1 + a2 + a3 + a4;
		//printf( " %i %i   %i %i %i %i \n", i, ii-3,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " way 2 i++    : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ), sum );


	sum = 0;
	t1 =  rdtsc();
	struct int4{	int a1,a2,a3,a4;	};
	int4* int4s = ( int4* )&ints[0];
	for (int i=0; i<ntimes; i++  ){
		int ii    = hash( i ) & mask;
		int4 u  = int4s[ ii ];
		sum += u.a1 + u.a2 + u.a3 + u.a4;
		//printf( " %i %i   %i %i %i %i \n", i, ii,  u.a1, u.a2, u.a3, u.a4 );
	}  
	t2 =  rdtsc();
	printf( " way 3 struct : %f ticks/op | %g \n", ( ((double)(t2-t1))/ntimes ),  sum );

}
