#include "harness.h"


//extern int16_t global_test(int8_t * args, int16_t argc);
//extern void _exit_test(void);
//extern int16_t branch_test(uint8_t * args, size_t size);
///void * g_sutPtr __attribute__((section(".fuzzbss"))) = &branch_test;
void * g_sutPtr __attribute__((section(".fuzzbss"))) = NULL;

void target_harness(uint8_t * testCase, size_t size){
	
	branch_test(testCase, size);
	//_exit_test();
//	global_test(testCase, size);
}
