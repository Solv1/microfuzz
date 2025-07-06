#include <../Inc/harness.h>


//extern int16_t global_test(int8_t * args, int16_t argc);
//extern void _exit_test(void);
extern int32_t susan_edges(uint8_t * in);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &susan_edges;

void target_harness(uint8_t * testCase, size_t size){
	
	susan_edges(testCase);
}
