// HANYU Zhang CS61C-AK
// YUANHAO Yang CS61C-AL
#include <emmintrin.h>
#include <omp.h>
#include <stdlib.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{   
    omp_set_num_threads(16);
    float t[9];
    kernel=memcpy(t, kernel,36);
    #pragma omp parallel shared(kernel, out, in)
    {
        #pragma omp for
        for (int y = 1; y < data_size_Y-1; y++){
            int lastColumn = data_size_X-1;
            float first = 0, last = 0;
            for(int rowNum = -1; rowNum <= 1; rowNum++){
                float *firsttemp =  (y+rowNum)*data_size_X+in, *lasttemp =  (y+rowNum)*data_size_X+lastColumn+in;
                int kerPositionTemp =(1-rowNum)*3;
                for(int col = -1; col <= 1; col++){
                    float *kernelPosition= (1-col)+kerPositionTemp+kernel;
                    if(y+rowNum!=data_size_Y&&y+rowNum!=-1 ){
                        if(col!=-1 && col!=data_size_X) first += *(kernelPosition) * *(firsttemp+col);                    
                        if(lastColumn+col!=-1  && lastColumn+col!=data_size_X) last += *(kernelPosition) * *(lasttemp+col);
                    }
                }
            }
            int row = y*data_size_X;
            out[row] = first;
            out[lastColumn+row] = last;
        }
    }


    #pragma omp parallel shared(kernel, out, in)
    {
        #pragma omp for
        for (int x = 0;x<data_size_X;x++){
            int  lastLineNum=data_size_Y-1;
            float first = 0, last = 0;
            for(int j = -1; j <= 1; j++){

                int firstLineTemp = j*data_size_X+x,
                    secondLineTemp = (lastLineNum+j)*data_size_X+x,
                    kerPositionTemp =(1-j)*3;

                for(int i = -1; i <= 1; i++){
                    int kernelPosition= (1-i)+kerPositionTemp;
                    if(x+i>-1 && x+i<data_size_X ){
                        if(j!=-1 && j!=data_size_Y) first += kernel[kernelPosition] * in[i + firstLineTemp];                    
                        if(lastLineNum+j!=-1 && lastLineNum+j!=data_size_Y) last  += kernel[kernelPosition] * in[i + secondLineTemp];
                    }
                }
            }
            out[x] = first;
            out[x+lastLineNum*data_size_X] = last;
        }
    }  

    float tt[9];
    kernel=memcpy(tt, kernel,36);

omp_set_num_threads(16);
#pragma  omp parallel 
{
    #pragma omp for 
    for (int y = 1; y < data_size_Y-1; y++){   
        int row = y*data_size_X, x = 1;
        for (; x < (data_size_X-2)/32*32; x+=32){
            __m128 vector1 = _mm_set1_ps(0.0f);
            __m128 vector2=  vector1;
            __m128 vector3 = vector1;
            __m128 vector4 = vector1;
            __m128 vector5 = vector1;
            __m128 vector6 = vector1;
            __m128 vector7 = vector1;
            __m128 vector8 = vector1;
            for (int row = -1; row < 2; row++){
                int temp = (y+row)*data_size_X;
                int kerPositionTemp = (1-row)*3;
                for (int col = -1; col < 2; col++){                 
                    __m128 kerVal = _mm_set1_ps(kernel[(1-col)+kerPositionTemp]);
                    vector1 = _mm_add_ps (vector1, _mm_mul_ps(_mm_loadu_ps(in+x+col+temp),kerVal));
                    vector2 = _mm_add_ps (vector2, _mm_mul_ps(_mm_loadu_ps(in+x+col+temp+4),kerVal));
                    vector3 = _mm_add_ps (vector3, _mm_mul_ps(_mm_loadu_ps(in+x+col+temp+8),kerVal));
                    vector4 = _mm_add_ps (vector4, _mm_mul_ps(_mm_loadu_ps(in+x+col+temp+12),kerVal));
                    vector5 = _mm_add_ps (vector5, _mm_mul_ps(_mm_loadu_ps(in+x+col+temp+16),kerVal));
                    vector6 = _mm_add_ps (vector6, _mm_mul_ps(_mm_loadu_ps(in+x+col+temp+20),kerVal));
                    vector7 = _mm_add_ps (vector7, _mm_mul_ps(_mm_loadu_ps(in+x+col+temp+24),kerVal));
                    vector8 = _mm_add_ps (vector8, _mm_mul_ps(_mm_loadu_ps(in+x+col+temp+28),kerVal));
                }

            }
            int ot = x+row;
            _mm_storeu_ps(out+ot, vector1);
            _mm_storeu_ps(out+ot+4, vector2);
            _mm_storeu_ps(out+ot+8, vector3);
            _mm_storeu_ps(out+ot+12, vector4);
            _mm_storeu_ps(out+ot+16, vector5);
            _mm_storeu_ps(out+ot+20, vector6);
            _mm_storeu_ps(out+ot+24, vector7);
            _mm_storeu_ps(out+ot+28, vector8);

        }
        for (;x<data_size_X-1; x++){
                for(int i = -1; i <= 1; i++){
                    for(int j = -1; j <= 1; j++){
                        out[x+y*data_size_X] +=
                        kernel[(1-i)+(1-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
                    }
                }
            }
        }
}

   


    return 1;
}

