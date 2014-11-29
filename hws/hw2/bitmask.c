#include <stdio.h>

unsigned int set_bit(unsigned int number, int value, int position) {
    /* YOUR CODE HERE */

    int get_bit(unsigned int, int); // declare function get_bit because it is used in set_bit

    int d = 1;
    for(int i = 0; i < position;i++) d*=2; // compute 2 to the power of position and store it in d
    
    if(number < d){
        if(value == 1) return number + d;
        if(value == 0) return number;
    }
    if(number >= d){
        if(get_bit(number, position) == value) return number; // set 0 to 0 or set 1 to 1
        if(get_bit(number, position)  < value) return number + d; // set 0 to 1
        else return number - d;                                // set 1 to 0
    }

    return 0;
}

int get_bit(unsigned int number, int position) {
    /* YOUR CODE HERE */
    int d = 1;
    for(int i = 0; i < position; i++) d*=2; // d == 2**position
    return (number / d) % 2;
}

int main(void) {
    unsigned int num = 0;
    printf("num is currently 0x%X.\n", num);
    printf("Setting the 3rd bit to 1. num should read 0x8.\n");
    num = set_bit(num, 1, 3);
    printf("num is currently 0x%X.\n", num);
    printf("Setting the 7th bit to 1. num should read 0x88.\n");
    num = set_bit(num, 1, 7);
    printf("num is currently 0x%X.\n", num);
    printf("Setting the 3rd bit to 0. num should read 0x80.\n");
    num = set_bit(num, 0, 3);
    printf("num is currently 0x%X.\n", num);
    printf("Getting the value of the 4th bit, which should be 0.\n");
    printf("value of the 4th bit is %d.\n", get_bit(num, 4));
    printf("Getting the value of the 7th bit, which should be 1.\n");
    printf("value of the 7th bit is %d.\n", get_bit(num, 7));
    return 0;
}

