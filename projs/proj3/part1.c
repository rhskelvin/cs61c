#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
#include <emmintrin.h> /* where intrinsics are defined */
#define MIN(a,b) (((a)<(b))?(a):(b))
#include <stdlib.h>
#include <omp.h>

int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
           float* kernel)
{
    // #pragma omp parallel
    // {
    int kern_cent_X = (KERNX - 1)/2;
    int kern_cent_Y = (KERNY - 1)/2;
    int X_large = data_size_X+2 ,Y_large = data_size_Y+2;
    float temp[X_large*Y_large];
    memset(temp,0,sizeof(temp));
    // #pragma parallel for
    for (int y = 0; y < data_size_Y; y++) {
        for (int x = 0; x < data_size_X; x++) 
            temp[(x+kern_cent_X)+(y+kern_cent_Y)*X_large] = in[x+y*data_size_X];        
    }
    float t[KERNX*KERNY];
    kernel=memcpy(t, kernel,sizeof(t));

    int a=0,b=0, b_x=1000;
    for(a=0; a< data_size_X; a+=b_x){
        for(int y = 0; y < data_size_Y; y++){
                for(int x = a; x < MIN(data_size_X, a+b_x); x++){
                    int k  = x+y*data_size_X;
                    out[k] += kernel[8] * *(temp+x + y*X_large); // j = -1
                    out[k] += kernel[5] * *(temp+x + (y+1)*X_large); // j  = 0
                    out[k] += kernel[2] * *(temp+x + (y+2)*X_large); // j = 1
                    // i = 0
                    x++;
                    out[k] += kernel[7] * *(temp+x + y*X_large);
                    out[k] += kernel[4] * *(temp+x + (y+1)*X_large);
                    out[k] += kernel[1] * *(temp+x + (y+2)*X_large);
                    // i = 1
                    x++;
                    out[k] += kernel[6] * *(temp+x + y*X_large);
                    out[k] += kernel[3] * *(temp+x + (y+1)*X_large);
                    out[k] += kernel[0] * *(temp+x + (y+2)*X_large);
                    x-=2;
                }
            }
        }
    }
    return 1;
}
