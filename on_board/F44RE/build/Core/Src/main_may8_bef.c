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
        BYTEFLIP1_1,	        //Walking 1 byte with 1 byte step over
        BYTEFLIP2_1,	        //Walking 2 byte with 1 byte step over
        BYTEFLIP4_1,	        //Walking 4 byte with 1 byte step over
        ADDINTVALUE,	        //Add an interesting value 1-35
        SUBINTVALUE,	        //Subtract an interesting value 1-35
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
}mutationContext_t;

typedef struct {
	uint8_t seedHead;
	uint8_t seedTail;
	uint16_t poolCycles; // How many times have we gone through the pool.
}poolContext_t ;





volatile testCase_t * g_testCaseBuffer; /*Holds current testcase.*/
volatile testCase_t g_localPool[LOCAL_POOL_SIZE]; /*Local Pool for testcases.*/
volatile size_t g_nSeeds; /*How many seeds are currently loaded into the local pool*/

poolContext_t g_poolContext;
mutationContext_t g_mutContext;

/*-------------------Coverage Tracing------------------------------------------------------*/
volatile uint16_t g_coverageList[MAX_BLOCKS_PER_INPUT];
volatile uint16_t * g_covListPtr = 0;   /*Used in ASM function to point to the coverage list*/
uint32_t * g_sutStartPtr = 0;   /*Points to the start of our sut for coverage tracing*/
uint8_t g_isIncreasing = 0;
uint32_t g_iterations = 0;
/*-----------------------------------------------------------------------------------------*/

//jmp_buf saved_context;

extern int16_t branch_test(int8_t * args, int16_t argc);

void SystemClock_Config(void);


void crash_void(){ //The place where all bad errors go
        while(1);
}



int16_t fuzzer_setup(void * funPtr){
/********************************************************
 * @brief	Init for fuzzer
 * @param 	funPtr: Pointer to the start of our SUT function
 * 			mutPtr: Pointer to mutationContext
 * 			poolPtr: Pointer to poolContext
 * @return 0 on success, -1 on failure
********************************************************/


        g_mutContext.stageCycles = 0;
        g_mutContext.mutationDegression = 1;
        g_mutContext.currentMutation = BITFLIP1_1;

        g_testCaseBuffer = calloc(1, sizeof(testCase_t));

        if(g_testCaseBuffer == NULL){
                //printf("Failed to get memory for input buffer.\n");
                return -1;
        }

        g_nSeeds = 0;	/*To be loaded later by the debugger*/
        g_poolContext.poolCycles = 0;
        g_poolContext.seedHead = 0;
        g_poolContext.seedTail = LOCAL_POOL_SIZE - 1;
        memset(g_localPool, 0, sizeof(g_localPool ));


        memset(g_coverageList,0,sizeof(g_coverageList));
        g_coverageList[0] = UINT_MAX;
        g_covListPtr = g_coverageList; //Used in ASM function to point to the coverage list
        g_sutStartPtr = (uint32_t *)funPtr; /*Memory location of the SUT.*/

        return 0;

}

int16_t  mutator(void){
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



        size_t inputSize = g_testCaseBuffer->size; //For readablity
        volatile uint8_t * testCase = g_testCaseBuffer->testCase;
        /*TEST*/
        inputSize = 15;


        if(inputSize < MIN_TESTCASE_LEN || inputSize > MAX_TESTCASE_LEN){
                return -2;
        }

	/*Mutation type calced here to increase readability. Readable out of struct easier from host*/
        if(g_mutContext.stageCycles >= 192 || g_mutContext.mutationDegression > 1){
                g_mutContext.currentMutation = RANDOM;
        }
        else if(g_mutContext.stageCycles < 2){
                g_mutContext.currentMutation = BITFLIP1_1;
        }
        else if(g_mutContext.stageCycles < 4){
                g_mutContext.currentMutation = BITFLIP2_1;
        }
        else if(g_mutContext.stageCycles < 6){
                g_mutContext.currentMutation = BITFLIP4_1;
        }
        else if(g_mutContext.stageCycles < 8){
                g_mutContext.currentMutation = BYTEFLIP1_1;
        }
        else if(g_mutContext.stageCycles < 10){
                g_mutContext.currentMutation = BYTEFLIP2_1;
        }
        else if(g_mutContext.stageCycles < 12){
                g_mutContext.currentMutation = BYTEFLIP4_1;
        }
        else if(g_mutContext.stageCycles < 82){
                g_mutContext.currentMutation = ADDINTVALUE;
        }
        else if(g_mutContext.stageCycles < 152){
                g_mutContext.currentMutation = SUBINTVALUE;
        }
        else if(g_mutContext.stageCycles < 172 && g_nSeeds > 1){
        //TODO: Add checks for input size here.
                g_mutContext.currentMutation = SPLICE;
        }
        else if(g_mutContext.stageCycles < 182){
                g_mutContext.currentMutation = SHRINK;
        }
        else if(g_mutContext.stageCycles < 192){
                g_mutContext.currentMutation = PAD;
        }


/*-------------------------Mutator Body--------------------------------*/

        if(g_mutContext.currentMutation == BITFLIP1_1){
                for(i = 0; i < inputSize; i++){
                        for(j = g_mutContext.stageCycles % 2; j < 8; j += 2){
                                testCase[i] = bitflip(testCase[i], j);
                        }
                }
        }

    else if(g_mutContext.currentMutation == BITFLIP2_1){
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

        else if(g_mutContext.currentMutation == BITFLIP4_1){
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

        else if(g_mutContext.currentMutation == BYTEFLIP1_1){
                for(i = g_mutContext.stageCycles % 2; i < inputSize; i+=2){
                        testCase[i] = testCase[i] ^ 0xFF;
                }
        }

        else if(g_mutContext.currentMutation == BYTEFLIP2_1){
                for(i = g_mutContext.stageCycles % 2; i < inputSize; i++){
                        if(!(b % 2 == 0)){
                                testCase[i] = testCase[i] ^ 0xFF;
                                b++;
                        }
                        else{
                                b = 0;
                        }
                }
        }

        else if(g_mutContext.currentMutation == BYTEFLIP4_1){

                for(i = g_mutContext.stageCycles % 2; i < inputSize; i++){
                        if(!(i % 4 == 0)){
                                testCase[i] = testCase[i] ^ 0xFF;
                        }
                }
        }
        else if(g_mutContext.currentMutation == ADDINTVALUE && g_mutContext.stageCycles > 46){
                for (i = 1; i < inputSize - 1; i+=2){
                        testCase[i] = testCase[i] + (g_mutContext.stageCycles % 35);
                }
        }
        else if(g_mutContext.currentMutation == ADDINTVALUE){
                for(i = 0; i < inputSize; i+=2){
                        testCase[i] = testCase[i] + (g_mutContext.stageCycles % 35);

                }
        }
        else if(g_mutContext.currentMutation == SUBINTVALUE && g_mutContext.stageCycles > 82){
                for(i = 1; i < inputSize -1; i+=2){
                        testCase[i] = testCase[i] - (g_mutContext.stageCycles % 35);

                }
        }

        else if(g_mutContext.currentMutation == SUBINTVALUE){
                for(i = 0; i < inputSize; i+=2){
                        testCase[i] = testCase[i] - (g_mutContext.stageCycles % 35);

                }
        }

        else if(g_mutContext.currentMutation == SPLICE){
        size_t rndSeed = rand() % g_nSeeds; //Choose a random seed to SPLICE with current testcase
        size_t posCur = g_testCaseBuffer->size/2;
        size_t posNext = g_localPool[rndSeed].size/2;

        //This will eventaully cause a problem
        if(posCur == 0){
                posCur++;
        }
        if(posNext == 0){
                posNext++;
        }
        for(posCur; posCur < g_testCaseBuffer->size; posCur++){
                testCase[posCur] = g_localPool[rndSeed].testCase[posNext];
                posNext++;
                if(posNext >= g_localPool[rndSeed].size/2){
                        break;
                }
        }


        }
        else if(g_mutContext.currentMutation == SHRINK){
        rndValue = (uint8_t)rand() % (MAX_TESTCASE_LEN /2);

        if((int16_t)(g_testCaseBuffer->size - rndValue) <= MIN_TESTCASE_LEN){
                ; //Don't need to shrink this test case. It is already at a minimum.
        }
        else{
                for(i = g_testCaseBuffer->size -1; i > g_testCaseBuffer->size - rndValue; i--){
                        g_testCaseBuffer->testCase[i] = (uint8_t)'\0';
                }
                        g_testCaseBuffer->size -= rndValue;
                }
        }
        else if(g_mutContext.currentMutation == PAD){
                rndValue = (uint8_t)rand() % (MAX_TESTCASE_LEN /2);
                if((g_testCaseBuffer->size + rndValue) >= MAX_TESTCASE_LEN){
                        ;//Don't need to pad this test case. It is already at a maximal range.
                }
                else{
                        for(i = g_testCaseBuffer->size; i < g_testCaseBuffer->size + rndValue; i++){
                                g_testCaseBuffer->testCase[i] = (uint8_t)'E';
                        }
                        g_testCaseBuffer->size += rndValue;
                }
        }

        else if(g_mutContext.currentMutation == RANDOM){
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
                                testCase[rndByte] = testCase[rndByte] + (g_mutContext.stageCycles % 34); //Add a random 'char'
                                break;
                        case 2:
                                testCase[rndByte] = testCase[rndByte] + (g_mutContext.stageCycles % 256); //Add a random byte
                                break;
                        case 3:
                                testCase[rndByte] = testCase[rndByte] - (g_mutContext.stageCycles % 256); //Subtract a random byte
                                break;
                        case 4:
                                testCase[rndByte] = testCase[rndByte] - (g_mutContext.stageCycles % 34); //Subtract a random 'char'
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

        g_mutContext.stageCycles++;
        return 0;
}


void dequeue_seed(void){
/********************************************************
 * @brief dequeue_seed: handles need seeds being loaded into the input buffer
 * @param void
 * @return void
 ********************************************************/

        size_t i ;

        if(g_poolContext.seedHead == g_poolContext.seedTail){ //Circular queue of Seeds
                g_poolContext.seedHead = 0;
        }

        if(g_mutContext.stageCycles == MAX_CYCLES){ //We have done enough cycles next seed.
                g_mutContext.stageCycles = 0;
                g_poolContext.seedHead++;
        }
        if(g_poolContext.seedHead >= g_nSeeds){ //As time goes on lets get less aggressive.
                g_poolContext.seedHead = 0;
                g_poolContext.poolCycles++;
        }

        if(g_poolContext.poolCycles > 5){
                if(g_mutContext.mutationDegression < 10){
                        g_mutContext.mutationDegression++;
                }
                if(g_mutContext.mutationDegression >= 10){
                        g_mutContext.mutationDegression = 1;
                }
        }

   //Zero out the buffer.
        for(i = 0; i < MAX_TESTCASE_LEN; i++){
                g_testCaseBuffer->testCase[i] = 0;
        }

        for(i = 0; i < g_localPool[g_poolContext.seedHead].size; i++){
                g_testCaseBuffer->testCase[i] = g_localPool[g_poolContext.seedHead].testCase[i];
        }
        g_testCaseBuffer->size = g_localPool[g_poolContext.seedHead].size;

}


void bubble_coverage(){
	return;
}

void check_for_coverage(){
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

                g_mutContext.mutationDegression = 1; //We need to be more aggressive with the new test case
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
 * @param  void
 * @return void
 ********************************************************/

        int16_t retVal = 0;

        retVal = fuzzer_setup(&branch_test);

        if(retVal != 0){
                exit(-1);
        }

//    setjmp(saved_context);


        while(1){

                g_iterations++;
                dequeue_seed();

                retVal = mutator();

                if(retVal != 0){
                        exit(-1);
                }
                //TODO: Implement Timing of Test Cases HERE
                branch_test(g_testCaseBuffer->testCase,g_testCaseBuffer->size);

                check_for_coverage();
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
