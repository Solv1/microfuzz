;/*********************************************************************************/
;@brief _coverage_trace: inline assembly function for tracing coverage on STM32F446RE
;@param NONE
;@return NONE
;/***********************************************************************************/
    .global _coverage_trace
    .extern coverage_list, sut_start_address, coverage_list_head, isIncreasing //Theses variables are defined in fuzz.c

_coverage_trace:
    //Let's store the register context by pushing everything to the stack.
    PUSH {r0}
    PUSH {r1}
    PUSH {r2}
    PUSH {r3}
    PUSH {r4}
    PUSH {r5}
    PUSH {r6}
    PUSH {r7}
    //PUSH {r8}
    //PUSH {r9}
    //PUSH {r10}
    //PUSH {r11}
    //PUSH {r12}

    MOV r0, lr                  //Store Return Address in R0.
    SUB r0, #1                   //Minus 1 from the return address to get to the start of the call .

    MOV r1, #0x00000000               //Store NOP instruction in R1
    STR r1, [r0]                  //Overwrite coverage call.
    //Okay we have overwritten the coverage call, time to track where we have hit.

    LDR r1, =sut_start_address
    LDR r2, [r1]
    SUB r0, r0, r2              //RetAddress - SutStartAddress = list value.

    LDR r2, =coverage_list_head
    LDR r3, [r2]
    STR r0, [r3]                //Store the found coverage offset in the list.

    ADD r3, #1                  //Add one to the head pointer.
    STR r3, [r2]                //Store the new head pointer value.

    LDR r4, =isIncreasing
    LDR r3, [r4]
    ADD r3, r3, #1
    STR r3, [r4]

    //POP {r12}
    //POP {r11}
    //POP {r10}
    //POP {r9}
    //POP {r8}
    POP {r7}
    POP {r6}
    POP {r5}
    POP {r4}
    POP {r3}
    POP {r2}
    POP {r1}
    POP {r0, pc}
