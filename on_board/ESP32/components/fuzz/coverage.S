

	.global __sanitizer_cov_trace_pc
	.extern g_coverageList, g_sutStartPtr, g_covListPtr, g_isIncreasing 
	//    ;Theses variables are defined in fuzz.c
	.type __sanitizer_cov_trace_pc, %function


__sanitizer_cov_trace_pc:
    //Let's store the register context by pushing everything to the stack.
	.word g_covListPtr
	.word g_isIncreasing
	.word g_sutStartPtr
	addi a1,a1, -4 	//Decrement that stack pointer
	s32i a0, a1, 0 	//Store the register context
	addi a1, a1, -4
	s32i a2, a1, 0
	addi a1, a1, -4
	s32i a3, a1, 0
	addi a1, a1, -4
	s32i a4, a1, 0
	addi a1, a1, -4
	s32i a5, a1, 0
	addi a1, a1, -4
	s32i a6, a1, 0
	addi a1, a1, -4
	s32i a7, a1, 0
	addi a1, a1, -4
	s32i a8, a1, 0
	addi a1, a1, -4
	s32i a9, a1, 0
	addi a1, a1, -4
	s32i a10, a1, 0
	addi a1, a1, -4
	s32i a11, a1, 0
	addi a1, a1, -4
	s32i a12, a1, 0
	addi a1, a1, -4
	s32i a13, a1, 0
	addi a1, a1, -4
	s32i a14, a1, 0
	addi a1, a1, -4
	s32i a15, a1, 0

		       	//Return Address is in a8.
    	addi a15, a8, -4 //Minus 3 from the return address to get to the start of the call 24 bit instruction size.
	nop
	movi a2,  240
	movi a3,  20
	movi a4, 8
//	sll a3, a4  //Left shift by 8(a4)
	add a2, a2, a3  //20f0 OPCode for NOP in a2 now, little endian.

	s32i a2, a15, 0	//Store NOP at call location.
			//Xtensa ASM is not fun :(

    //Okay we have overwritten the coverage call, time to track where we have hit.
    	.text
    	.literal_position
	l32r a2, g_sutStartPtr //Address of call
	l32i a4, a2, 0
	
	l32r a4, g_covListPtr
	l32i a5, a4, 0

	s32i a3, a5, 0	//Store found coverage offset in the coverage list.

	addi a5, a5, 4 	//Add two to the head pointer as we are storing a 2 byte value
	l32r a4, g_covListPtr
	s32i a4, a5, 0

	l32r a2, g_isIncreasing	//Indicate when coverage is found.
	movi a3,  1
	s32i a3, a2, 0

	l32i a15, a1, 0
	addi a1, a1, 4
	l32i a14, a1, 0
	addi a1, a1, 4
	l32i a13, a1, 0
	addi a1, a1, 4
	l32i a12, a1, 0
	addi a1, a1, 4
	l32i a11, a1, 0
	addi a1, a1, 4
	l32i a10, a1, 0
	addi a1, a1, 4
	l32i a9, a1, 0
	addi a1, a1, 4
	l32i a8, a1, 0
	addi a1, a1, 4
	l32i a7, a1, 0
	addi a1, a1, 4
	l32i a6, a1, 0
	addi a1, a1, 4
	l32i a5, a1, 0
	addi a1, a1, 4
	l32i a4, a1, 0
	addi a1, a1, 4
	l32i a3, a1, 0
	addi a1, a1, 4
	l32i a2, a1, 0
	addi a1, a1, 4
	l32i a0, a1, 0
	retw

