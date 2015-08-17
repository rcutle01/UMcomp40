/*
 * segments.c
 *      the implementation for the segmented memory of the UM
 *      defines the components of the struct UM_memory
 *      defines functions that manipulate the memory
 *
 * Written by: Nathan Majumder (nmajum01) & Becky Cutler (rcutle01)
 * Date: 8 April 2015
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "seq.h"
#include "stack.h"
#include "segments.h"
#include "bitpack.h"

#define BYTE 8
#define BYTES_IN_WORD 4

struct UM_memory {
        Seq_T segments;
        Stack_T unmapped;
        unsigned prog_counter;
};

/****** private helper function declarations ******/

/* removes all words in given segment but does not free the sequence itself */
void free_segment_words(Seq_T segment);

/**************************************************/

/* initializes a UM_memory and mallocs space for all appropriate
 * components, initially mapping just the 0-segment and setting the 
 * program counter to 0
 */
UM_memory initialize_memory()
{       
        UM_memory mem = malloc(sizeof(struct UM_memory));
        uint32_t initial_guess = 10;

        /* create new memory with initial guess of 10 segments */
        mem->segments = Seq_new(initial_guess);
        mem->unmapped = Stack_new();

        map_segment(mem, 0);
        mem->prog_counter = 0; 

        return mem;
}

/* frees memory for the entire provided UM_memory struct */
void free_memory(UM_memory mem)
{
        int length = Seq_length(mem->segments);
        for (int i = 0; i < length; i++) {
                Seq_T segment = Seq_get(mem->segments, i);
                free_segment_words(segment);
                Seq_free(&segment);
        }
        Seq_free(&(mem->segments));
        
        while (Stack_empty(mem->unmapped) != 1) {
                uint32_t *popped = Stack_pop(mem->unmapped);
                free(popped);

        }
        Stack_free(&mem->unmapped);

        free(mem);
}

/* parses the given input file and creates the 32 bit instructions,
 * which it then loads into the 0-segment until it hits EOF
 */
void load_instructions(UM_memory mem, FILE *input) {
        uint32_t new_word = ~0;
        int c = 0;
        while (c != EOF) {
                if (new_word != (uint32_t) ~0) {
                        Seq_T seg_zero = Seq_get(mem->segments, 0);
                        uint32_t *word = malloc(sizeof(uint32_t));

                        *word = new_word;
                        Seq_addhi(seg_zero, word);
                        new_word = 0;
                }
                for (int i = 3; i >= 0; i--) {
                        c = fgetc(input);
                        if (c == EOF) {
                                break;
                        } else {
                                new_word = Bitpack_newu(new_word, BYTE,
                                                        BYTE * i, c);
                        }
                }
        }
}

/* gets the next instruction in the 0-segment and increments
 * the program counter -- if there are no more instructions
 * to read, it returns 0
 */
uint32_t get_instruction(UM_memory mem)
{
        if (done_with_instructions(mem) == 1) {
                return 0;
        }

        Seq_T seg_zero = Seq_get(mem->segments, 0);
        uint32_t *instruction = Seq_get(seg_zero, mem->prog_counter);

        mem->prog_counter++;

        return *instruction;
}

/* checks whether the prog_counter is still less than the number of
 * instructions in the 0-segment -- returns 0 if there are still more
 * instructions to read, returns 1 if done with instructions
 */
uint32_t done_with_instructions(UM_memory mem)
{
        Seq_T seg_zero = Seq_get(mem->segments, 0);
        if (mem->prog_counter >= (unsigned) Seq_length(seg_zero)) {
                return 1;
        }
        return 0;
}

/* checks whether there are any indices on the unmapped address stack
 *      if so, it pops an address and maps a segment of the given number of
 *              words to that address.
 *      if not, it maps a segment to the end of the memory sequence
 * initializes the given number of words to all zeros in the segment
 * returns the index of the new segment in the sequence
 */
uint32_t map_segment(UM_memory mem, uint32_t num_words) 
{
        uint32_t index;
        /* checking to see if segment was previously mapped  */
        if (Stack_empty(mem->unmapped) == 1) {
                index = Seq_length(mem->segments);
                Seq_addhi(mem->segments, Seq_new(num_words));
        } else {
                uint32_t *popped = Stack_pop(mem->unmapped);
                index = *popped;
                free(popped);
                Seq_T old_segment = Seq_get(mem->segments, index);
                /* we have to free everything before we remap it */
                free_segment_words(old_segment);
        }

        /* initialize new segment to 0s */
        Seq_T segment = Seq_get(mem->segments, index);
        for (uint32_t i = 0; i < num_words; i++) {
                uint32_t *new_space = malloc(sizeof(*new_space));

                *new_space = 0;
                Seq_addhi(segment, new_space);
        }
        return index;
}

/* to unmap, simply push the index of the given segment to the unmapped
 * address stack so we can reuse it -- don't free segment here, b/c allocated
 * memory will be freed if/when the segment index is reused
 */
void unmap_segment(UM_memory mem, uint32_t segment_index)
{
        uint32_t *new_seg_index = malloc(sizeof(*new_seg_index));

        *new_seg_index = segment_index;
        Stack_push(mem->unmapped, new_seg_index);
}

/* return the value at the given segment in the memory sequence at the given
 * offset in that segment sequence
 */
uint32_t segments_load(UM_memory mem, uint32_t segment_index, uint32_t offset)
{
        Seq_T segment = Seq_get(mem->segments, segment_index);
        uint32_t *loaded_value = Seq_get(segment, offset);

        return *loaded_value;
}

/* store the given value in the memory sequence in the given segment at the
 * given offset in that segment sequence
 */
void segments_store(UM_memory mem, uint32_t segment_index, uint32_t offset,
                    uint32_t value)
{
        Seq_T segment = Seq_get(mem->segments, segment_index);
        uint32_t *value_location = Seq_get(segment, offset);
        *value_location = value;
}

/* unmaps and remaps the 0-segment with the number of words in the segment
 * at the given segment index
 * then copies each value in the given segment into the 0-segment
 * finally sets the program counter to be the given offset in the new 0-segment
 * if the segment index is 0, nothing is unmapped, mapped, or copied, but the
 * prog counter is set to the offset in the 0-segment
 */
void segments_load_program(UM_memory mem, uint32_t segment_index,
                           uint32_t offset)
{
        if (segment_index != 0) {
                Seq_T segment = Seq_get(mem->segments, segment_index);
                int length = Seq_length(segment);
                /* unmapping the zero segment so that it will be the next
                   possible address on the stack to map to */
                unmap_segment(mem, 0);
                map_segment(mem, length);
                Seq_T seg_zero = Seq_get(mem->segments, 0);
                for (int i = 0; i < length; i++) {
                        uint32_t *value = Seq_get(segment, i);
                        uint32_t *newval_location = Seq_get(seg_zero, i);
                        *newval_location = *value;
                }
        }
        mem->prog_counter = offset;
}

/****** private heler function definitions ******/

/* frees all words in the given sequence */
void free_segment_words(Seq_T segment)
{
        int length = Seq_length(segment);
        for (int i = 0; i < length; i++) {
                uint32_t *removed = Seq_remhi(segment);
                free(removed);
        }
}
