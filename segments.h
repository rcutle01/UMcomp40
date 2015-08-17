/*
 * segments.h
 *      the interface for the segmented memory of the UM
 *      uses an incomplete struct definition called UM_memory
 *      declares functions that manipulate the memory
 *              (see individual function comments)
 *
 * Written by: Nathan Majumder (nmajum01) & Becky Cutler (rcutle01)
 * Date: 8 April 2015
 *
 */


#ifndef SEGMENTS_H_INCLUDED
#define SEGMENTS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct UM_memory *UM_memory;

/* creates and initializes a new UM_memory struct (all memory associated with
 * the universal machine then returns the struct
 */
UM_memory initialize_memory();

/* frees all memory associated with the given UM_memory struct */
void free_memory(UM_memory mem);

/* takes the initialized memory and an input FILE *
 * parses the file to get the 32 bit instructions it holds and puts them
 *      in memory
 */
void load_instructions(UM_memory mem, FILE *input);

/* maps a new segment in memory of the given number of words
 * returns the segment index in memory of the newly mapped segment
 */
uint32_t map_segment(UM_memory mem, uint32_t num_words);

/* unmaps the segment at the given index from memory, allowing another
 * segment mapping to potentially reuse that location
 */
void unmap_segment(UM_memory mem, uint32_t segment_index);

/* loads a value from memory at the given segment index and at the given offset
 * within that segment -- the value at that location is returned to the user
 */
uint32_t segments_load(UM_memory mem, uint32_t segment_index, uint32_t offset);

/* stores the given value in memory in the segment at the given index and at
 * the given offset within that segment
 */
void segments_store(UM_memory mem, uint32_t segment_index, uint32_t offset,
                    uint32_t value);

/* replaces the memory instruction stream with the words following the given
 * offset in the given segment -- so get_instruction will now begin returning
 * the words following that offset in that segment
 */
void segments_load_program(UM_memory mem, uint32_t segment_index,
                           uint32_t new_offset);

/* returns the next instruction (a 32 bit word) in the instruction stream for
 * the given UM_memory
 * if there are no more instructions to read, it returns 0
 */
uint32_t get_instruction(UM_memory mem);

/* returns 1 if there are no more instructions to be read in memory,
 * returns 0 otherwise
 */
uint32_t done_with_instructions(UM_memory mem);

#endif /* SEGMENTS_H_INCLUDED_ */
