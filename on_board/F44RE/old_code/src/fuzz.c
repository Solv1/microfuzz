/**
 * fuzz.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <setjmp.h>

//#define NO_LOGGING


/*-------------------------------------------------------------------------------------------*/
#define TIMEOUT 100000 /*TODO: Adjust this for the right clock.*/
#define SEED_CAPACITY 15
#define MAX_BLOCKS_PER_INPUT 200
#define MAX_CYCLES 250           /*250 Cycles of mutation before getting a new seed.*/
#define WIDTH 128 //Fixed size width for now.
#define UINT_MAX 65535
/*------------------------------------------------------------------------------------------*/


/*-------------------Test Case Handling-----------------------------------------------------*/
volatile int8_t local_pool[SEED_CAPACITY][WIDTH]; //Local storage of test cases
volatile int8_t current_seed_num = 0; //Tracks which seed in the seed pool is in use

volatile uint16_t intersting_cases = 0; //The number of coverage increasing test cases so far

int8_t * current_testcase; // Pointer to current test case

uint16_t seed_head = 0;
uint16_t seed_tail = SEED_CAPACITY - 1;
uint16_t stage_cycles = 0;
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

/* Preprocessor Defines */
void crash_void();

/*-----------------------------------------------------------------------------------------*/

/*----------------------------STM32 Init-----------------------------*/


//TODO: Add interrupts HERE

void crash_void(){ //The place where all bad errors go
    while(1);
}

int16_t setup(void * function_pointer){
/********************************************************
 * @brief Inits fuzzer, testing random number generatation, and sets the coverage map to zero.
 * @param None
 * @return 0 on success, -1 on failure
********************************************************/

    current_testcase = calloc(WIDTH, sizeof(int8_t)); //Malloc memory for the testcase buffer
    if(current_testcase == NULL){
        printf("Failed to get memory for input buffer.\n");
        exit(-1);
    }

    memset(&coverage_list,0,sizeof(coverage_list));
    coverage_list[0] = UINT_MAX;

    memset(&local_pool, 0, sizeof(local_pool));
    coverage_list_head =  &coverage_list[0];

    sut_start_address = ((uint32_t *)function_pointer); /*Finds the offset in memory of the start of our sut */


    return 0;

}



void mutator(int8_t * input, size_t input_size){
/********************************************************
 * @brief Mutator:  A test generation method through mutation
 * @param input: pointer to byte input
 *        input_size: size of the input
 * @return void
 ********************************************************/
    uint16_t i, j;
    uint16_t rnd_byte;
    uint16_t rnd_bit;
    uint16_t rnd_value;
    int16_t mutation_pick;

    uint16_t start;
    uint16_t mutation_amount;
    if(input_size == 0){
        exit(-1);
    }

//Flips a random bit in the 16 bit value
#define bitflip(value, b) (value ^ (1 << (b)))
    if(mutation_degression > 2){ //If there is mutation degression calc it here.
        mutation_amount = input_size / (input_size / mutation_degression);
        if(mutation_amount == 0){
            mutation_amount = 1; //TODO: Handle different input sizes here maybe a while loop counting up to adjust?
        }
        
        start = 0;
    }
    else{
        mutation_amount = 1;
        start = 0;
    }

    /*-----Mutator Body-----*/

    if(stage_cycles < 2 && mutation_degression < 5){
        for(i = 0; i < input_size; i = i + mutation_amount){
            //Walking 1-bit flip with one bit step over
            for(j = stage_cycles % 2; j < 8; j++){
                input[i] = bitflip(input[i], j);
                j++;
            }
        }
    }

    else if(stage_cycles < 4 && mutation_degression < 5){
        for(i = 0; i < input_size; i = i + mutation_amount){
            // Walking 2-bit flip with one bit step over
            for(j = stage_cycles % 2; j < 8; j++){
                input[i] = bitflip(input[i], j);
                input[i] = bitflip(input[i], j+1);
                j = j + 2;
            }
        }
    }
    else if(stage_cycles < 6 && mutation_degression < 5){
        for(i = 0; i < input_size; i = i + mutation_amount){
            // Walking 4-bit with one bit step over
            for(j = stage_cycles % 2; j < 8; j++){
                input[i] = bitflip(input[i], j);
                input[i] = bitflip(input[i], j+1);
                input[i] = bitflip(input[i], j+2);
                input[i] = bitflip(input[i], j+3);
                j = j + 4;
            }            
        }
    }
    else if(stage_cycles < 8 && mutation_degression < 5){

        // Walking byte flip with one byte step over
        for(i = stage_cycles % 2; i < input_size; i = i + mutation_amount){
            input[i] = input[i] ^ 0xFF;
            i++;
        }
    }
    else if(stage_cycles < 10 && mutation_degression < 5){

        //Walking 2-byte flip with one byte step over
        for(i = stage_cycles % 2; i < input_size - 1 -(stage_cycles % 2); i = i + mutation_amount){
            input[i] = input[i] ^ 0xFF;
            input[i+1] = input[i+1] ^ 0xFF;
            i = i + 2;
        }
    }
    else if(stage_cycles < 12 && mutation_degression < 5){

        //Walking 32-bit flip
        for(i = stage_cycles % 2; i < input_size - 4; i = i + mutation_amount + 2){
            input[i] = input[i] ^ 0xFF;
            input[i+1] = input[i+1] ^ 0xFF;
            input[i + 2] = input[i] ^ 0xFF;
            input[i + 3] = input[i+1] ^ 0xFF;
            i = i + 4;
        }
    }
    
    else if(stage_cycles < 48 && mutation_degression < 5){

        //Add a increasing value.
        for(i = start; i < input_size; i = i + mutation_amount){
            input[i] = input[i] + (stage_cycles % 35);
            i++;
        }
    }
    else if(stage_cycles < 82 && mutation_degression < 5){
        //Subtract a increasing value.
        for(i = start; i < input_size; i = i + mutation_amount){
                input[i] = input[i] - (stage_cycles % 35);
                i++;
        }
    }
    else if(stage_cycles <= MAX_CYCLES){

        mutation_pick = rand() % 12;

        rnd_value = (rand() % (100/mutation_degression)); // Perform a random amount of random mutations

        for(i = 0; i < rnd_value; i++){
            rnd_byte = rand() % input_size;
            switch (mutation_pick)
            {
            case 0:
                input[rnd_byte] = rand() % 256; //Replace a random byte with another random byte.
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
                rnd_byte = rand() % input_size;
                rnd_bit = rand() % 8;
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit);
                break;
            case 6:
                rnd_byte = rand() % input_size;
                rnd_bit = rand() % 7; //Hack because I don't want to worry about wrap around
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit + 1);
                break;
            case 7:
                rnd_byte = rand() % input_size;
                rnd_bit = rand() % 5;
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit + 1);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit + 2);
                input[rnd_byte] = bitflip(input[rnd_byte],rnd_bit + 3);
                break;
            case 8:
                rnd_byte = rand() % input_size;
                input[rnd_byte] = input[rnd_byte] ^ 0xFF;
                break;
            case 9:
                rnd_byte = rand() % input_size - 1;
                input[rnd_byte] = input[rnd_byte] ^ 0xFF;
                input[rnd_byte + 1] = input[rnd_byte + 1] ^ 0xFF;
                break;
            case 10:
                rnd_byte = rand() % input_size - 4;
                input[rnd_byte] = input[rnd_byte] ^ 0xFF;
                input[rnd_byte + 1] = input[rnd_byte + 1] ^ 0xFF;
                input[rnd_byte + 2] = input[rnd_byte + 2] ^ 0xFF;
                input[rnd_byte + 3] = input[rnd_byte + 3] ^ 0xFF;

                break;
            case 11:
                //set a random byte to zero
                rnd_byte = rand() % input_size;
                input[rnd_byte] = 0;
                break;    
            }
            //TODO: Add shrinking mutator, and add test case splicing.
            mutation_pick = rand() % 12; //Choose another mutatation
            }
        }

    stage_cycles++;
}


void dequeue_seed(int8_t * test_case){
/********************************************************
 * @brief dequeue_seed: handles need seeds being loaded into the input buffer 
 * @param input: pointer to the input buffer
 * @return void
 ********************************************************/

    int16_t * result;
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
//    if(pool_loops > 100){
//        mutation_degression = 1;
//        pool_loops = 0;
//        }
//    }
   if(pool_loops > 5){
       if(mutation_degression < 10){
           mutation_degression++;
       }
       if(mutation_degression == 10){
           mutation_degression = 1;
       }
   }
    result = memcpy(test_case, (int8_t *)local_pool[seed_head], WIDTH);

    if(result == NULL){
        printf("ERROR: Failed to copy seed to input buffer. \n");
    }

}


void bubble_coverage(){
    int16_t i = 0;
	return;
}


void main_harness_loop(){
/********************************************************
 * @brief main_harness_loop: harness for varrious functions
 * @param  none
 * @return void
 ********************************************************/
    int16_t test_case_size ;
	uint16_t i;
    test_case_size = 35;

    setup(&branch_test);

    //printf("DEBUG: Finished Setup, starting test case execution.\n");
    setjmp(saved_context);

    
    while(1){
        
	iterations++;
	dequeue_seed(current_testcase);
    
    mutator(current_testcase,test_case_size);
    

    //TODO: Implement Timing of Test Cases HERE
    branch_test(current_testcase, test_case_size);

    if(isIncreasing && (coverage_list[0] != -1)){
        intersting_cases++;
        bubble_coverage();

        /* Clear the coverage map*/
        for(i = 0; i < MAX_BLOCKS_PER_INPUT; i++){
            coverage_list[i] = 0;
        }
        coverage_list[0] = -1;
        coverage_list_head = &coverage_list[0];
        isIncreasing = 0;
        mutation_degression = 1;
    }
    else if(isIncreasing){
        printf("ERROR: Coverage map is zeroed but flag is set. \n");
    }

    longjmp(saved_context, 1);

    }
}

int main(void){

    main_harness_loop();
}
