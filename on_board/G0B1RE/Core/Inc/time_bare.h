#ifndef __TIME_BARE_H
#define __TIME_BARE_H

/* time_bare.h -> bare metal timer for fuzzer.
 * These functions need to be implemented per target device.
 * Would recommend using the systick timer of your target board.
 * */
#include <stdint.h>
#include "fuzz.h"
#include "Drivers/STM32G0xx_HAL_Driver/Inc/stm32g0xx_hal.h"

int16_t	setup_fuzz_time(void);
void start_fuzz_time(void);
void end_fuzz_time(void);
void adjust_fuzz_time(void);
void execution_cleanup(void);
void SysTick_Handler(void);


//void __atrribute__((interrupt)) TimeoutHandler (void)


#endif // __TIME_BARE_H

