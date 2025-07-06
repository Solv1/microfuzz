#include <../Inc/harness.h>


extern int32_t lorawan_down(uint8_t * input, size_t size);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &lorawan_down;

void target_harness(uint8_t * testCase, size_t size){
	
	lorawan_down(testCase, size);
}
