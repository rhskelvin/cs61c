#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h> /* where intrinsics are defined */

#define CLOCK_RATE_GHZ 2.26e9

/* Time stamp counter from Lecture 2/17 */
static __inline__ unsigned long long RDTSC(void) {
    unsigned hi,lo;
    __asm__ volatile("rdtsc" : "=a"(lo),"=d"(hi));
    return ((unsigned long long) lo)| (((unsigned long long)hi) << 32);
}

int sum_naive( int n, int *a )
{
    int sum = 0;
    for( int i = 0; i < n; i++ )
        sum += a[i];
    return sum;
}

int sum_unrolled( int n, int *a )
{
    int sum = 0;

    /* do the body of the work in a faster unrolled loop */
    for( int i = 0; i < n/4*4; i += 4 )
    {
        sum += a[i+0];
        sum += a[i+1];
        sum += a[i+2];
        sum += a[i+3];
    }

    /* handle the small tail in a usual way */
    for( int i = n/4*4; i < n; i++ )   
        sum += a[i];

    return sum;
}

int sum_vectorized( int n, int *a )
{
    /* WRITE YOUR VECTORIZED CODE HERE */
        __m128i sumi=_mm_setzero_si128(); /* c1 = (C[0],C[1]) */
	    int sum = 0, i = 0;
    for(i = 0; i < n-4;i+=4 )
         sumi=_mm_add_epi32(_mm_loadu_si128( a+i ),sumi);           
    for(;i<n;i++) sum+=a[i];
 
    int result[4];
    _mm_storeu_si128(result, sumi);
    for(i=0;i<4;i++) sum+=result[i];
    return sum;
}

int sum_vectorized_unrolled( int n, int *a )
{
    /* UNROLL YOUR VECTORIZED CODE HERE*/
    
	 __m128i sumi=_mm_setzero_si128(); /* c1 = (C[0],C[1]) */
        int sum = 0, i = 0;
    for(i = 0; i < n-16;i+=16 ){
         sumi=_mm_add_epi32(_mm_loadu_si128( a+i ),sumi);           
         sumi=_mm_add_epi32(_mm_loadu_si128( a+i+4 ),sumi);           
         sumi=_mm_add_epi32(_mm_loadu_si128( a+i+8 ),sumi);           
         sumi=_mm_add_epi32(_mm_loadu_si128( a+i+12 ),sumi);           
    }
    for(;i<n;i++) sum+=a[i];

 
    int result[4];
    _mm_storeu_si128(result, sumi);
    for(i=0;i<4;i++) sum+=result[i];
    return sum;
	
	
}

void benchmark( int n, int *a, int (*computeSum)(int,int*), char *name )
{
    /* warm up */
    int sum = computeSum( n, a );

    /* measure */
    unsigned long long cycles = RDTSC();
    sum += computeSum( n, a );
    cycles = RDTSC()-cycles;
    
    double microseconds = cycles/CLOCK_RATE_GHZ*1e6;
    
    /* report */
    printf( "%20s: ", name );
    if( sum == 2*sum_naive(n,a) ) printf( "%.2f microseconds\n", microseconds );
    else	                  printf( "ERROR!\n" );
}

int main( int argc, char **argv )
{
    const int n = //2000;
	7777; /* small enough to fit in cache */
    
    /* init the array */
    srand48( time( NULL ) );
    int a[n] __attribute__ ((aligned (16))); /* align the array in memory by 16 bytes */
    for( int i = 0; i < n; i++ ) a[i] = lrand48( );
    
    /* benchmark series of codes */
    benchmark( n, a, sum_naive, "naive" );
    benchmark( n, a, sum_unrolled, "unrolled" );
    benchmark( n, a, sum_vectorized, "vectorized" );
    benchmark( n, a, sum_vectorized_unrolled, "vectorized unrolled" );

    return 0;
}

