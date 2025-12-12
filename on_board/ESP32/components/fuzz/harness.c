#include <harness.h>


//extern int16_t global_test(int8_t * args, int16_t argc);
//extern void _exit_test(void);
//extern int16_t branch_test(uint8_t * args, size_t size);
//extern uint16_t activity_rec(uint32_t seed);
//extern int32_t mp3_encode(uint8_t * input, size_t size);
extern int32_t susan_edges(uint8_t * in);

void * g_sutPtr  = &susan_edges;
//void * g_sutPtr = &branch_test;

__attribute__((no_instrument_function)) void target_harness(uint8_t * testCase, size_t size){
	
	//uint32_t seed = testCase[0] | (testCase[1] << 8) | (testCase[2] << 16) | (testCase[3] << 24);

	susan_edges(testCase);
	//activity_rec(seed);
//	branch_test(testCase, size);
	//_exit_test();
//	global_test(testCase, size);
	//mp3_encode(testCase, size);
}
