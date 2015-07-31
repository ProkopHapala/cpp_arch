

/*

pocitac v praci:
cat /proc/cpuinfo: Intel(R) Core(TM) i5 CPU  750  @ 2.67GHz	cache size	: 8192 KB


 ========= TESTING : 100000000 8 256 255 
 ground sum+=i :  4.033144 ticks/op | 1.71799e+10 
 one [ i+1 ]   :  3.935357 ticks/op | 1.35795e+06 
 way 1 [ i+1 ] :  6.888157 ticks/op | 2.76434e+06 
 way 2 i++     :  8.177008 ticks/op | 2.76434e+06 
 way 3 struct  :  5.328588 ticks/op | 2.76434e+06 
 ========= TESTING : 100000000 16 65536 65535 
 ground sum+=i :  3.918629 ticks/op | 4.39805e+12 
 one [ i+1 ]   :  6.869366 ticks/op | -13365.5 
 way 1 [ i+1 ] : 11.897319 ticks/op | 3347.87 
 way 2 i++     : 12.153224 ticks/op | 3347.87 
 way 3 struct  : 11.767395 ticks/op | 3347.87 
 ========= TESTING : 100000000 24 16777216 16777215 
 ground sum+=i :  3.848060 ticks/op | 1.1259e+15 
 one [ i+1 ]   : 53.518402 ticks/op | -9079.78 
 way 1 [ i+1 ] : 59.079880 ticks/op | 7742.47 
 way 2 i++     : 58.917152 ticks/op | 7742.47 
 way 3 struct  : 58.509201 ticks/op | 7742.47 
 ========= TESTING : 100000000 26 67108864 67108863 
 ground sum+=i :  3.897057 ticks/op | 4.5036e+15 
 one [ i+1 ]   : 66.976791 ticks/op | -6009.48 
 way 1 [ i+1 ] : 85.274811 ticks/op | -1844.3 
 way 2 i++     : 85.012871 ticks/op | -1844.3 
 way 3 struct  : 84.857529 ticks/op | -1844.3



pocitac na koleji:
model name      : Intel(R) Core(TM)2 Quad  CPU   Q9450  @ 2.66GHz

 ========= TESTING : 100000000 8 256 255                                                                                                   
 ground sum+=i  : 5.432511 ticks/op | 1.71799e+10                                                                                           
 one [ i+1 ]    : 4.531024 ticks/op | 1.35795e+06 
 way 1 [ i+1 ]  : 6.307909 ticks/op | 2.76434e+06 
 way 2 i++      : 6.740351 ticks/op | 2.76434e+06 
 way 3 struct   : 6.360709 ticks/op | 2.76434e+06
 ========= TESTING : 100000000 16 65536 65535 
 ground sum+=i  : 5.125530 ticks/op | 4.39805e+12 
 one [ i+1 ]    : 4.304242 ticks/op | -13365.5 
 way 1 [ i+1 ]  : 9.265764 ticks/op | 3347.87 
 way 2 i++      : 9.299588 ticks/op | 3347.87 
 way 3 struct   : 9.264762 ticks/op | 3347.87
 ========= TESTING : 100000000 24 16777216 16777215 
 ground sum+=i  :  4.739793 ticks/op | 1.1259e+15 
 one [ i+1 ]    : 41.120914 ticks/op | -9079.78 
 way 1 [ i+1 ]  : 51.914364 ticks/op | 7742.47 
 way 2 i++      : 57.399399 ticks/op | 7742.47 
 way 3 struct   : 58.922466 ticks/op | 7742.47
 ========= TESTING : 100000000 26 67108864 67108863 
 ground sum+=i  :  4.513832 ticks/op | 4.5036e+15 
 one [ i+1 ]    : 40.534943 ticks/op | -6009.48 
 way 1 [ i+1 ]  : 57.964325 ticks/op | -1844.3 
 way 2 i++      : 62.446285 ticks/op | -1844.3 
 way 3 struct   : 58.958828 ticks/op | -1844.3 





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

const uint8_t npower = 26;

const uint32_t nsamples = ( 1 << npower );
const uint32_t mask     = nsamples-1;

const int nsamptot = 4*nsamples+3;
float * floats   = new float[ nsamptot];

int main( int argc, char* args[] ){

	long  t1, t2;
	float sum = 0;

	printf( " ========= GENERATE SAMPLES : %i %i %i \n", npower, nsamples, mask );

	sum = 0;
	for( int i=0; i< nsamptot; i++ ){
		floats[i] = randf()-0.5d; 
		sum += floats[i];
		//printf( " %i %f \n", i, floats[i] );
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
	printf( " ground sum+=i : %f ticks/op | %g \n", ( ((float)(t2-t1))/ntimes ), sum );
	

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		float a1 = floats[ ii   ]; 
		sum += a1;
		//printf( " %i %i %f %f %f %f \n", i, ii,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " one [ i+1 ]   : %f ticks/op | %g \n", ( ((float)(t2-t1))/ntimes ),  sum );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		float a1 = floats[ ii   ]; 
		float a2 = floats[ ii+1 ]; 
		float a3 = floats[ ii+2 ]; 
		float a4 = floats[ ii+3 ]; 
		sum += a1 + a2 + a3 + a4;
		//printf( " %i %i %f %f %f %f \n", i, ii,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " way 1 [ i+1 ]  : %f ticks/op | %g \n", ( ((float)(t2-t1))/ntimes ),  sum );

	sum = 0;
	t1 =  rdtsc();
	for (int i=0; i<ntimes; i++  ){
		int ii    = (hash( i ) & mask )<<2;
		float a1 = floats[ ii ]; ii++;
		float a2 = floats[ ii ]; ii++;
		float a3 = floats[ ii ]; ii++;
		float a4 = floats[ ii ];
		sum += a1 + a2 + a3 + a4;
		//printf( " %i %i %f %f %f %f \n", i, ii-3,  a1, a2, a3, a4 );
	}  
	t2 =  rdtsc();
	printf( " way 2 i++    : %f ticks/op | %g \n", ( ((float)(t2-t1))/ntimes ), sum );


	sum = 0;
	t1 =  rdtsc();
	struct float4{	float a1,a2,a3,a4;	};
	float4* float4s = ( float4* )&floats[0];
	for (int i=0; i<ntimes; i++  ){
		int ii    = hash( i ) & mask;
		float4 u  = float4s[ ii ];
		sum += u.a1 + u.a2 + u.a3 + u.a4;
		//printf( " %i %i %f %f %f %f \n", i, ii,  u.a1, u.a2, u.a3, u.a4 );
	}  
	t2 =  rdtsc();
	printf( " way 3 struct : %f ticks/op | %g \n", ( ((float)(t2-t1))/ntimes ),  sum );

}
