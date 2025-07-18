#include <../Inc/harness.h>


extern int32_t mp3_encode(uint8_t * input, size_t size);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &mp3_encode;

void target_harness(uint8_t * testCase, size_t size){
	
	mp3_encode(testCase, size);
}
