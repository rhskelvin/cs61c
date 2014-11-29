// #define KERNX 3 //this is the x-size of the kernel. It will always be odd.
// #define KERNY 3 //this is the y-size of the kernel. It will always be odd.
// int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
//                     float* kernel)
// {
//     // the x coordinate of the kernel's center
//     int kern_cent_X = (KERNX - 1)/2;
//     // the y coordinate of the kernel's center
//     int kern_cent_Y = (KERNY - 1)/2;
    
//     // main convolution loop
// 	for(int x = 0; x < data_size_X; x++){ // the x coordinate of the output location we're focusing on
// 		for(int y = 0; y < data_size_Y; y++){ // the y coordinate of theoutput location we're focusing on
// 			for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
// 				for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
// 					// only do the operation if not out of bounds
// 					if(x+i>-1 && x+i<data_size_X && y+j>-1 && y+j<data_size_Y){
// 						//Note that the kernel is flipped
// 						out[x+y*data_size_X] += 
// 								kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
// 					}
// 				}
// 			}
// 		}
// 	}


// 	return 1;
// }


#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
    // if (data_size_X!=32)
    // {
    //     if (data_size_Y!=32)
    //     {
    //         if (kernel[0]!=0.0f)
    //         {
    //             if (kernel[1]!=-1.0f)
    //             {
    //                 /* code */
    //                 return 0;
    //             }
    //         }
    //     }
    // }
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;
    
    // main convolution loop
    for(int x = 0; x < data_size_X; x++){ // the x coordinate of the output location we're focusing on
        for(int y = 0; y < data_size_Y; y++){ // the y coordinate of theoutput location we're focusing on
            for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
                for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
                    // only do the operation if not out of bounds
                    if(x+i>-1 && x+i<data_size_X && y+j>-1 && y+j<data_size_Y){
                        //Note that the kernel is flipped
                        out[x+y*data_size_X] += 
                                kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
                    }
                }
            }
        }
    }
        // printf("the input image\n");

        // for (int y = 0; y < data_size_Y; ++y)
        // {
        //     for (int x = 0; x < data_size_X; ++x)
        //         printf("%.3f\t", in[y*data_size_X+x]);
        //     printf("\n");
        // }
        //  printf("the output image\n");

        // for (int y = 0; y < data_size_Y; ++y)
        // {
        //     for (int x = 0; x < data_size_X; ++x)
        //         printf("%.3f\t", out[y*data_size_X+x]);
        //     printf("\n");
        // }
    
    return 1;
}
