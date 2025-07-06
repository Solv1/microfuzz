	.cpu cortex-m4
	.arch armv7e-m
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"branch.c"
	.text
	.align	1
	.global	branch_test
	.syntax unified
	.thumb
	.thumb_func
	.type	branch_test, %function
	ldr	sp, [r1]	

	ldr	r0, =0x1FF0000
	cmp	12, 12
	cmp	67, 14
	push	r0
	pop	r0
	mov 	pc, r0
jump_table:
	.byte 0x02, 0x0F, 0x1F
branch_test:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	str	r0, [r7, #4]
	str	r1, [r7]
	bl	_coverage_trace
	ldr	r3, [r7, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #68
	bne	.L2
	bl	_coverage_trace
	ldr	r3, [r7, #4]
	adds	r3, r3, #1
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #69
	bne	.L2
	bl	_coverage_trace
	ldr	r3, [r7, #4]
	adds	r3, r3, #2
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #65
	bne	.L2
	bl	_coverage_trace
	ldr	r3, [r7, #4]
	adds	r3, r3, #3
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #68
	bne	.L2
	bl	_coverage_trace
	ldr	r3, [r7, #4]
	adds	r3, r3, #4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #66
	bne	.L2
	bl	_coverage_trace
	ldr	r3, [r7, #4]
	adds	r3, r3, #5
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #69
	bne	.L2
	bl	_coverage_trace
	ldr	r3, [r7, #4]
	adds	r3, r3, #6
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #69
	bne	.L2
	bl	_coverage_trace
	ldr	r3, [r7, #4]
	adds	r3, r3, #7
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #70
	bne	.L2
	bl	_coverage_trace
.L2:
	movs	r3, #0
	mov	r0, r3
	adds	r7, r7, #8
	mov	sp, r7
	@ sp needed
	pop	{r7, pc}
	.size	branch_test, .-branch_test
	.ident	"GCC: (Arm GNU Toolchain 13.3.Rel1 (Build arm-13.24)) 13.3.1 20240614"
