#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main(){

        uint64_t a = 0x0000000000001111;
        uint32_t b = (uint32_t) a;
        if(b==0x00001111)
           printf("lolo"); 



        return 0;
}
