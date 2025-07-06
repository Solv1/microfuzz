#include <../Inc/harness.h>

extern int32_t lzfx_compress_test(uint8_t * data, size_t size);


void * g_sutPtr __attribute__((section(".fuzzbss"))) = &lzfx_compress_test;

void target_harness(uint8_t * testCase, size_t size){
	
	lzfx_compress_test(testCase, size);
}
