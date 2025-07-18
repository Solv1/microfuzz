#include <../Inc/harness.h>

extern int32_t susan_smooth(uint8_t * in);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &susan_smooth;

void target_harness(uint8_t * testCase, size_t size){
	
	susan_smooth(testCase);
}
