#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
#include <emmintrin.h> /* where intrinsics are defined */
#define MIN(a,b) (((a)<(b))?(a):(b))

int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
           float* kernel)
{
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;
    int newX = data_size_X+2;
    int newY = data_size_Y+2;
    int temp_size = newX*newY;
    float temp[temp_size];
    for (int i = 0; i < temp_size; i++) temp[i] = 0;
    for (int y = 0; y < data_size_Y; y++) {
        for (int x = 0; x < data_size_X; x++) 
            temp[(x+kern_cent_X)+(y+kern_cent_Y)*newX] = in[x+y*data_size_X];
        
    }
    float copy[KERNX*KERNY];
    for (int i = 0; i < KERNX*KERNY; i++) {
        copy[i] = kernel[i];
    }
    kernel = copy;//put kernel on cache

int a=0,b=0, b_x=240, b_y=240;

for(a=0; a< data_size_X; a+=b_x){
    for(int y = b; y < MIN(data_size_Y, b+b_y); y++){
            for(int x = a; x < MIN(data_size_X, a+b_x); x++){
                int k  = x+y*data_size_X;

                out[k] += kernel[8] * temp[x + y*newX]; // j = -1
                out[k] += kernel[5] * temp[x + (y+1)*newX]; // j  = 0
                out[k] += kernel[2] * temp[x + (y+2)*newX]; // j = 1
                // i = 0
                x++;
                out[k] += kernel[7] * temp[x + (y)*newX];
                out[k] += kernel[4] * temp[x + (y+1)*newX];
                out[k] += kernel[1] * temp[x + (y+2)*newX];
                // i = 1
                x++;
                out[k] += kernel[6] * temp[x + (y)*newX];
                out[k] += kernel[3] * temp[x + (y+1)*newX];
                out[k] += kernel[0] * temp[x + (y+2)*newX];

                x-=2;

            }
        }
    }
    

    return 1;
    
}
