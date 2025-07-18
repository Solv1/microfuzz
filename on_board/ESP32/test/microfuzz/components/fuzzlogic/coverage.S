    //.global __sanitizer_cov_trace_pc_guard

    .global __sanitizer_cov_trace_pc
    .extern g_coverageList, g_sutStartPtr, g_covListPtr, g_isIncreasing //Theses variables are defined in fuzz.c

//_coverage_trace:
//__sanitizer_cov_trace_pc_guard:
__sanitizer_cov_trace_pc:
    //Let's store the register context by pushing everything to the stack
    RET
    /*.
    PUSH {lr}

    PUSH {r0}
    PUSH {r1}
    PUSH {r2}
    PUSH {r3}
    PUSH {r4}
    PUSH {r5}
    PUSH {r6}
    PUSH {r7}
    SUB sp, sp, #12				//Maintain stack alignment
    //PUSH {r8}
    //PUSH {r9}
    //PUSH {r10}
    //PUSH {r11}
    //PUSH {r12}

    MOV r0, lr                  //Store Return Address in R0.
    SUB r0, #5                   //Minus 5 from the return address to get to the start of the call .
//    SUB r0, #4
    NOP

    MOV r1, #0xBF               //Store NOP instruction in R1. Only can fit 0xBF
    LSL r1, r1, #8				//Logical Left Shift to 0xBF00
    STRH r1, [r0]               //Overwrite coverage call. Stores half a word which is 16-bits
    ADD r0, #2					//Move to next half word
    STRH r1, [r0]				//Write another NOP instruction at next half word
    							//NOTE: Calls are word sized while Nops are half-word sized.
    //Okay we have overwritten the coverage call, time to track where we have hit.
    LDR r1, =g_sutStartPtr
    LDR r2, [r1]
    SUB r0, #2					//So alignment is back to normal.
//    SUB r0, r0, r2              //RetAddress - SutStartAddress = list value.

    LDR r2, =g_covListPtr
    LDR r3, [r2]
    STR r0, [r3]                //Store the found coverage offset in the list.

    ADD r3, #4                  //Add two to the head pointer as we are storing a 2 byte value.
    STR r3, [r2]                //Store the new head pointer value.

    LDR r4, =g_isIncreasing
    LDR r3, [r4]
    ADD r3, r3, #1
    STR r3, [r4]

    //POP {r12}
    //POP {r11}
    //POP {r10}
    //POP {r9}
    //POP {r8}
    ADD sp, sp, #12 				//Align the stack
    POP {r7}
    POP {r6}
    POP {r5}
    POP {r4}
    POP {r3}
    POP {r2}
    POP {r1}
    POP {r0}
    //POP {lr}
    //BX lr
    POP {pc}*/
