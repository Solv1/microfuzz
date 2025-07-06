#include <../Inc/harness.h>



extern int32_t jpeg_encode(uint8_t * data, uint16_t w, uint16_t h);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &jpeg_encode;

void target_harness(uint8_t * testCase, size_t size){
	
	uint16_t w, h;
	w = size/8;
	h = size/w;
	jpeg_encode(testCase, w, h);
}
