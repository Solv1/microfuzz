#include "../../../Inc/harness.h"


//extern int16_t global_test(int8_t * args, int16_t argc);
//extern void _exit_test(void);
extern uint16_t activity_rec(uint32_t seed);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &activity_rec;

void target_harness(uint8_t * testCase, size_t size){
	uint32_t dummy_var = (testCase[3]) | (testCase[2] << 8) | (testCase[1] << 16) | (testCase[0] << 24);
	activity_rec(dummy_var);
	
}
