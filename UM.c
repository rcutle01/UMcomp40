/*
 * UM.c
 *      the main for the universal machine
 *      reads in a file from the command line,
 *      calls functions in segments.h and unpack.h to:
 *              store the instructions (32 bit words) in a memory struct,
 *              loop through the instructions performing each desired operation
 *
 * Written by: Nathan Majumder (nmajum01) & Becky Cutler (rcutle01)
 * Date: 8 April 2015
 *
 */

#include <stdlib.h>
#include "unpack.h"

int main(int argc, char *argv[])
{
        /* the .um file with the instructions must be the second argument
           on the command line */
        if(argc != 2) {
                printf("Incorrect input\n");
                exit(1);
        }

        FILE *input = fopen(argv[1], "rb");
        if (input == NULL) {
                printf("Could not open file\n");
                exit(1);
        }

        UM_memory mem = initialize_memory();
        load_instructions(mem, input);

        fclose(input);
        unpack_instructions(mem);
       
        return 0;
}
