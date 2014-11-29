#include <emmintrin.h>
#include <omp.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;
   


    
//    //middle
    omp_set_num_threads(16);
    #pragma omp parallel
    {
        #pragma omp for
        for(int y = 1; y < data_size_Y-1; y++){
            int row = y*data_size_X;
            int x=1;
            for(; x < (data_size_X-2)/28*28+1; x+=28){
                int outPos = x+row;
                __m128 outResult1 = _mm_set1_ps(0);
                __m128 outResult2 = _mm_set1_ps(0);
                __m128 outResult3 = _mm_set1_ps(0);
                __m128 outResult4 = _mm_set1_ps(0);
                __m128 outResult5 = _mm_set1_ps(0);
                __m128 outResult6 = _mm_set1_ps(0);
                __m128 outResult7 = _mm_set1_ps(0);
                for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
                    int temp = (y+j)*data_size_X;
                    int kerTemp =(kern_cent_Y-j)*KERNX;
                    for(int i = -kern_cent_X; i <= kern_cent_X; i++){
                        int kerPos = (kern_cent_X-i)+kerTemp;
                        __m128 kerVal = _mm_set1_ps(kernel[kerPos]);
                        __m128 inVal1 = _mm_loadu_ps(in+(x+i)+temp);
                        __m128 inVal2 = _mm_loadu_ps(in+(x+i)+temp+4);
                        __m128 inVal3 = _mm_loadu_ps(in+(x+i)+temp+8);
                        __m128 inVal4 = _mm_loadu_ps(in+(x+i)+temp+12);
                        __m128 inVal5 = _mm_loadu_ps(in+(x+i)+temp+16);
                        __m128 inVal6 = _mm_loadu_ps(in+(x+i)+temp+20);
                        __m128 inVal7 = _mm_loadu_ps(in+(x+i)+temp+24);
                        outResult1 =  _mm_add_ps(_mm_mul_ps(inVal1,kerVal),outResult1);
                        outResult2 =  _mm_add_ps(_mm_mul_ps(inVal2,kerVal),outResult2);
                        outResult3 =  _mm_add_ps(_mm_mul_ps(inVal3,kerVal),outResult3);
                        outResult4 =  _mm_add_ps(_mm_mul_ps(inVal4,kerVal),outResult4);
                        outResult5 =  _mm_add_ps(_mm_mul_ps(inVal5,kerVal),outResult5);
                        outResult6 =  _mm_add_ps(_mm_mul_ps(inVal6,kerVal),outResult6);
                        outResult7 =  _mm_add_ps(_mm_mul_ps(inVal7,kerVal),outResult7);
                    }
                }
                _mm_storeu_ps(out+outPos, outResult1);
                _mm_storeu_ps(out+outPos+4, outResult2);
                _mm_storeu_ps(out+outPos+8, outResult3);
                _mm_storeu_ps(out+outPos+12, outResult4);
                _mm_storeu_ps(out+outPos+16, outResult5);
                _mm_storeu_ps(out+outPos+20, outResult6);
                _mm_storeu_ps(out+outPos+24, outResult7);
            }
            for (;x<data_size_X-1; x++){
                for(int i = -kern_cent_X; i <= kern_cent_X; i++){
                    for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
                        out[x+y*data_size_X] +=
                        kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
                    }
                }
            }
        }
    }
    //outer cycle
    omp_set_num_threads(16);
    #pragma omp parallel
    {
        #pragma omp for
        for (int x = 0;x<data_size_X;x++){
            int y1 = 0, y2=data_size_Y-1;
            float result1 = 0;
            float result2 = 0;
            for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
                int temp1 = (j)*data_size_X;
                int temp2 = (y2+j)*data_size_X;
                int kerTemp =(kern_cent_Y-j)*KERNX;
                for(int i = -kern_cent_X; i <= kern_cent_X; i++){
                    int kerPos = (kern_cent_X-i)+kerTemp;
                    if(x+i>-1 && x+i<data_size_X && y1+j>-1 && y1+j<data_size_Y){
                        result1 += kernel[kerPos] * in[(x+i) + temp1];
                    }
                    if(x+i>-1 && x+i<data_size_X && y2+j>-1 && y2+j<data_size_Y){
                        result2 += kernel[kerPos] * in[(x+i) + temp2];
                    }
                }
            }
            out[x] = result1;
            out[x+y2*data_size_X] = result2;
        }
    }

    omp_set_num_threads(16);
    #pragma omp parallel
    {
        #pragma omp for
        for (int y = 1; y < data_size_Y -1; y++){
            int x1 = 0, x2 = data_size_X-1;
            int row = y*data_size_X;
            int outPos1 = x1+row, outPos2 = x2+row;
            float result1 = 0;
            float result2 = 0;
            for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
                int temp = (y+j)*data_size_X;
                int kerTemp =(kern_cent_Y-j)*KERNX;
                for(int i = -kern_cent_X; i <= kern_cent_X; i++){
                    int kerPos = (kern_cent_X-i)+kerTemp;
                    if(x1+i>-1 && x1+i<data_size_X && y+j>-1 && y+j<data_size_Y){
                        result1 += kernel[kerPos] * in[(x1+i) + temp];
                    }
                    if(x2+i>-1 && x2+i<data_size_X && y+j>-1 && y+j<data_size_Y){
                        result2 += kernel[kerPos] * in[(x2+i) + temp];
                    }
                }
            }
            out[outPos1] = result1;
            out[outPos2] = result2;
        }
    }
    
    return 1;
}
