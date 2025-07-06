/*
 * microfuzz_mem.c
 *
 *  Created on: Dec 10, 2024
 *      Author: santiago
 */
#include "microfuzz_mem.h"
#include <stdint.h>
#include <stdlib.h>




void volatile_memset(volatile void * des,  int8_t val, size_t n){
	size_t i;
	volatile int8_t * mem = des;

	if(des == NULL){
		exit(-1);
	}

	for(i = 0; i < n; i++){
		mem[i] = val;
	}

	return;
}
void * volatile_memcpy(volatile void * des, volatile void * src, size_t n){
	size_t i;

	volatile uint8_t * des_mem = des;
	volatile uint8_t * src_mem = src;

	if(des == NULL || src == NULL){
		return NULL;
	}

	for(i = 0; i < n; i++){
		des_mem[i] = src_mem[i];
	}

	return des;
}

