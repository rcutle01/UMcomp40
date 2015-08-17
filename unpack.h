/*
 * unpack.h
 *      the interface for the unpack module of the UM
 *      unpacks the instructions in the UM_memory and performs the
 *      operations provided by the instructions
 *
 * Written by: Nathan Majumder (nmajum01) & Becky Cutler (rcutle01)
 * Date: 8 April 2015
 *
 */

#ifndef UNPACK_H_INCLUDED_
#define UNPACK_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include "segments.h"
#include <stdint.h>

/* Gets instructions from the given UM_memory and performs the operations
 * necessary
 */
void unpack_instructions(UM_memory mem); 

#endif /* UNPACK_H_INCLUDED */
