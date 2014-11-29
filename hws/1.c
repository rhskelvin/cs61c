#include <stdio.h>
#include "contest.h"
#include <math.h>
#include <stdlib.h>
int main(int argc, const char * argv[])
{


    int sum = 0, i = 0;
    int * numlist = calloc(argc - 1, sizeof(int));
    float * distList = calloc(argc - 1, sizeof(float));
    for(i=0;i<argc-1;i++) {
        int num = atoi(argv[i+1]);
        *(numlist+i) = num;
        *(distList+i)= (float)num;
        sum += num;
    }
    float average = sum / (argc - 1.0);
    float minDist = *(distList);
    for (i = 0; i < argc - 1; i++) {
        *(distList+i) = fabsf( *(distList+i)-average);
        minDist = fmin(minDist,*(distList+i));
    }
    for( i = 0; i < argc -1; i++){
        if(fabsf(minDist - *(distList+i))<0.0000000001) printf("%d", *(numlist+i));
    }


    return 0;
}


