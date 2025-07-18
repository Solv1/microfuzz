#include <../Inc/harness.h>


//extern int16_t global_test(int8_t * args, int16_t argc);
//extern void _exit_test(void);

extern uint32_t image_class(uint8_t * image);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &image_class;

void target_harness(uint8_t * testCase, size_t size){
	
	image_class(testCase);
}
