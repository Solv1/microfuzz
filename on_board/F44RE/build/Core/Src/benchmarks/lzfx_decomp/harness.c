#include <../Inc/harness.h>



extern int32_t lzfx_decomp_test(uint8_t * data, size_t size);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &lzfx_decomp_test;

void target_harness(uint8_t * testCase, size_t size){
	
	lzfx_decomp_test(testCase, size);
}
