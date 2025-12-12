#include "../../Inc/fuzz.h"
#include "../../Inc/harness.h"

volatile uint32_t fuzzer_return __attribute__((section(".fuzzbss")));
volatile cpuState_t reg_context __attribute__((section(".fuzzbss")));
volatile testCase_t * g_testCaseBuffer __attribute__((section(".fuzzbss")));
void (*cleanup_function)(void) __attribute__((section(".fuzzdata"))) = &execution_cleanup;

volatile testCase_t g_localPool[LOCAL_POOL_SIZE] __attribute__((section(".fuzzbss")));
volatile uint8_t g_nSeeds __attribute__((section(".fuzzbss"))); /*Used to be a size_t, we will never have more then 255 seeds at a time, makes host side easier with uint8_t*/

poolContext_t g_poolContext __attribute__((section(".fuzzbss")));
mutationContext_t g_mutContext __attribute__((section(".fuzzbss")));

volatile uint32_t g_randomSeed __attribute__((section(".fuzzbss")));

volatile uint32_t g_coverageList[25] __attribute__((section(".fuzzbss")));
volatile uint32_t * g_covListPtr __attribute__((section(".fuzzbss"))) = 0;
uint32_t * g_sutStartPtr __attribute__((section(".fuzzbss"))) = 0;
uint8_t g_isIncreasing __attribute__((section(".fuzzbss"))) = 0;
volatile uint32_t g_iterations __attribute__((section(".fuzzbss"))) = 0;
volatile uint8_t g_isReset __attribute__((section(".fuzzbss"))) = 0;
uint8_t g_isRestore __attribute__((section(".fuzzbss"))) = 0;
extern void * g_sutPtr __attribute__((section(".fuzzbss")));

extern uint32_t * _targetdatastart;
extern uint32_t * _targetdataend;

extern uint32_t * _targetbssstart;
extern uint32_t * _targetbssend;

uint8_t * g_dataContext __attribute__((section(".fuzzbss")));


/*******************************************************
 * @breif	Handles crash condtions. Some may not need a reset.
 * @param	none
 * @return 	void -> will jump back to the fuzzing loop.
 *******************************************************/
void crash_handler(void){
	return;
	/*Set breakpoint here.*/
}
void HardFault_Handler(void) {
	crash_handler();

}



/*******************************************************
 * @breif	state_restore
 * @disc	restores global state back to first execution. 
 * @return 	void
 *******************************************************/
void state_restore(void) {
	if (g_isRestore){
		uint32_t diff;
		diff = ((uint32_t)&_targetdataend - (uint32_t)&_targetdatastart);
		memcpy(&_targetdatastart, g_dataContext, diff);
		diff = ((uint32_t)&_targetbssend - (uint32_t)&_targetbssstart);
		memset(&_targetbssstart, 0, diff);
	}
}


/********************************************************
 * @brief	fuzzer_setup
 * @disc	handles the intialization of important fuzzing functions and ds's.
 * @param 	funPtr: Pointer to the start of our SUT function
 * @return 0 on success, -1 on failure
********************************************************/
int16_t fuzzer_setup(void * funPtr){

	int16_t ret = 0;

	ret = setup_fuzz_time();
	
        g_nSeeds = 0;	//To be loaded later by the debugger

        g_poolContext.poolCycles = 0;
        g_poolContext.seedHead = 0;
        g_poolContext.seedTail = LOCAL_POOL_SIZE - 1;
        g_mutContext.stageCycles = 0;
        g_mutContext.mutationDegression = 1;
        g_mutContext.currentMutation = BITFLIP1_1;

        g_covListPtr = g_coverageList; //Used in ASM function to point to the coverage list
        g_sutStartPtr = (uint32_t *)funPtr; //Memory location of the SUT.

	__asm volatile("bkpt \n\t");

	srand(g_randomSeed);

        g_testCaseBuffer = calloc(1, sizeof(testCase_t));

        if(g_testCaseBuffer == NULL){
                //printf("Failed to get memory for input buffer.\n");
                return -1;
        }

	//	for (int i = 0; i < MAX_TESTCASE_SIZE; i++){
	//		g_testCaseBuffer.testcase[i] = 0;
	//	}
	//	g_testCaseBuffer.size = 0;

	

	#ifdef STD_LIBRARY
        	memset(g_localPool, 0, sizeof(g_localPool ));
        	memset(g_coverageList,0,sizeof(g_coverageList));
	#else
		for (int i = 0; i < LOCAL_POOL_SIZE; i++) {
			for (int j = 0; j < MAX_TESTCASE_LEN; j++) {
				g_localPool[i].testCase[j] = 0;
			}
			g_localPool[i].size = 0;
		}
		
		for (int i = 0; i < sizeof(g_coverageList); i++){
			g_coverageList[i] = 0;
		}
		
	#endif
	uint32_t diff = ((uint32_t)&_targetdataend - (uint32_t)&_targetdatastart);
	if (diff > 0) {
		g_dataContext = calloc(diff , sizeof(uint8_t));
		memcpy(g_dataContext, &_targetdatastart, diff);
                g_isRestore = 0;
	}
	diff = ((uint32_t)&_targetbssend - (uint32_t)&_targetbssstart);
	if (diff > 0) {
		g_isRestore = 1;
	}


        g_coverageList[0] = UINT_MAX;
        return 0;

}

/********************************************************
 * @brief Mutator:  A test generation method through mutation
 * @param mutPtr: Mutation context pointer
 * @return void
 ********************************************************/
int16_t  mutator(void){
	#define bitflip(value, b) (value ^ (1 << (b)))

        size_t i,j;
        uint8_t b = 0;

        size_t rndByte = 0;
        uint8_t rndBit = 0;
        uint8_t rndValue = 0;

        uint8_t mutationPick = 0;



        size_t inputSize = g_testCaseBuffer->size; //For readablity
        volatile uint8_t * testCase = g_testCaseBuffer->testCase;
       	if (inputSize < 15){
		inputSize = 15;
	}


        if(inputSize < MIN_TESTCASE_LEN || inputSize > MAX_TESTCASE_LEN){
                return -2;
        }

	//Mutation type calced here to increase readability. Readable out of struct easier from host
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
	if(g_mutContext.mutationDegression == 0){
		g_mutContext.mutationDegression = 1;
	}

                rndValue = (uint8_t)rand() % (100/g_mutContext.mutationDegression); // Perform a random amount of random mutations

                for(i = 0; i < rndValue; i++){
                        mutationPick = (uint8_t)rand() % 12; //Choose another mutatation

                        rndByte = (uint8_t)rand() % inputSize;

                        switch (mutationPick)
                        {
                        case 0:
                                testCase[rndByte] = (uint8_t)rand() % 256;
                                break;
                        case 1:
                                testCase[rndByte] = testCase[rndByte] + (rand() % 34); 
                                break;
                        case 2:
                                testCase[rndByte] = testCase[rndByte] + (rand() % 256); //Add a random byte
                                break;
                        case 3:
                                testCase[rndByte] = testCase[rndByte] - (rand()  % 256); //Subtract a random byte
                                break;
                        case 4:
                                testCase[rndByte] = testCase[rndByte] - (rand()  % 34); //Subtract a random 'char'
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

/********************************************************
 * @brief dequeue_seed: handles need seeds being loaded into the input buffer
 * @param void
 * @return void
 ********************************************************/
void dequeue_seed(void){

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


/********************************************************
 * @brief 	bubble_coverage
 * @disc	a function blank to set a breakpoint on. 
 * @return 	void
 ********************************************************/
void bubble_coverage(){
	g_iterations = 0;
	return;
}


/********************************************************
 * @brief 	check_for_coverage
 * @disc	checks to see for increasing coverage and then clears coverage list for next iteration.
 * @return 	void
 ********************************************************/
void check_for_coverage(){

	size_t i ;

        if(g_isIncreasing && (g_coverageList[0] != UINT_MAX)){
        //        intersting_cases++;
                bubble_coverage();
                //Breakpoint HERE
                // Clear the coverage map
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

	if (g_isReset > 0) {
		g_iterations = 0;
		g_isReset = 0;
	}

}


/********************************************************
 * @brief 	fuzzing_loop
 * @disc	Main fuzzing loop, handles mutation, execution, coverage tracking, and state resotration.
 * @return 	void
 ********************************************************/
void fuzzing_loop() {

	int16_t retVal = 0;
	
	retVal = fuzzer_setup(g_sutPtr);

	if (retVal != 0) {
		exit(-1);
	}
	
	__asm volatile (
		"ldr r0, = reg_context\n\t"
		"add r0, r0, #4\n\t"
		"stmia.w r0!, {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}\n\t"
		"mov r1, sp\n\t"
		"str r1, [r0]\n\t"
		"add r0, r0, #4\n\t"
		"mov r1, pc\n\t"
		"add r1, r1, #5\n\t"
		"str r1, [r0]\n\t"
		"nop\n\t"
		"nop\n\t"
	);
	while(1) {
		state_restore();
		g_iterations++;
		dequeue_seed();

		retVal = mutator();
		if (retVal != 0){
			exit(-1);
		}
		start_fuzz_time();
		target_harness(g_testCaseBuffer->testCase, g_testCaseBuffer->size);
		end_fuzz_time();

		check_for_coverage();
	}
}


/*******************************************************
 * @breif exit/_exit wrapper functions
 * @disc 	these function wrappers handle exit calls instead of crashing out.
 * @return	will jump back to the start of the fuzzing loop.
 *******************************************************/
void __wrap_exit(int status) {
	check_for_coverage();
	cleanup_function();
}
void __wrap__exit() {
	check_for_coverage();
	cleanup_function();
}



