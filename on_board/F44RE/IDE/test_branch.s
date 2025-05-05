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
	.file	"test_branch.c"
	.text
	.align	1
	.global	branch_test
	.syntax unified
	.thumb
	.thumb_func
	.type	branch_test, %function
branch_test:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	str	r0, [r7, #4]
	str	r1, [r7]
	ldr	r3, [r7, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #68
	bne	.L2
	ldr	r3, [r7, #4]
	adds	r3, r3, #1
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #69
	bne	.L2
	ldr	r3, [r7, #4]
	adds	r3, r3, #2
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #65
	bne	.L2
	ldr	r3, [r7, #4]
	adds	r3, r3, #3
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #68
	bne	.L2
	ldr	r3, [r7, #4]
	adds	r3, r3, #4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #66
	bne	.L2
	ldr	r3, [r7, #4]
	adds	r3, r3, #5
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #69
	bne	.L2
	ldr	r3, [r7, #4]
	adds	r3, r3, #6
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #69
.L2:
	movs	r3, #0
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
	.size	branch_test, .-branch_test
	.ident	"GCC: (Arm GNU Toolchain 13.3.Rel1 (Build arm-13.24)) 13.3.1 20240614"
