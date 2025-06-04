#ifndef __FUZZ_H
#define __FUZZ_H

#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "time_bare.h"

#define LOCAL_POOL_SIZE 15
#define MAX_BLOCKS_PER_INPUT 200
#define MAX_CYCLES 250           /*250 Cycles of mutation before getting a new seed.*/
#define MAX_TESTCASE_LEN 256 //Max size of a testcase 256 bytes
#define MIN_TESTCASE_LEN 4
#define UINT_MAX 65535
#define STD_LIBRARY 1
/*------------------------------------------------------------------------------------------*/

enum Mutation{
        BITFLIP1_1,		//Walking 1 bit with 1 bit step over
        BITFLIP2_1,		//Walking 2 bit with 1 bit step over
        BITFLIP4_1,		//Walking 4 bit with 1 bit step over
        BYTEFLIP1_1,	        //Walking 1 byte with 1 byte step over
        BYTEFLIP2_1,	        //Walking 2 byte with 1 byte step over
        BYTEFLIP4_1,	        //Walking 4 byte with 1 byte step over
        ADDINTVALUE,	        //Add an interesting value 1-35
        SUBINTVALUE,	        //Subtract an interesting value 1-35
        RANDOM,			//Do a random amount of stacked mutations
        SPLICE,			//Take two testcases and combine them
        SHRINK,			//Decrease the size of the testcase
        PAD,			//Increase the size of the testcase
};

typedef struct {
	uint8_t testCase[MAX_TESTCASE_LEN];
	size_t size;
}testCase_t;

typedef struct {
	uint8_t stageCycles;
	uint16_t mutationDegression;
	uint16_t currentMutation;	//Current mutation pattern.
}mutationContext_t;

typedef struct {
	uint8_t seedHead;
	uint8_t seedTail;
	uint16_t poolCycles; // How many times have we gone through the pool.
}poolContext_t ;


typedef enum
{
	TIMEOUT,
	MEM_ERROR,
	BUS_ERROR

} crash_t;

typedef struct {
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r12;
	uint32_t lr;
	uint32_t sp;
	uint32_t pc;
}cpuState_t;


//TODO: Move all the fuzzing items here.

void fuzz_exit(int status);
void crash_handler(void);
void state_restore(void);
int16_t fuzzer_setup(void * funPtr);
int16_t mutator(void);
void bubble_coverage(void);
void check_for_coverage(void);
void dequeue_seed(void);
void fuzzing_loop(void);


#endif // __FUZZ_H
