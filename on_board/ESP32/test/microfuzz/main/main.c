/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Microfuzz main file.
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------*/

#ifdef STD_LIBRARY
#include <stdio.h>

#include <stdint.h>
#include <stdalign.h>

#include <string.h>
#endif

#include <stdlib.h>
#include "../components/fuzzlogic/time_bare.h"
#include "../components/fuzzlogic/fuzz.h"


//#include "stm32f4xx_it.h"
#include "main.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
void app_main(void)
{
  	fuzzing_loop();

}

