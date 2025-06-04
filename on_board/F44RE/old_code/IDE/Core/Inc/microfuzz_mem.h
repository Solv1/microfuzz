/*
 * microfuzz_mem.h
 *
 *  Created on: Dec 10, 2024
 *      Author: santiago
 */

#ifndef MICROFUZZ_MEM_H_
#define MICROFUZZ_MEM_H_
#include <stdint.h>
#include <stddef.h>

/********************************************************
 * @brief volatile_memset:  A memset function for volatile memory.
 * @param des: pointer to the memory region to set
 *        val: the value to set
 *        n: number of bytes to set
 * @return void
 ********************************************************/
void volatile_memset(volatile void * des,  int8_t val, size_t n);



#endif /* MICROFUZZ_MEM_H_ */
