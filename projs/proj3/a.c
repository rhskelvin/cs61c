#include <emmintrin.h> /* where intrinsics are defined */
int main(){
    float a[4] ={1.0f, 2.0f,3.0f,4.0f};
    float b[4] = {5.0f,6.0f,7.0f,8.0f};
    float r[4];
    __m128 aa = _mm_loadu_ps(a);
    __m128 bb = _mm_loadu_ps(b);
    __m128 rr = _mm_mul_ps(aa,bb);
    _mm_storeu_ps( r+0, rr );
    int i = 0;
    for(i = 0; i < 4;i++) printf("%f ", r[i]);
    return 0;
}