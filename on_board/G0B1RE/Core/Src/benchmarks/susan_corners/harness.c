#include <../Inc/harness.h>

extern int32_t susan_corners(uint8_t * in);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &susan_corners;

void target_harness(uint8_t * testCase, size_t size){
	
	susan_corners(testCase);
}
