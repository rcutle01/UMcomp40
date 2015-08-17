/*
 *
 *      operations.c
 *      The implementation for the operations.h file
 *      Written by: Becky Cutler and Nathan Majumder
 *      3/25/15
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "seq.h"
#include "stack.h"
#include "operations.h"
#include "segments.h"
#include <inttypes.h>

/*if registers[c]!= 0, then if moves the contents of registers[b] into
 *registers[a]
 */
void move(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c)
{
        if (registers[c] != 0) {
                registers[a] = registers[b];
        }
}

/* calls segments_load and passes it the memory index (registers[b]) and the
 * offset (registers[c]). The value from that space in memory is loaded into
 * registers[a]
 */
void load_from_memory(UM_memory mem, uint32_t *registers, uint32_t a,
                      uint32_t b, uint32_t c)
{
        registers[a] = segments_load(mem, registers[b], registers[c]);
}

/*calls segments_store and passes it the value (registers[c]),along with
 * the memory index (registers[b]) and the offset (registers[c])
 */
void store_in_memory(UM_memory mem, uint32_t *registers,uint32_t a,
                     uint32_t b, uint32_t c)
{
       segments_store(mem, registers[a], registers[b], registers[c]);
}

/* add vales in the given registers and stores the answer in registers[a] */
void add(uint32_t *registers,uint32_t a, uint32_t b, uint32_t c)
{
        registers[a] = registers[b] + registers[c];
}

/* stores the multiplication of the values in the given registers in 
 * registers[a]
 */
void multiply(uint32_t *registers,uint32_t a, uint32_t b, uint32_t c)
{
        registers[a] = registers[b] * registers[c];
}

/* stores the division of the values in the given registers 
 * in registers[a]
 */
void divide(uint32_t *registers,uint32_t a, uint32_t b, uint32_t c)
{
        registers[a] = registers[b] / registers[c];
}

/* stores the bitwise nand of the given registers in registers[a] */
void nand(uint32_t *registers,uint32_t a, uint32_t b, uint32_t c)
{
        registers[a] = ~(registers[b] & registers[c]);
}

/*stops the computation and frees the associated memory */
void halt(UM_memory mem)
{
        free_memory(mem);
        exit(0);
}

/* calls map_segment and passes it the number of words to be stored. The
 * location of the new memory is stored in registers[b]
 */
void map(UM_memory mem, uint32_t *registers, uint32_t b, uint32_t c)
{
        registers[b] = map_segment(mem, registers[c]);
}

/* calls unmap_segment and passes it the memory index */
void unmap(UM_memory mem, uint32_t *registers, uint32_t c)
{
        unmap_segment(mem, registers[c]);
}

/* the value in registers[c] is displayed to standard output */
void output(uint32_t *registers, uint32_t c)
{
        if (registers[c] < 256) {
                uint32_t value = registers[c];
                putchar(value);
        }
}

/*a value from stdin is loaded into registers[c] as long as the value */
void input(uint32_t *registers, uint32_t c)
{
        uint32_t value = getc(stdin);
        if (value == (uint32_t) EOF) {
                registers[c] = ~0;
        }
        else if (value < 256) {
                registers[c] = value;
        }
}

/* calls segments_load_program and passes it the memory location (registers[b])
 * and the value in registers[c]
 */
void load_program(UM_memory mem, uint32_t *registers, uint32_t b,uint32_t c)
{
        segments_load_program(mem, registers[b], registers[c]);
}

/* the value passed in as is loaded into registers[a] */
void load_value(uint32_t *registers, uint32_t a, uint32_t value)
{
        registers[a] = value;
}
