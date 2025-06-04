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
#include <setjmp.h>
#endif

#include <stdlib.h>
#include "Core/Inc/fuzz.h"
#include "Core/Inc/time_bare.h"

//#include "stm32f4xx_it.h"
#include "main.h"

void SystemClock_Config(void);

void board_setup(){
/*** Handle any registers we need to setup here...
 *
***/
//	volatile uint32_t * SHCSR = (volatile uint32_t *)0xE000ED24;
//	volatile uint32_t * VTOR = (volatile uint32_t *)0xE000ED08; // Vector Table Offset Register
	volatile uint32_t * ACTLR = (volatile uint32_t *)0xE000E008;


	volatile uint32_t reg_value = 0;

//	reg_value = *SHCSR;
//	reg_value = reg_value | 0x30000; //Enable BUSFAULT, MEMFAULT:bits 18,17,16
//	*SHCSR = reg_value;

	reg_value = *ACTLR;
//	reg_value = reg_value | 0x1; //MultiCycle Instructions
	reg_value = reg_value | 0x2; //Disables write buffers; makes bus faults precise.
	reg_value = reg_value | 0x4; //Disables IT instruction folding.
	*ACTLR = reg_value;
//	*VTOR = g_pfnVectors; //Set the vector table offset register

}

/**
  * @brief  The application entry point.
  * @retval int
  */
int16_t main(void)
{


  	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  	HAL_Init();

  	/* Configure the system clock */
	SystemClock_Config();

  	board_setup();
  	/* Initialize all configured peripherals */
  	fuzzing_loop();

  	return 0;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
   USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
