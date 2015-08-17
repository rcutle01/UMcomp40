/*
 * unpack.c
 *      the implementation for the unpack module of the UM
 *      functions loop through the instructions in the UM_memory,
 *      unpack the words into their components, and call functions
 *      to perform the appropriate operation
 *
 * Written by: Nathan Majumder (nmajum01) & Becky Cutler (rcutle01)
 * Date: 8 April 2015
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "unpack.h"
#include "bitpack.h"
#include "operations.h"

#define REGISTER_WIDTH 3
#define OPCODE_WIDTH 4
#define LOAD_VAL_WID 25
#define OPCODE_LSB 28

/****** private helper function declarations ******/

/* calls the appropriate function in the operations module and passes it the
 * appropriate parameters
 */
void determine_operation(UM_memory mem, uint32_t opcode, uint32_t *registers,
                         uint32_t a, uint32_t b, uint32_t c);
/**************************************************/

/* initializes the registers array, and
 * while there are more instructions to read in the UM_memory, gets an
 * instruction, unpacks it, and calls a function to perform the operation
 */
extern void unpack_instructions(UM_memory mem)
{
        /* create and initialize registers */
        uint32_t registers[8] = {0,0,0,0,0,0,0,0};
        while (done_with_instructions(mem) == 0) {
                uint32_t word = get_instruction(mem);
                uint32_t opcode = Bitpack_getu((uint64_t) word, OPCODE_WIDTH,
                                               OPCODE_LSB);
                /* checking for special case of load_value */
                if (opcode == 13) {
                        /* unpacking the register/value for load value */
                        uint32_t a = Bitpack_getu((uint64_t) word, 
                                                  REGISTER_WIDTH,
                                                  LOAD_VAL_WID);
                        uint32_t val = Bitpack_getu((uint64_t) word,
                                                    LOAD_VAL_WID, 0);
                        load_value(registers, a, val);
                } else {
                        /* unpack the registers needed (lowest order 9 bits) */
                        uint32_t c = Bitpack_getu((uint64_t) word,
                                                  REGISTER_WIDTH, 0);
                        uint32_t b = Bitpack_getu((uint64_t) word,
                                                  REGISTER_WIDTH,
                                                  REGISTER_WIDTH * 1);
                        uint32_t a = Bitpack_getu((uint64_t) word,
                                                  REGISTER_WIDTH,
                                                  REGISTER_WIDTH * 2);

                        determine_operation(mem, opcode, registers, a, b, c);
                 }
        }
        free_memory(mem);
}

/****** private helper function definitions ******/

/* determines which function in the operations module to call given the opcode
 * checks for all opcodes 0-12 -- opcode 13 was previously checked
 * if opcode is invalid (14 or 15), the program exits
 */
void determine_operation(UM_memory mem, uint32_t opcode, uint32_t *registers,
                         uint32_t a, uint32_t b, uint32_t c)
{
        switch (opcode) {
                case 0:
                        move(registers, a, b, c);
                        break;
                case 1:
                        load_from_memory(mem, registers, a, b, c);
                        break;
                case 2:
                        store_in_memory(mem, registers, a, b, c);
                        break;
                case 3:
                        add(registers, a, b, c);
                        break;
                case 4:
                        multiply(registers, a, b, c);
                        break;
                case 5:
                        divide(registers, a, b, c);
                        break;
                case 6:
                        nand(registers, a, b, c);
                        break;
                case 7:
                        halt(mem);
                case 8:
                        map(mem, registers, b, c);
                        break;
                case 9:
                        unmap(mem, registers, c);
                        break;
                case 10:
                        output(registers, c);
                        break;
                case 11:
                        input(registers, c);
                        break;
                case 12:
                        load_program(mem, registers, b, c);
                        break;
                default:
                        /* op code must be 14 or 15 which is invalid so we must
                         * free memory and quit the program
                         */
                        free_memory(mem);
                        exit(1);
        }
}

