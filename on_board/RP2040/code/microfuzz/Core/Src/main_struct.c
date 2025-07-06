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
#include <stdalign.h>

#include <string.h>
#include <setjmp.h>

#include "stm32f4xx_it.h"
#include "main.h"
//#include "microfuzz_mem.h"

/*-------------------------------------------------------------------------------------------*/
#define TIMEOUT 100000 /*TODO: Adjust this for the right clock.*/
/*#define SEED_CAPACITY 15*/
#define LOCAL_POOL_SIZE 15
#define MAX_BLOCKS_PER_INPUT 200
#define MAX_CYCLES 250           /*250 Cycles of mutation before getting a new seed.*/
#define MAX_TESTCASE_LEN 256 //Max size of a testcase 256 bytes
#define MIN_TESTCASE_LEN 4
#define UINT_MAX 65535
/*------------------------------------------------------------------------------------------*/

enum Mutation{
	BITFLIP1_1,		//Walking 1 bit with 1 bit step over
	BITFLIP2_1,		//Walking 2 bit with 1 bit step over
	BITFLIP4_1,		//Walking 4 bit with 1 bit step over
	BYTEFLIP1_1,	//Walking 1 byte with 1 byte step over
	BYTEFLIP2_1,	//Walking 2 byte with 1 byte step over
	BYTEFLIP4_1,	//Walking 4 byte with 1 byte step over
	ADDINTVALUE,	//Add an interesting value 1-35
	SUBINTVALUE,	//Subtract an interesting value 1-35
	RANDOM,			//Do a random amount of stacked mutations
	SPLICE,			//Take two testcases and combine them
	SHRINK,			//Decrease the size of the testcase
	PAD,			//Increase the size of the testcase
};

typedef struct {
	uint8_t testCase[MAX_TESTCASE_LEN];
	size_t size;
}testCase_t;

typedef struct {
	uint8_t stageCycles;
	uint16_t mutationDegression;
	uint16_t currentMutation;	//Current mutation pattern.
	volatile testCase_t * testCaseBuffer;	//Holds current testcase
}mutationContext_t;

typedef struct {
	uint8_t seedHead;
	uint8_t seedTail;
	uint16_t poolCycles; // How many times have we gone through the pool.
	volatile testCase_t localPool[LOCAL_POOL_SIZE]; //Local Pool for testcases.
	volatile size_t nSeeds; //How many seeds are loaded into the local pool.
}poolContext_t ;

/*-------------------Coverage Tracing------------------------------------------------------*/
volatile uint16_t g_coverageList[MAX_BLOCKS_PER_INPUT];
volatile uint16_t * g_covListPtr = 0; //Used in ASM function to point to the coverage list
uint32_t * g_sutStartPtr = 0; //Points to the start of our sut for coverage tracing
uint8_t g_isIncreasing = 0;
uint32_t g_iterations = 0;

/*-----------------------------------------------------------------------------------------*/

//jmp_buf saved_context;

extern int16_t branch_test(int8_t * args, int16_t argc);

void SystemClock_Config(void);


void crash_void(){ //The place where all bad errors go
    while(1);
}



int16_t fuzzer_setup(void * funPtr, mutationContext_t * mutPtr, poolContext_t * poolPtr){
/********************************************************
 * @brief	Init for fuzzer
 * @param 	funPtr: Pointer to the start of our SUT function
 * 			mutPtr: Pointer to mutationContext
 * 			poolPtr: Pointer to poolContext
 * @return 0 on success, -1 on failure
********************************************************/


	mutPtr->stageCycles = 0;
	mutPtr->mutationDegression = 1;
	mutPtr->currentMutation = BITFLIP1_1;
	mutPtr->testCaseBuffer = calloc(1, sizeof(testCase_t));

	if(mutPtr->testCaseBuffer == NULL){
		//printf("Failed to get memory for input buffer.\n");
		return -1;
	}


	poolPtr->nSeeds = 0;	/*To be loaded later by the debugger*/
	poolPtr->poolCycles = 0;
	poolPtr->seedHead = 0;
	poolPtr->seedTail = LOCAL_POOL_SIZE - 1;
	memset(poolPtr->localPool, 0, sizeof(poolPtr->localPool));


	memset(g_coverageList,0,sizeof(g_coverageList));
    g_coverageList[0] = UINT_MAX;

    g_covListPtr = g_coverageList; //Used in ASM function to point to the coverage list
    g_sutStartPtr = (uint32_t *)funPtr;

//    sut_start_address = ((uint32_t *)function_pointer); /*Finds the offset in memory of the start of our sut */


   return 0;

}



int16_t  mutator(mutationContext_t * mutPtr, poolContext_t * poolPtr){
/********************************************************
 * @brief Mutator:  A test generation method through mutation
 * @param mutPtr: Mutation context pointer
 * @return void
 ********************************************************/
#define bitflip(value, b) (value ^ (1 << (b)))

	size_t i,j;
	uint8_t b = 0;

    size_t rndByte = 0;
    uint8_t rndBit = 0;
    uint8_t rndValue = 0;

    uint8_t mutationPick = 0;

    //TEST
    mutPtr->testCaseBuffer->size = 15;

    size_t inputSize = mutPtr->testCaseBuffer->size; //For readablity
    volatile uint8_t * testCase = mutPtr->testCaseBuffer->testCase;



    if(mutPtr == NULL){
		return -1;
    }

    if(inputSize < MIN_TESTCASE_LEN || inputSize > MAX_TESTCASE_LEN){
        return -2;
    }

    /*Mutation type calced here to increase readability. Readable out of struct easier from host*/
    if(mutPtr->stageCycles >= 192 || mutPtr->mutationDegression > 1){
    	mutPtr->currentMutation = RANDOM;
    }
    else if(mutPtr->stageCycles < 2){
    	mutPtr->currentMutation = BITFLIP1_1;
    }
    else if(mutPtr->stageCycles < 4){
		mutPtr->currentMutation = BITFLIP2_1;
	}
    else if(mutPtr->stageCycles < 6){
		mutPtr->currentMutation = BITFLIP4_1;
	}
    else if(mutPtr->stageCycles < 8){
		mutPtr->currentMutation = BYTEFLIP1_1;
	}
    else if(mutPtr->stageCycles < 10){
		mutPtr->currentMutation = BYTEFLIP2_1;
	}
    else if(mutPtr->stageCycles < 12){
		mutPtr->currentMutation = BYTEFLIP4_1;
	}
    else if(mutPtr->stageCycles < 82){
		mutPtr->currentMutation = ADDINTVALUE;
	}
    else if(mutPtr->stageCycles < 152){
		mutPtr->currentMutation = SUBINTVALUE;
	}
    else if(mutPtr->stageCycles < 172 && poolPtr->nSeeds > 1){
    	//TODO: Add checks for input size here.
    	mutPtr->currentMutation = SPLICE;
    }
    else if(mutPtr->stageCycles < 182){
    	mutPtr->currentMutation = SHRINK;
    }
    else if(mutPtr->stageCycles < 192){
		mutPtr->currentMutation = PAD;
    }


/*-------------------------Mutator Body--------------------------------*/

    if(mutPtr->currentMutation == BITFLIP1_1){
        for(i = 0; i < inputSize; i++){
            for(j = mutPtr->stageCycles % 2; j < 8; j += 2){
				testCase[i] = bitflip(testCase[i], j);
			}
        }
    }

    else if(mutPtr->currentMutation == BITFLIP2_1){
        for(i = 0; i < inputSize; i++){

        	for(j = 0; j < 8; j++){
				if(b % 2 == 0){
					testCase[i] = bitflip(testCase[i], j);
					b++;
				}
				else{
					b = 0;
				}
			}
        }
    }

    else if(mutPtr->currentMutation == BITFLIP4_1){
        for(i = 0; i < inputSize; i++){
            for(j = 0; j < 8; j++){

            	if(b % 4 == 0){
            		testCase[i] = bitflip(testCase[i], j);
            		b++;
            	}
            	else{
            		b = 0;
            	}
            }
        }
    }

    else if(mutPtr->currentMutation == BYTEFLIP1_1){
        for(i = mutPtr->stageCycles % 2; i < inputSize; i+=2){
            testCase[i] = testCase[i] ^ 0xFF;
        }
    }

    else if(mutPtr->currentMutation == BYTEFLIP2_1){
        for(i = mutPtr->stageCycles % 2; i < inputSize; i++){

        	if(!(b % 2 == 0)){
        		testCase[i] = testCase[i] ^ 0xFF;
        		b++;
        	}
        	else{
        		b = 0;
        	}
        }
    }

    else if(mutPtr->currentMutation == BYTEFLIP4_1){

        for(i = mutPtr->stageCycles % 2; i < inputSize; i++){
        	if(!(i % 4 == 0)){
				testCase[i] = testCase[i] ^ 0xFF;
			}
        }
    }
    else if(mutPtr->currentMutation == ADDINTVALUE && mutPtr->stageCycles > 46){
         for (i = 1; i < inputSize - 1; i+=2){
         	testCase[i] = testCase[i] + (mutPtr->stageCycles % 35);
         }
    }
    else if(mutPtr->currentMutation == ADDINTVALUE){
        for(i = 0; i < inputSize; i+=2){
        	testCase[i] = testCase[i] + (mutPtr->stageCycles % 35);

        }
   }
    else if(mutPtr->currentMutation == SUBINTVALUE && mutPtr->stageCycles > 82){
        for(i = 1; i < inputSize -1; i+=2){
                testCase[i] = testCase[i] - (mutPtr->stageCycles % 35);

        }
    }

    else if(mutPtr->currentMutation == SUBINTVALUE){
        for(i = 0; i < inputSize; i+=2){
                testCase[i] = testCase[i] - (mutPtr->stageCycles % 35);

        }
    }

    else if(mutPtr->currentMutation == SPLICE){
    	size_t rndSeed = rand() % poolPtr->nSeeds; //Choose a random seed to SPLICE with current testcase
    	size_t posCur = mutPtr->testCaseBuffer->size/2;
    	size_t posNext = poolPtr->localPool[rndSeed].size/2;

    	//This will eventaully cause a problem
    	if(posCur == 0){
    		posCur++;
    	}
    	if(posNext == 0){
    		posNext++;
    	}
    	for(posCur; posCur < mutPtr->testCaseBuffer->size; posCur++){
    		testCase[posCur] = poolPtr->localPool[rndSeed].testCase[posNext];
    		posNext++;
    		if(posNext >= poolPtr->localPool[rndSeed].size/2){
    			break;
    		}
    	}


    }
    else if(mutPtr->currentMutation == SHRINK){
    	rndValue = (uint8_t)rand() % (MAX_TESTCASE_LEN /2);

    	if((int16_t)(mutPtr->testCaseBuffer->size - rndValue) <= MIN_TESTCASE_LEN){
    		; //Don't need to shrink this test case. It is already at a minimum.
    	}
    	else{
    		for(i = mutPtr->testCaseBuffer->size -1; i > mutPtr->testCaseBuffer->size - rndValue; i--){
    			mutPtr->testCaseBuffer->testCase[i] = (uint8_t)'\0';
    		}
    		mutPtr->testCaseBuffer->size -= rndValue;
    	}
    }
    else if(mutPtr->currentMutation == PAD){
    	rndValue = (uint8_t)rand() % (MAX_TESTCASE_LEN /2);
    	if((mutPtr->testCaseBuffer->size + rndValue) >= MAX_TESTCASE_LEN){
			;//Don't need to pad this test case. It is already at a maximal range.
		}
		else{
			for(i = mutPtr->testCaseBuffer->size; i < mutPtr->testCaseBuffer->size + rndValue; i++){
				mutPtr->testCaseBuffer->testCase[i] = (uint8_t)'E';
			}
			mutPtr->testCaseBuffer->size += rndValue;
		}
    }

    else if(mutPtr->currentMutation == RANDOM){
    	//TODO: Interface with the mutaiton degression here
//        if(mutation_degression == 0){
//        	exit(-1);
//        }

        rndValue = (uint8_t)rand() % 100; // Perform a random amount of random mutations

        for(i = 0; i < rndValue; i++){
        	mutationPick = (uint8_t)rand() % 12; //Choose another mutatation

            rndByte = (uint8_t)rand() % inputSize;

            switch (mutationPick)
            {
            case 0:
                testCase[rndByte] = (uint8_t)rand() % 256; //Replace a random byte with another random byte.
                break;
            case 1:
                testCase[rndByte] = testCase[rndByte] + (mutPtr->stageCycles % 34); //Add a random 'char'
                break;
            case 2:
                testCase[rndByte] = testCase[rndByte] + (mutPtr->stageCycles % 256); //Add a random byte
                break;
            case 3:
                testCase[rndByte] = testCase[rndByte] - (mutPtr->stageCycles % 256); //Subtract a random byte
                break;
            case 4:
                testCase[rndByte] = testCase[rndByte] - (mutPtr->stageCycles % 34); //Subtract a random 'char'
                break;
            case 5:
                rndBit = ((uint8_t)rand() % 8);
                testCase[rndByte] = bitflip(testCase[rndByte],rndBit);
                break;
            case 6:
                rndBit = ((uint8_t)rand() % 7);
                testCase[rndByte] = bitflip(testCase[rndByte],rndBit);
                testCase[rndByte] = bitflip(testCase[rndByte],rndBit + 1);
                break;
            case 7:
                rndBit = ((uint8_t)rand() % 5);
                testCase[rndByte] = bitflip(testCase[rndByte],rndBit);
                testCase[rndByte] = bitflip(testCase[rndByte],rndBit + 1);
                testCase[rndByte] = bitflip(testCase[rndByte],rndBit + 2);
                testCase[rndByte] = bitflip(testCase[rndByte],rndBit + 3);
                break;
            case 8:
                testCase[rndByte] = testCase[rndByte] ^ 0xFF;
                break;
            case 9:
                rndByte = rndByte - 1;
                if (rndByte < 2 || rndByte > inputSize){ rndByte = 0;}

                testCase[rndByte] = testCase[rndByte] ^ 0xFF;
                testCase[rndByte + 1] = testCase[rndByte + 1] ^ 0xFF;
                break;
            case 10:
                rndByte = rndByte - 4;

                if (rndByte < 4 || rndByte > inputSize){rndByte = 0;}

                testCase[rndByte] = testCase[rndByte] ^ 0xFF;
                testCase[rndByte + 1] = testCase[rndByte + 1] ^ 0xFF;
                testCase[rndByte + 2] = testCase[rndByte + 2] ^ 0xFF;
                testCase[rndByte + 3] = testCase[rndByte + 3] ^ 0xFF;

                break;
            case 11:
                //set a random byte to zero
                testCase[rndByte] = 0;
                break;
            default:
            	break;
            }
            //TODO: Add shrinking mutator, and add test case splicing.
            }
        }

    mutPtr->stageCycles++;
    return 0;
}


void dequeue_seed(mutationContext_t * mutPtr, poolContext_t * poolPtr){
/********************************************************
 * @brief dequeue_seed: handles need seeds being loaded into the input buffer
 * @param mutPtr: mutation context
 * 		  poolPtr: pool context
 * @return void
 ********************************************************/

    size_t i ;

    if(poolPtr->seedHead == poolPtr->seedTail){ //Circular queue of Seeds
        poolPtr->seedHead = 0;
    }

   if(mutPtr->stageCycles == MAX_CYCLES){ //We have done enough cycles next seed.
	   mutPtr->stageCycles = 0;
	   poolPtr->seedHead++;
   }
   if(poolPtr->seedHead >= poolPtr->nSeeds){ //As time goes on lets get less aggressive.
	poolPtr->seedHead = 0;
    poolPtr->poolCycles++;
   }

   if(poolPtr->poolCycles > 5){
       if(mutPtr->mutationDegression < 10){
    	   mutPtr->mutationDegression++;
       }
       if(mutPtr->mutationDegression >= 10){
    	   mutPtr->mutationDegression = 1;
       }
   }

   //Zero out the buffer.
   for(i = 0; i < MAX_TESTCASE_LEN; i++){
   	   mutPtr->testCaseBuffer->testCase[i] = 0;
   }

   for(i = 0; i < poolPtr->localPool[poolPtr->seedHead].size; i++){
	   mutPtr->testCaseBuffer->testCase[i] = poolPtr->localPool[poolPtr->seedHead].testCase[i];
   }
   mutPtr->testCaseBuffer->size = poolPtr->localPool[poolPtr->seedHead].size;

}


void bubble_coverage(){
	return;
}

void check_for_coverage(mutationContext_t * mutPtr){
	size_t i ;

    if(g_isIncreasing && (g_coverageList[0] != UINT_MAX)){
//        intersting_cases++;
        bubble_coverage();
    	//Breakpoint HERE
        /* Clear the coverage map*/
        for(i = 0; i < MAX_BLOCKS_PER_INPUT; i++){
            g_coverageList[i] = 0;
        }
        g_coverageList[0] = UINT_MAX;
        g_covListPtr = &g_coverageList[0];
        g_isIncreasing = 0;

        mutPtr->mutationDegression = 1; //We need to be more aggressive with the new test case
    }
    else if(g_isIncreasing){
    	//Error condition: increasing coverage indicated but list empty...
//        printf("ERROR: Coverage map is zeroed but flag is set. \n");
        exit(-1);
    }
    else{
    	return;
    }

}

void harness_loop(){
/********************************************************
 * @brief harness_loop: fuzzing harness
 * @param  none
 * @return void
 ********************************************************/
	mutationContext_t mutC;
	poolContext_t poolC;

	int16_t retVal = 0;

    retVal = fuzzer_setup(&branch_test, &mutC, &poolC);

    if(retVal != 0){
    	exit(-1);
    }

//    setjmp(saved_context);


    while(1){

    g_iterations++;
	dequeue_seed(&mutC,&poolC);

    retVal = mutator(&mutC, &poolC);

    if(retVal != 0){
    	exit(-1);
    }
    //TODO: Implement Timing of Test Cases HERE
    branch_test(mutC.testCaseBuffer->testCase,mutC.testCaseBuffer->size);

    check_for_coverage(&mutC);


//    longjmp(saved_context, 1);

    }
}


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
