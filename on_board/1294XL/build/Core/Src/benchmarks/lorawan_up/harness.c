#include <../Inc/harness.h>


//extern int16_t global_test(int8_t * args, int16_t argc);
//extern void _exit_test(void);
extern int16_t lorawan_up(uint8_t *data, size_t size);

void * g_sutPtr __attribute__((section(".fuzzbss"))) = &lorawan_up;

void target_harness(uint8_t * testCase, size_t size){
	
	lorawan_up(testCase, size);
}
