#include "../../Inc/time_bare.h"
#include "../../Inc/fuzz.h"

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
uint8_t timeout_flag = 0;

int16_t setup_fuzz_time(void) {
	/*assumes a 100Mhz clock speed*/

	uint32_t ret  = 0;

	ret = HAL_SetTickFreq(HAL_TICK_FREQ_10HZ);	/*10 ticks a second. This will re-init the tick.*/
	if ( ret != 0 ){
		return ret;
	}
//	NVIC_SetPriority(PendSV_IRQn, 0xFF);
//	NVIC_SetPriority(SysTick_IRQn, 0x00);
	return 0;
}

void start_fuzz_time(void) {

	HAL_ResumeTick();
	__enable_irq();
}

void end_fuzz_time(void){

	HAL_SuspendTick();
	g_count = 0;
}

void adjust_fuzz_time(void) {
	/*Adjusts the tick handler in 1/10 second intervals.*/

	if (g_sut_timeout == MAX_TIMEOUT_VALUE){
		return;
	}
	g_sut_timeout++; 
}

void execution_cleanup(void) {
	/*Restore stack context from begining of execution and jump back to start*/
	end_fuzz_time();
        __asm volatile (    
                "mov r0, %0\n\t"    
                :    
                : "r" (&reg_context));    
        __asm volatile (    
                "adds r0, #56\n\t"    
                "ldr r1, [r0]\n\t"      /*Stack Pointer here*/    
                "mov r2, sp\n\t"    
                "subs r1, r2\n\t"       /*Restore stack*/    
                "add sp, r1\n\t"    
                "adds r0, #4\n\t"
                "ldr r1, [r0]\n\t"      
                "mov r2, sp\n\t"
                "str r1, [r2, #4]\n\t"  /*PC on the stack*/
        );
        __asm volatile (
                "mov r0, %0\n\t"
                :
                : "r" (&reg_context));
        __asm volatile (
                "adds r0, #4\n\t"
                //"ldm r0!, {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}\n\t"
                "ldm r0!, {r1, r2, r3, r4, r5, r6, r7}\n\t"
                "ldr r1, [r0]\n\t"
                "mov r1, r8\n\t"
                "adds r0, #4\n\t"
                "ldr r1, [r0]\n\t"
                "mov r1, r9\n\t"
                "adds r0, #4\n\t"
                "ldr r1, [r0]\n\t"
                "mov r1, r10\n\t"
                "adds r0, #4\n\t"
                "ldr r1, [r0]\n\t"
                "mov r1, r11\n\t"
                "adds r0, #4\n\t"                                                                                     
                "ldr r1, [r0]\n\t"    
                "mov r1, r12\n\t"    
                "adds r0, #4\n\t"    
                "ldr r1, [r0]\n\t"    
                "mov r1, lr\n\t"    
                "adds r0,  #4\n\t");    
        __asm volatile (    
                "mov r0, %0\n\t"    
                :    
                : "r" (&reg_context));    
        __asm volatile (    
                "adds r0, #4\n\t"    
                "ldr r1, [r0]\n\t"    
                "mov r0, sp\n\t"    
                "str r7, [r0]\n\t"    
                "movs r0, #0\n\t"       /* r0 is used for return values. In this case the saved state is always 0*/
                "msr CONTROL, r0\n\t"
                "isb\n\t");


}


void SysTick_Handler(void) {
	/*For an execution timeout but ARM semantics etc.*/
	g_count++;
	
	__disable_irq();
	if (g_count >= MAX_TIMEOUT_VALUE) {
		timeout_flag = 1;
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; /*Set the SV flag to get out of handler context and back into thread*/
	}
	__enable_irq();

}

void PendSV_Handler(void) {
	if (timeout_flag == 1){
		timeout_flag = 0;
		g_count = 0;
		__asm volatile (
			"ldr r0, =cleanup_function\n\t" 	/*ARM hack here to get to the clean-up function*/
			"ldr r1, [r0]\n\t"		/*Write onto the saved stack context to be loaded by the core automatically*/
			"mov r0, sp\n\t"
			"str r1, [r0, 0x18]\n\t"
		);

	}

}

