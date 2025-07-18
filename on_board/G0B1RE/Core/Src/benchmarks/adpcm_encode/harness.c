#include <../Inc/harness.h>

extern uint32_t adpcm_encode(uint8_t * audio_file);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &adpcm_encode;

void target_harness(uint8_t * testCase, size_t size){
	
	adpcm_encode(testCase);
}
