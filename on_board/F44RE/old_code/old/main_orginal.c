
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Microfuzz main file.
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <setjmp.h>

#include "stm32f4xx_it.h"
#include "main.h"
#include "microfuzz_mem.h"

/*-------------------------------------------------------------------------------------------*/
#define TIMEOUT 100000 /*TODO: Adjust this for the right clock.*/
#define SEED_CAPACITY 15
#define MAX_BLOCKS_PER_INPUT 200
#define MAX_CYCLES 250           /*250 Cycles of mutation before getting a new seed.*/
#define WIDTH 128 //Fixed size width for now.
#define UINT_MAX 65535
/*------------------------------------------------------------------------------------------*/

/*-------------------Test Case Handling-----------------------------------------------------*/
volatile uint8_t local_pool[SEED_CAPACITY][WIDTH]; //Local storage of test cases
volatile uint8_t current_seed_num = 0; //Tracks which seed in the seed pool is in use

volatile uint16_t intersting_cases = 0; //The number of coverage increasing test cases so far

volatile uint8_t * current_testcase; // Pointer to current test case

uint16_t seed_head = 0;
uint16_t seed_tail = SEED_CAPACITY - 1;
uint8_t stage_cycles = 0;
uint16_t pool_loops = 0;

volatile uint16_t iterations = 0;
volatile uint16_t mutation_degression = 1;
/*-------------------Coverage Tracing------------------------------------------------------*/
volatile uint16_t coverage_list[MAX_BLOCKS_PER_INPUT];
volatile uint16_t * coverage_list_head; //Used in ASM function to point to the coverage list
uint32_t * sut_start_address = 0; //Points to the start of our sut for coverage tracing
int8_t isIncreasing = 0;
/*-----------------------------------------------------------------------------------------*/

jmp_buf saved_context;

extern int16_t branch_test(int8_t * args, int16_t argc);

void SystemClock_Config(void);


void crash_void(){ //The place where all bad errors go
    while(1);
}


int16_t fuzzer_setup(void * function_pointer){
/********************************************************
 * @brief Inits fuzzer, testing random number generatation, and sets the coverage map to zero.
 * @param None
 * @return 0 on success, -1 on failure
********************************************************/

    current_testcase = calloc(WIDTH, sizeof(uint8_t)); //Malloc memory for the testcase buffer
    if(current_testcase == NULL){
        printf("Failed to get memory for input buffer.\n");
        exit(-1);
    }

//    volatile_memset(&coverage_list,0,sizeof(coverage_list));
    memset(&coverage_list,0,sizeof(coverage_list));
    coverage_list[0] = UINT_MAX;

    memset(&coverage_list,0,sizeof(coverage_list));
//    volatile_memset(&local_pool, 0, sizeof(local_pool));
    coverage_list_head =  &coverage_list[0];

    sut_start_address = ((uint32_t *)function_pointer); /*Finds the offset in memory of the start of our sut */


    return 0;

}



void mutator(uint8_t * input, size_t input_size){
/********************************************************
 * @brief Mutator:  A test generation method through mutation
 * @param input: pointer to byte input
 *        input_size: size of the input
 * @return void
 ********************************************************/
	uint8_t i = 0;
    uint8_t j = 0;
    size_t rnd_byte = 0;
    uint8_t rnd_bit = 0;
    uint8_t rnd_value = 0;
    uint8_t mutation_pick = 0;
    uint8_t bits_set = 0;
    uint8_t bytes_set = 0;

    uint8_t start = 0;
    uint8_t mutation_amount = 0;


    /*Input Santitization*/
    if(input_size == 0 || input_size > WIDTH){
        exit(-1);
    }
    if(input == NULL){
    	exit(-1);
    }

//Flips a random bit in the 8 bit value
#define bitflip(value, b) (value ^ (1 << (b)))
    if(mutation_degression > 2){ //If there is mutation degression calc it here.
//        mutation_amount = input_size / (input_size / mutation_degression);
//        if(mutation_amount == 0){
//            mutation_amount = 1; //TODO: Handle different input sizes here maybe a while loop counting up to adjust?
//        }

        start = 0;
    }
    else{
        mutation_amount = 1;
        start = 0;
    }

    /*-----Mutator Body-----*/

    if(stage_cycles < 2 && mutation_degression < 2){
        for(i = 0; i < input_size; i++){
            //Walking 1-bit flip with one bit step over
            for(j = stage_cycles % 2; j < 8; j += 2){
                input[i] = bitflip(input[i], j);
            }
        }
    }

    else if(stage_cycles < 4 && mutation_degression < 2){
        for(i = 0; i < input_size; i++){
            // Walking 2-bit flip with one bit step over
        	for(j = 0; j < 8; j++){
				if(bits_set % 2 == 0){
					input[i] = bitflip(input[i], j);
					bits_set++;
				}
				else{
					bits_set = 0;
				}
			}
        }
    }
    else if(stage_cycles < 6 && mutation_degression < 2){
        for(i = 0; i < input_size; i++){
            // Walking 4-bit with one bit step over
            for(j = 0; j < 8; j++){
            	if(bits_set % 4 == 0){
            		input[i] = bitflip(input[i], j);
            		bits_set++;
            	}
            	else{
            		bits_set = 0;
            	}
            }
        }
    }
    else if(stage_cycles < 8 && mutation_degression < 2){

        // Walking byte flip with one byte step over
        for(i = stage_cycles % 2; i < input_size; i+=2){
            input[i] = input[i] ^ 0xFF;
        }
    }
    else if(stage_cycles < 10 && mutation_degression < 2){
    	//for(i = stage_cycles % 2; i < input_size - 1 -(stage_cycles % 2); i = i + mutation_amount){
        //Walking 2-byte flip with one byte step over
        for(i = stage_cycles % 2; i < input_size; i++){
        	if(bytes_set % 2 == 0){
        		input[i] = input[i] ^ 0xFF;
        		bytes_set++;
        	}
        	else{
        		bytes_set = 0;
        	}
        }
    }
    else if(stage_cycles < 12 && mutation_degression < 2){

        //Walking 32-bit flip
        for(i = stage_cycles % 2; i < input_size; i++){
        	if(i % 4 == 0){
				input[i] = input[i] ^ 0xFF;
				bytes_set++;
			}
        	else{
        		bytes_set = 0;
        	}
        }
    }
    else if((stage_cycles < 48) && (mutation_degression < 2)){

        //Add a increasing value.
        for(i = 0; i < input_size; i+=2){
            input[i] = input[i] + (stage_cycles % 35);

        }
    }
    else if(stage_cycles < 82 && mutation_degression < 5){
        //Subtract a increasing value.
        for(i = 0; i < input_size; i+=2){
                input[i] = input[i] - (stage_cycles % 35);

        }
    }
    else if(stage_cycles <= MAX_CYCLES){

        if(mutation_degression == 0){
        	exit(-1);
        }

        rnd_value = (uint8_t)(rand() % (100/(mutation_degression % 100))); // Perform a random amount of random mutations

        for(i = 0; i < rnd_value; i++){
        	mutation_pick = (uint8_t)(rand() % 12); //Choose another mutatation

            rnd_byte = (uint8_t)(rand() % input_size);

            switch (mutation_pick)
            {
            case 0:
                input[rnd_byte] = (uint8_t)(rand() % 256); //Replace a random byte with another random byte.
                break;
            case 1:
                input[rnd_byte] = input[rnd_byte] + (stage_cycles % 34); //Add a random 'char'
                break;
            case 2:
                input[rnd_byte] = input[rnd_byte] + (stage_cycles % 256); //Add a random byte
                break;
            case 3:
                input[rnd_byte] = input[rnd_byte] - (stage_cycles % 256); //Subtract a random byte
                break;
            case 4:
                input[rnd_byte] = input[rnd_byte] - (stage_cycles % 34); //Subtract a random 'char'
                break;
            case 5:
                rnd_bit = (uint8_t)(rand() % 8);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit);
                break;
            case 6:
                rnd_bit = (uint8_t)(rand() % 7); //Hack because I don't want to worry about wrap around
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit + 1);
                break;
            case 7:
                rnd_bit = (uint8_t)(rand() % 5);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit + 1);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit + 2);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit + 3);
                break;
            case 8:
                input[rnd_byte] = input[rnd_byte] ^ 0xFF;
                break;
            case 9:
                rnd_byte = rnd_byte - 1;
                if (rnd_byte < 2 || rnd_byte > input_size){
					rnd_byte = 0;
                }
                input[rnd_byte] = input[rnd_byte] ^ 0xFF;
                input[rnd_byte + 1] = input[rnd_byte + 1] ^ 0xFF;
                break;
            case 10:
                rnd_byte = rnd_byte - 4;
                if (rnd_byte < 4 || rnd_byte > input_size){
                	rnd_byte = 0;
                }
                input[rnd_byte] = input[rnd_byte] ^ 0xFF;
                input[rnd_byte + 1] = input[rnd_byte + 1] ^ 0xFF;
                input[rnd_byte + 2] = input[rnd_byte + 2] ^ 0xFF;
                input[rnd_byte + 3] = input[rnd_byte + 3] ^ 0xFF;

                break;
            case 11:
                //set a random byte to zero
                input[rnd_byte] = 0;
                break;
            default:
            	break;
            }
            //TODO: Add shrinking mutator, and add test case splicing.
            }
        }

    stage_cycles++;
}


void dequeue_seed(volatile uint8_t * test_case){
/********************************************************
 * @brief dequeue_seed: handles need seeds being loaded into the input buffer
 * @param input: pointer to the input buffer
 * @return void
 ********************************************************/

    uint8_t * result;
    //At the end of queue set it back to the beggining
    //Corpus will loop continously.
    if(seed_head == seed_tail){
        seed_head = 0;
    }

   if(stage_cycles == MAX_CYCLES){
	stage_cycles = 0;
	seed_head++;
   }
   if(seed_head >= current_seed_num){
    //If we have cycled through the seed corpus the aggression may need to go down.
	seed_head = 0;
    pool_loops++;
   }

   if(pool_loops > 5){
       if(mutation_degression < 10){
           mutation_degression++;
       }
       if(mutation_degression == 10){
           mutation_degression = 1;
       }
   }
//    result = (uint8_t *)volatile_memcpy(test_case, local_pool[seed_head], sizeof(local_pool[seed_head]));
   result = (uint8_t *)memcpy(test_case, local_pool[seed_head], sizeof(local_pool[seed_head]));

    //if(result == NULL){
    //    printf("ERROR: Failed to copy seed to input buffer. \n");
    //}

}


void bubble_coverage(){
	return;
}


void harness_loop(){
/********************************************************
 * @brief main_harness_loop: harness for varrious functions
 * @param  none
 * @return void
 ********************************************************/
    uint16_t test_case_size ;
	uint16_t i;
    test_case_size = WIDTH;

    fuzzer_setup(&branch_test);

    //printf("DEBUG: Finished Setup, starting test case execution.\n");
//    setjmp(saved_context);


    while(1){

	iterations++;
	dequeue_seed(current_testcase);
//	for(i = 0; i < WIDTH; i+=2){
//		 current_testcase[i] = current_testcase[i] + (stage_cycles % 35);
//
//	 }
    mutator(current_testcase,test_case_size);

    //TODO: Implement Timing of Test Cases HERE
    //branch_test(current_testcase, test_case_size);

    if(isIncreasing && (coverage_list[0] != UINT_MAX)){
        intersting_cases++;
        bubble_coverage();

        /* Clear the coverage map*/
        for(i = 0; i < MAX_BLOCKS_PER_INPUT; i++){
            coverage_list[i] = 0;
        }
        coverage_list[0] = UINT_MAX;
        coverage_list_head = &coverage_list[0];
        isIncreasing = 0;
        mutation_degression = 1;
    }
    else if(isIncreasing){
        printf("ERROR: Coverage map is zeroed but flag is set. \n");
    }

//    longjmp(saved_context, 1);

    }
}


void board_setup(){
/*** Handle any registers we need to setup here...
 *
***/
	volatile uint32_t * SHCSR = (volatile uint32_t *)0xE000ED24;
	volatile uint32_t * VTOR = (volatile uint32_t *)0xE000ED08; // Vector Table Offset Register

//	volatile uint32_t * vector_table_address = &g_pfnVectors;

	volatile uint32_t reg_value = 0;

	reg_value = *SHCSR;
	reg_value = reg_value | 0x70000; //Enable all faults USGFAULT, BUSFAULT, MEMFAULT:bits 18,17,16
	*SHCSR = reg_value;

//	*VTOR = *vector_table_address; //Set the vector table offset register

}

/**
  * @brief  The application entry point.
  * @retval int
  */
uint16_t main(void)
{


  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  board_setup();
  /* Initialize all configured peripherals */

  harness_loop();

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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
