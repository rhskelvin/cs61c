#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h> // We have already imported the SEE Intrinsics for you

/* 
* the definition of array_size is after its first use,
* so we declare the function here
* Do not change the line below.
*/
int array_size(char s[]);  

/*
 * rotates string s (given size of char array, INCLUDING the null terminator) by n places
 * edits the s character array formed by pushing leftmost characters to the right; only alphanumeric characters rotate.
 * the result is saved in the local variable new_array, then printed.
 *
 * EX: rotate("hello", 1) --> "elloh"
 * EX: rotate("hello", 2) --> "llohe"
 * EX: rotate("hello", 5) --> "hello"
 *
 * Please SIMD-ize this standard implementation of rotate_vectorized!! 
 */
void rotate_vectorized(char s[], int n) {
    int size = array_size(s);
    char new_array[size];
    /* YOUR CODE HERE */
    int i;
    int pos = 0;
    
    if (size - 1 != 0) {
        for (i = n % (size - 1); i < (size - 1); i++) {
            new_array[pos] = s[i];
            pos++;
        }

        for (i = 0; i < n % (size - 1); i++) {
            new_array[pos] = s[i];
            pos++;
        }
    }
    
    new_array[size-1] = '\0';

    /* DO NOT CHANGE CODE BELOW THIS LINE */
    printf("%s\n", new_array);
}

/* DO NOT CHANGE CODE BELOW THIS LINE */
void rotate_reference(char s[], int n) {
    int size = array_size(s);
    char new_array[size];
    int i;
    int pos = 0;
    
    if (size - 1 != 0) {
        for (i = n % (size - 1); i < (size - 1); i++) {
            new_array[pos] = s[i];
            pos++;
        }

        for (i = 0; i < n % (size - 1); i++) {
            new_array[pos] = s[i];
            pos++;
        }
    }
    
    new_array[size-1] = '\0';

    printf("%s\n", new_array);
}

/*
 * returns the size of the char array s, including the null terminator
 * you should NOT need to modify this
 */
int array_size(char s[]) {
    int total = 0;
    int index = 0;
    while (s[index] != '\0') {
        total++;
        index++;
    }
    total++;
    return total;
}

/*
 * main function for testing your code
 */
int main(int argc, char* argv[]) {
    if (argc == 3) {
        rotate_vectorized(argv[1], atoi(argv[2]));
    } else if (argc != 1) {
        printf("incorrect number of args!\n");
    } else {
        char* s1 = "Roses are red, Violets are blue, this is a long string, I hope you rotate it correctly. ";
        char* s2 = "abcdefghijklmnopqrstuvwxyz1234567890 0987654321zyxwvutsrqponmlkjihgfedcba";
        char* s3 = "What is love! Baby don't hurt me, don't hurt me, no mo'. ";

        printf("Should be, followed by what you got: \n");
        rotate_reference("applesauce", 5);
        rotate_vectorized("applesauce", 5);
        
        printf("Should be, followed by what you got: \n");
        rotate_reference("coolbeans", 4);
        rotate_vectorized("coolbeans", 4);
        
        printf("Should be, followed by what you got: \n");
        rotate_reference("CS61c", 2);
        rotate_vectorized("CS61c", 2);
        
        printf("Should be, followed by what you got: \n");
        rotate_reference("CS61c", 5);
        rotate_vectorized("CS61c", 5);
        
        printf("Should be, followed by what you got: \n");
        rotate_reference(s1, 33);
        rotate_vectorized(s1, 33);
        
        printf("Should be, followed by what you got: \n");
        rotate_reference(s2, 37);
        rotate_vectorized(s2, 37);
        
        printf("Should be, followed by what you got: \n");
        rotate_reference(s3, 14);
        rotate_vectorized(s3, 14);
    }
    return 0;
}


