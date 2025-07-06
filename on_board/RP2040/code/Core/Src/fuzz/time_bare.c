#include "../../Inc/time_bare.h"
#include "../../Inc/fuzz.h"
//#include "Drivers/CMSIS/Include/core_cm4.h"

#define MAX_TIMEOUT_VALUE (5 * 1)	//2 Seconds.. TODO: Figure out what the right value is here

uint64_t g_sut_timeout = 1;
/*
uint64_t g_count __attribute__((section(".fuzzbss"))) = 0 ;
extern uint32_t fuzzer_return __attribute__((section(".fuzzbss")));
extern volatile cpuState_t reg_context __attribute__((section(".fuzzbss")));
uint8_t timeout_flag __attribute__((section(".fuzzbss"))) = 0;
*/

uint64_t g_count = 0;
extern uint32_t fuzzer_return;
extern volatile cpuState_t reg_context;
static alarm_id_t timer_id;
static alarm_pool_t * pool_ptr;

int16_t setup_fuzz_time(void) {
	pool_ptr = alarm_pool_create(1,1);

	if(!pool_ptr){
		return -1;
	}
//	uint32_t ret  = 0;

//	ret = HAL_SetTickFreq(HAL_TICK_FREQ_10HZ);	/*10 ticks a second. This will re-init the tick.*/
//	if ( ret != 0 ){
//		return ret;
//	}
//	NVIC_SetPriority(PendSV_IRQn, 0xFF);
//	NVIC_SetPriority(SysTick_IRQn, 0x00);
	return 0;
}
void execution_cleanup(void) {
	/*Restore stack context from begining of execution and jump back to start*/
	__asm volatile (
		"mov r0, %0\n\t"
		:
		: "r" (&reg_context));
	__asm volatile (
		"add r0, #56\n\t"
		"ldr r1, [r0]\n\t"	/*Stack Pointer here*/
		"mov r2, sp\n\t"
		"sub r1, r2\n\t"	/*Restore stack*/
		"add sp, r1\n\t"
		"add r0, #4\n\t"
		"ldr r1, [r0]\n\t"	
		"mov r2, sp\n\t"
		"str r1, [r2, #4]\n\t" 	/*PC on the stack*/
	);
	__asm volatile (
		"mov r0, %0\n\t"
		:
		: "r" (&reg_context));
	__asm volatile (
		"add r0, #4\n\t"
		//"ldm r0!, {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}\n\t"
		"ldm r0!, {r1, r2, r3, r4, r5, r6, r7}\n\t"
		"ldr r1, [r0]\n\t"
		"mov r1, r8\n\t"
		"add r0, #4\n\t"
		"ldr r1, [r0]\n\t"
		"mov r1, r9\n\t"
		"add r0, #4\n\t"
		"ldr r1, [r0]\n\t"
		"mov r1, r10\n\t"
		"add r0, #4\n\t"
		"ldr r1, [r0]\n\t"
		"mov r1, r11\n\t"
		"add r0, #4\n\t"
		"ldr r1, [r0]\n\t"
		"mov r1, r12\n\t"
		"add r0, #4\n\t"
		"ldr r1, [r0]\n\t"
		"mov r1, lr\n\t"
		"add r0,  #4\n\t");
	__asm volatile (
		"mov r0, %0\n\t"
		:
		: "r" (&reg_context));
	__asm volatile (
		"add r0, #4\n\t"
		"ldr r1, [r0]\n\t"
		"mov r0, sp\n\t"
		"str r7, [r0]\n\t"
		"mov r0, #0\n\t" 	/* r0 is used for return values. In this case the saved state is always 0*/
		"msr CONTROL, r0\n\t"
		"isb\n\t"

	);
}
int64_t timer_callback(alarm_id_t id, __unused void *user_data){
	g_sut_timeout++;

	__asm volatile (
		"mov r0, %0\n\t"
		:
		: "r" (&execution_cleanup));
	__asm volatile (
		"ldr r1, [r0]\n\t"	/*Write onto the saved stack context to be loaded by the core automatically*/
		"mov r0, sp\n\t"
		"str r1, [r0, #24]\n\t"
		);
	return 1;
}

void start_fuzz_time(void) {

	timer_id = add_alarm_in_ms(2000, timer_callback, NULL, true);
}

void end_fuzz_time(void){

	alarm_pool_cancel_alarm(pool_ptr, timer_id);
	g_count = 0;
}



