/*
 *
 *      operations interface: declares functions that perform the 13 possible
 *      operations
 *      Written by: Becky Cutler and Nathan Majumder
 *      3/25/15
 */

#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "segments.h"

/*if registers[c]!= 0, then if moves the contents of registers[b] into
 *registers[a]
 */
void move(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/*loads into registers[a], the value in memory at the segment stored in
  registers[b] and at offset stored in registers[c] */
void load_from_memory(UM_memory mem, uint32_t *registers, uint32_t a,
                      uint32_t b, uint32_t c);

/*stores the value at registers[c], in memory at the segment stored in
  registers[a] and at offset stored in registers[b] */
void store_in_memory(UM_memory mem, uint32_t *registers, uint32_t a,
                     uint32_t b, uint32_t c);

/*stores in registers[a], the sum of the contents of registers[b] and
  registers[c] mod 2^32 */
void add(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/*stores in registers[a], the product of the contents of registers[b] and
  registers[c] mod 2^32 */
void multiply(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/*stores in registers[a], the floor of the quotient of the contents of 
  registers[b] and registers[c] */
void divide(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/*stores in registers[a] the bitwise "not" of the bitwise "and" of the contents
  of registers[b] and registers[c] */
void nand(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/*stops the computation and frees the memory associated with the sequence and
  the stack */
void halt(UM_memory mem);

/*creates a new segment in the sequence with the number of words equal to the
 *value in registers[c]. Each word is initialized to zero. The location in the
 *sequence of the new mapped segment is placed into registers[b]
 */
void map(UM_memory mem, uint32_t *registers, uint32_t b, uint32_t c);

/*the segment in memory location at registers[c] is unmapped and the location
  is freed for future mapping use */
void unmap(UM_memory mem, uint32_t *registers, uint32_t c);

/*the value in registers[c] is displayed to standard output as long as the
  value is in the range of 0 to 255 */
void output(uint32_t *registers, uint32_t c);

/*a value from standard input is loaded into registers[c] as long as the value
 *is between 0 and 255. If it is at the end of input, a value of 32 bits of
 *all ones is loaded into registers[c]
 */
void input(uint32_t *registers, uint32_t c);

/*the segment at memory location in registers[b] is put at memory location 0,
 *and the program counter is set to an offset of the value in registers[c] in
 *that segment
 */
void load_program(UM_memory mem, uint32_t *registers, uint32_t b, uint32_t c);

/*the value passed in as a uint32_t is loaded into registers[a] */
void load_value(uint32_t *registers, uint32_t a, uint32_t value);

#endif /* OPERATIONS_H_INCLUDED */

