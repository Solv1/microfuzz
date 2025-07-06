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
	.file	"fuzz.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "./fuzz.c"
	.global	local_pool
	.bss
	.align	2
	.type	local_pool, %object
	.size	local_pool, 1920
local_pool:
	.space	1920
	.global	current_seed_num
	.type	current_seed_num, %object
	.size	current_seed_num, 1
current_seed_num:
	.space	1
	.global	intersting_cases
	.align	1
	.type	intersting_cases, %object
	.size	intersting_cases, 2
intersting_cases:
	.space	2
	.global	current_testcase
	.align	2
	.type	current_testcase, %object
	.size	current_testcase, 4
current_testcase:
	.space	4
	.global	seed_head
	.align	1
	.type	seed_head, %object
	.size	seed_head, 2
seed_head:
	.space	2
	.global	seed_tail
	.data
	.align	1
	.type	seed_tail, %object
	.size	seed_tail, 2
seed_tail:
	.short	14
	.global	stage_cycles
	.bss
	.align	1
	.type	stage_cycles, %object
	.size	stage_cycles, 2
stage_cycles:
	.space	2
	.global	pool_loops
	.align	1
	.type	pool_loops, %object
	.size	pool_loops, 2
pool_loops:
	.space	2
	.global	iterations
	.align	1
	.type	iterations, %object
	.size	iterations, 2
iterations:
	.space	2
	.global	mutation_degression
	.data
	.align	1
	.type	mutation_degression, %object
	.size	mutation_degression, 2
mutation_degression:
	.short	1
	.global	coverage_list
	.bss
	.align	2
	.type	coverage_list, %object
	.size	coverage_list, 400
coverage_list:
	.space	400
	.global	coverage_list_head
	.align	2
	.type	coverage_list_head, %object
	.size	coverage_list_head, 4
coverage_list_head:
	.space	4
	.global	sut_start_address
	.align	2
	.type	sut_start_address, %object
	.size	sut_start_address, 4
sut_start_address:
	.space	4
	.global	isIncreasing
	.type	isIncreasing, %object
	.size	isIncreasing, 1
isIncreasing:
	.space	1
	.global	saved_context
	.align	3
	.type	saved_context, %object
	.size	saved_context, 160
saved_context:
	.space	160
	.text
	.align	1
	.global	crash_void
	.syntax unified
	.thumb
	.thumb_func
	.type	crash_void, %function
crash_void:
.LFB3:
	.loc 1 61 18
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	.cfi_def_cfa_offset 4
	.cfi_offset 7, -4
	add	r7, sp, #0
	.cfi_def_cfa_register 7
.L2:
	.loc 1 62 10
	b	.L2
	.cfi_endproc
.LFE3:
	.size	crash_void, .-crash_void
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Failed to get memory for input buffer.\000"
	.text
	.align	1
	.global	setup
	.syntax unified
	.thumb
	.thumb_func
	.type	setup, %function
setup:
.LFB4:
	.loc 1 65 39
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #8
	.cfi_def_cfa_offset 16
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	str	r0, [r7, #4]
	.loc 1 72 24
	movs	r1, #1
	movs	r0, #128
	bl	calloc
	mov	r3, r0
	mov	r2, r3
	.loc 1 72 22 discriminator 1
	ldr	r3, .L6
	str	r2, [r3]
	.loc 1 73 25
	ldr	r3, .L6
	ldr	r3, [r3]
	.loc 1 73 7
	cmp	r3, #0
	bne	.L4
	.loc 1 74 9
	ldr	r0, .L6+4
	bl	puts
	.loc 1 75 9
	mov	r0, #-1
	bl	exit
.L4:
	.loc 1 78 5
	mov	r2, #400
	movs	r1, #0
	ldr	r0, .L6+8
	bl	memset
	.loc 1 79 22
	ldr	r3, .L6+8
	movw	r2, #65535
	strh	r2, [r3]	@ movhi
	.loc 1 81 5
	mov	r2, #1920
	movs	r1, #0
	ldr	r0, .L6+12
	bl	memset
	.loc 1 82 24
	ldr	r3, .L6+16
	ldr	r2, .L6+8
	str	r2, [r3]
	.loc 1 84 23
	ldr	r2, .L6+20
	ldr	r3, [r7, #4]
	str	r3, [r2]
	.loc 1 87 12
	movs	r3, #0
	.loc 1 89 1
	mov	r0, r3
	adds	r7, r7, #8
	.cfi_def_cfa_offset 8
	mov	sp, r7
	.cfi_def_cfa_register 13
	@ sp needed
	pop	{r7, pc}
.L7:
	.align	2
.L6:
	.word	current_testcase
	.word	.LC0
	.word	coverage_list
	.word	local_pool
	.word	coverage_list_head
	.word	sut_start_address
	.cfi_endproc
.LFE4:
	.size	setup, .-setup
	.align	1
	.global	mutator
	.syntax unified
	.thumb
	.thumb_func
	.type	mutator, %function
mutator:
.LFB5:
	.loc 1 93 48
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #24
	.cfi_def_cfa_offset 32
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	str	r0, [r7, #4]
	str	r1, [r7]
	.loc 1 108 7
	ldr	r3, [r7]
	cmp	r3, #0
	bne	.L9
	.loc 1 109 9
	mov	r0, #-1
	bl	exit
.L9:
	.loc 1 114 28
	ldr	r3, .L61
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 114 7
	cmp	r3, #2
	bls	.L10
	.loc 1 115 52
	ldr	r3, .L61
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	mov	r2, r3
	ldr	r3, [r7]
	udiv	r3, r3, r2
	.loc 1 115 38
	ldr	r2, [r7]
	udiv	r3, r2, r3
	.loc 1 115 25
	strh	r3, [r7, #14]	@ movhi
	.loc 1 116 11
	ldrh	r3, [r7, #14]
	cmp	r3, #0
	bne	.L11
	.loc 1 117 29
	movs	r3, #1
	strh	r3, [r7, #14]	@ movhi
.L11:
	.loc 1 120 15
	movs	r3, #0
	strh	r3, [r7, #16]	@ movhi
	b	.L12
.L10:
	.loc 1 123 25
	movs	r3, #1
	strh	r3, [r7, #14]	@ movhi
	.loc 1 124 15
	movs	r3, #0
	strh	r3, [r7, #16]	@ movhi
.L12:
	.loc 1 129 21
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 129 7
	cmp	r3, #1
	bhi	.L13
	.loc 1 129 48 discriminator 1
	ldr	r3, .L61
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 129 25 discriminator 1
	cmp	r3, #4
	bhi	.L13
	.loc 1 130 15
	movs	r3, #0
	strh	r3, [r7, #22]	@ movhi
	.loc 1 130 9
	b	.L14
.L17:
	.loc 1 132 34
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 132 19
	and	r3, r3, #1
	strh	r3, [r7, #20]	@ movhi
	.loc 1 132 13
	b	.L15
.L16:
	.loc 1 133 28
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #20]
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 133 22
	ldrh	r3, [r7, #22]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 133 28
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 133 26
	strb	r2, [r3]
	.loc 1 134 18
	ldrh	r3, [r7, #20]
	adds	r3, r3, #1
	strh	r3, [r7, #20]	@ movhi
	.loc 1 132 47 discriminator 3
	ldrh	r3, [r7, #20]
	adds	r3, r3, #1
	strh	r3, [r7, #20]	@ movhi
.L15:
	.loc 1 132 41 discriminator 1
	ldrh	r3, [r7, #20]
	cmp	r3, #7
	bls	.L16
	.loc 1 130 38 discriminator 2
	ldrh	r2, [r7, #22]	@ movhi
	ldrh	r3, [r7, #14]	@ movhi
	add	r3, r3, r2
	strh	r3, [r7, #22]	@ movhi
.L14:
	.loc 1 130 22 discriminator 1
	ldrh	r3, [r7, #22]
	ldr	r2, [r7]
	cmp	r2, r3
	bhi	.L17
	.loc 1 129 7
	b	.L18
.L13:
	.loc 1 139 26
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 139 12
	cmp	r3, #3
	bhi	.L19
	.loc 1 139 53 discriminator 1
	ldr	r3, .L61
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 139 30 discriminator 1
	cmp	r3, #4
	bhi	.L19
	.loc 1 140 15
	movs	r3, #0
	strh	r3, [r7, #22]	@ movhi
	.loc 1 140 9
	b	.L20
.L23:
	.loc 1 142 34
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 142 19
	and	r3, r3, #1
	strh	r3, [r7, #20]	@ movhi
	.loc 1 142 13
	b	.L21
.L22:
	.loc 1 143 28
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #20]
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 143 22
	ldrh	r3, [r7, #22]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 143 28
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 143 26
	strb	r2, [r3]
	.loc 1 144 28
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #20]
	adds	r3, r3, #1
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 144 22
	ldrh	r3, [r7, #22]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 144 28
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 144 26
	strb	r2, [r3]
	.loc 1 145 19
	ldrh	r3, [r7, #20]	@ movhi
	adds	r3, r3, #2
	strh	r3, [r7, #20]	@ movhi
	.loc 1 142 47 discriminator 3
	ldrh	r3, [r7, #20]
	adds	r3, r3, #1
	strh	r3, [r7, #20]	@ movhi
.L21:
	.loc 1 142 41 discriminator 1
	ldrh	r3, [r7, #20]
	cmp	r3, #7
	bls	.L22
	.loc 1 140 38 discriminator 2
	ldrh	r2, [r7, #22]	@ movhi
	ldrh	r3, [r7, #14]	@ movhi
	add	r3, r3, r2
	strh	r3, [r7, #22]	@ movhi
.L20:
	.loc 1 140 22 discriminator 1
	ldrh	r3, [r7, #22]
	ldr	r2, [r7]
	cmp	r2, r3
	bhi	.L23
	.loc 1 139 12
	b	.L18
.L19:
	.loc 1 149 26
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 149 12
	cmp	r3, #5
	bhi	.L24
	.loc 1 149 53 discriminator 1
	ldr	r3, .L61
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 149 30 discriminator 1
	cmp	r3, #4
	bhi	.L24
	.loc 1 150 15
	movs	r3, #0
	strh	r3, [r7, #22]	@ movhi
	.loc 1 150 9
	b	.L25
.L28:
	.loc 1 152 34
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 152 19
	and	r3, r3, #1
	strh	r3, [r7, #20]	@ movhi
	.loc 1 152 13
	b	.L26
.L27:
	.loc 1 153 28
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #20]
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 153 22
	ldrh	r3, [r7, #22]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 153 28
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 153 26
	strb	r2, [r3]
	.loc 1 154 28
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #20]
	adds	r3, r3, #1
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 154 22
	ldrh	r3, [r7, #22]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 154 28
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 154 26
	strb	r2, [r3]
	.loc 1 155 28
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #20]
	adds	r3, r3, #2
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 155 22
	ldrh	r3, [r7, #22]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 155 28
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 155 26
	strb	r2, [r3]
	.loc 1 156 28
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #20]
	adds	r3, r3, #3
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 156 22
	ldrh	r3, [r7, #22]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 156 28
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 156 26
	strb	r2, [r3]
	.loc 1 157 19
	ldrh	r3, [r7, #20]	@ movhi
	adds	r3, r3, #4
	strh	r3, [r7, #20]	@ movhi
	.loc 1 152 47 discriminator 3
	ldrh	r3, [r7, #20]
	adds	r3, r3, #1
	strh	r3, [r7, #20]	@ movhi
.L26:
	.loc 1 152 41 discriminator 1
	ldrh	r3, [r7, #20]
	cmp	r3, #7
	bls	.L27
	.loc 1 150 38 discriminator 2
	ldrh	r2, [r7, #22]	@ movhi
	ldrh	r3, [r7, #14]	@ movhi
	add	r3, r3, r2
	strh	r3, [r7, #22]	@ movhi
.L25:
	.loc 1 150 22 discriminator 1
	ldrh	r3, [r7, #22]
	ldr	r2, [r7]
	cmp	r2, r3
	bhi	.L28
	.loc 1 149 12
	b	.L18
.L24:
	.loc 1 161 26
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 161 12
	cmp	r3, #7
	bhi	.L29
	.loc 1 161 53 discriminator 1
	ldr	r3, .L61
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 161 30 discriminator 1
	cmp	r3, #4
	bhi	.L29
	.loc 1 164 30
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 164 15
	and	r3, r3, #1
	strh	r3, [r7, #22]	@ movhi
	.loc 1 164 9
	b	.L30
.L31:
	.loc 1 165 29
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 165 18
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 165 33
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 165 22
	strb	r2, [r3]
	.loc 1 166 14
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	strh	r3, [r7, #22]	@ movhi
	.loc 1 164 53 discriminator 3
	ldrh	r2, [r7, #22]	@ movhi
	ldrh	r3, [r7, #14]	@ movhi
	add	r3, r3, r2
	strh	r3, [r7, #22]	@ movhi
.L30:
	.loc 1 164 37 discriminator 1
	ldrh	r3, [r7, #22]
	ldr	r2, [r7]
	cmp	r2, r3
	bhi	.L31
	.loc 1 161 12
	b	.L18
.L29:
	.loc 1 169 26
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 169 12
	cmp	r3, #9
	bhi	.L32
	.loc 1 169 54 discriminator 1
	ldr	r3, .L61
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 169 31 discriminator 1
	cmp	r3, #4
	bhi	.L32
	.loc 1 172 30
	ldr	r3, .L61+4
	ldrh	r3, [r3]
	.loc 1 172 15
	and	r3, r3, #1
	strh	r3, [r7, #22]	@ movhi
	.loc 1 172 9
	b	.L33
.L62:
	.align	2
.L61:
	.word	mutation_degression
	.word	stage_cycles
.L34:
	.loc 1 173 29
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 173 18
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 173 33
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 173 22
	strb	r2, [r3]
	.loc 1 174 31
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 174 18
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 174 37
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 174 24
	strb	r2, [r3]
	.loc 1 175 15
	ldrh	r3, [r7, #22]	@ movhi
	adds	r3, r3, #2
	strh	r3, [r7, #22]	@ movhi
	.loc 1 172 77 discriminator 3
	ldrh	r2, [r7, #22]	@ movhi
	ldrh	r3, [r7, #14]	@ movhi
	add	r3, r3, r2
	strh	r3, [r7, #22]	@ movhi
.L33:
	.loc 1 172 37 discriminator 1
	ldrh	r2, [r7, #22]
	.loc 1 172 69 discriminator 1
	ldr	r3, .L63
	ldrh	r3, [r3]
	and	r3, r3, #1
	.loc 1 172 54 discriminator 1
	ldr	r1, [r7]
	subs	r3, r1, r3
	subs	r3, r3, #1
	.loc 1 172 37 discriminator 1
	cmp	r2, r3
	bcc	.L34
	.loc 1 169 12
	b	.L18
.L32:
	.loc 1 178 26
	ldr	r3, .L63
	ldrh	r3, [r3]
	.loc 1 178 12
	cmp	r3, #11
	bhi	.L35
	.loc 1 178 54 discriminator 1
	ldr	r3, .L63+4
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 178 31 discriminator 1
	cmp	r3, #4
	bhi	.L35
	.loc 1 181 30
	ldr	r3, .L63
	ldrh	r3, [r3]
	.loc 1 181 15
	and	r3, r3, #1
	strh	r3, [r7, #22]	@ movhi
	.loc 1 181 9
	b	.L36
.L37:
	.loc 1 182 29
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 182 18
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 182 33
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 182 22
	strb	r2, [r3]
	.loc 1 183 31
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 183 18
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 183 37
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 183 24
	strb	r2, [r3]
	.loc 1 184 33
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 184 18
	ldrh	r3, [r7, #22]
	adds	r3, r3, #2
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 184 37
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 184 26
	strb	r2, [r3]
	.loc 1 185 33
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 185 18
	ldrh	r3, [r7, #22]
	adds	r3, r3, #3
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 185 39
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 185 26
	strb	r2, [r3]
	.loc 1 186 15
	ldrh	r3, [r7, #22]	@ movhi
	adds	r3, r3, #4
	strh	r3, [r7, #22]	@ movhi
	.loc 1 181 61 discriminator 3
	ldrh	r2, [r7, #22]	@ movhi
	ldrh	r3, [r7, #14]	@ movhi
	add	r3, r3, r2
	uxth	r3, r3
	.loc 1 181 57 discriminator 3
	adds	r3, r3, #2
	strh	r3, [r7, #22]	@ movhi
.L36:
	.loc 1 181 37 discriminator 1
	ldrh	r2, [r7, #22]
	.loc 1 181 50 discriminator 1
	ldr	r3, [r7]
	subs	r3, r3, #4
	.loc 1 181 37 discriminator 1
	cmp	r2, r3
	bcc	.L37
	.loc 1 178 12
	b	.L18
.L35:
	.loc 1 190 26
	ldr	r3, .L63
	ldrh	r3, [r3]
	.loc 1 190 12
	cmp	r3, #47
	bhi	.L38
	.loc 1 190 54 discriminator 1
	ldr	r3, .L63+4
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 190 31 discriminator 1
	cmp	r3, #4
	bhi	.L38
	.loc 1 193 15
	ldrh	r3, [r7, #16]	@ movhi
	strh	r3, [r7, #22]	@ movhi
	.loc 1 193 9
	b	.L39
.L40:
	.loc 1 194 29
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r3, [r3]
	uxtb	r0, r3
	.loc 1 194 33
	ldr	r3, .L63
	ldrh	r1, [r3]
	ldr	r3, .L63+8
	umull	r2, r3, r3, r1
	subs	r2, r1, r3
	lsrs	r2, r2, #1
	add	r3, r3, r2
	lsrs	r2, r3, #5
	mov	r3, r2
	lsls	r3, r3, #2
	add	r3, r3, r2
	lsls	r2, r3, #3
	subs	r2, r2, r3
	subs	r3, r1, r2
	uxth	r3, r3
	uxtb	r3, r3
	add	r3, r3, r0
	uxtb	r1, r3
	.loc 1 194 18
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 194 33
	sxtb	r2, r1
	.loc 1 194 22
	strb	r2, [r3]
	.loc 1 195 14
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	strh	r3, [r7, #22]	@ movhi
	.loc 1 193 42 discriminator 3
	ldrh	r2, [r7, #22]	@ movhi
	ldrh	r3, [r7, #14]	@ movhi
	add	r3, r3, r2
	strh	r3, [r7, #22]	@ movhi
.L39:
	.loc 1 193 26 discriminator 1
	ldrh	r3, [r7, #22]
	ldr	r2, [r7]
	cmp	r2, r3
	bhi	.L40
	.loc 1 190 12
	b	.L18
.L38:
	.loc 1 198 26
	ldr	r3, .L63
	ldrh	r3, [r3]
	.loc 1 198 12
	cmp	r3, #81
	bhi	.L41
	.loc 1 198 54 discriminator 1
	ldr	r3, .L63+4
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 198 31 discriminator 1
	cmp	r3, #4
	bhi	.L41
	.loc 1 200 15
	ldrh	r3, [r7, #16]	@ movhi
	strh	r3, [r7, #22]	@ movhi
	.loc 1 200 9
	b	.L42
.L43:
	.loc 1 201 33
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r3, [r3]
	uxtb	r0, r3
	.loc 1 201 37
	ldr	r3, .L63
	ldrh	r1, [r3]
	ldr	r3, .L63+8
	umull	r2, r3, r3, r1
	subs	r2, r1, r3
	lsrs	r2, r2, #1
	add	r3, r3, r2
	lsrs	r2, r3, #5
	mov	r3, r2
	lsls	r3, r3, #2
	add	r3, r3, r2
	lsls	r2, r3, #3
	subs	r2, r2, r3
	subs	r3, r1, r2
	uxth	r3, r3
	uxtb	r3, r3
	subs	r3, r0, r3
	uxtb	r1, r3
	.loc 1 201 22
	ldrh	r3, [r7, #22]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 201 37
	sxtb	r2, r1
	.loc 1 201 26
	strb	r2, [r3]
	.loc 1 202 18
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	strh	r3, [r7, #22]	@ movhi
	.loc 1 200 42 discriminator 3
	ldrh	r2, [r7, #22]	@ movhi
	ldrh	r3, [r7, #14]	@ movhi
	add	r3, r3, r2
	strh	r3, [r7, #22]	@ movhi
.L42:
	.loc 1 200 26 discriminator 1
	ldrh	r3, [r7, #22]
	ldr	r2, [r7]
	cmp	r2, r3
	bhi	.L43
	.loc 1 198 12
	b	.L18
.L41:
	.loc 1 205 26
	ldr	r3, .L63
	ldrh	r3, [r3]
	.loc 1 205 12
	cmp	r3, #250
	bhi	.L18
	.loc 1 207 25
	bl	rand
	mov	r2, r0
	.loc 1 207 32 discriminator 1
	ldr	r3, .L63+12
	smull	r1, r3, r3, r2
	asrs	r1, r3, #1
	asrs	r3, r2, #31
	subs	r1, r1, r3
	mov	r3, r1
	lsls	r3, r3, #1
	add	r3, r3, r1
	lsls	r3, r3, #2
	subs	r1, r2, r3
	.loc 1 207 23 discriminator 1
	mov	r3, r1	@ movhi
	strh	r3, [r7, #18]	@ movhi
	.loc 1 209 22
	bl	rand
	mov	r2, r0
	.loc 1 209 35 discriminator 1
	ldr	r3, .L63+4
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	mov	r1, r3
	movs	r3, #100
	sdiv	r3, r3, r1
	.loc 1 209 29 discriminator 1
	sdiv	r1, r2, r3
	mul	r3, r1, r3
	subs	r3, r2, r3
	.loc 1 209 19 discriminator 1
	strh	r3, [r7, #12]	@ movhi
	.loc 1 211 15
	movs	r3, #0
	strh	r3, [r7, #22]	@ movhi
	.loc 1 211 9
	b	.L44
.L59:
	.loc 1 212 24
	bl	rand
	mov	r3, r0
	.loc 1 212 31 discriminator 1
	ldr	r2, [r7]
	udiv	r2, r3, r2
	ldr	r1, [r7]
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 212 22 discriminator 1
	strh	r3, [r7, #10]	@ movhi
	.loc 1 213 13
	ldrsh	r3, [r7, #18]
	cmp	r3, #11
	bhi	.L45
	adr	r2, .L47
	ldr	pc, [r2, r3, lsl #2]
	.p2align 2
.L47:
	.word	.L58+1
	.word	.L57+1
	.word	.L56+1
	.word	.L55+1
	.word	.L54+1
	.word	.L53+1
	.word	.L52+1
	.word	.L51+1
	.word	.L50+1
	.word	.L49+1
	.word	.L48+1
	.word	.L46+1
	.p2align 1
.L58:
	.loc 1 216 35
	bl	rand
	mov	r3, r0
	.loc 1 216 42 discriminator 1
	rsbs	r2, r3, #0
	uxtb	r3, r3
	uxtb	r2, r2
	it	pl
	rsbpl	r3, r2, #0
	.loc 1 216 22 discriminator 1
	ldrh	r2, [r7, #10]
	ldr	r1, [r7, #4]
	add	r2, r2, r1
	.loc 1 216 33 discriminator 1
	sxtb	r3, r3
	strb	r3, [r2]
	.loc 1 217 17
	b	.L45
.L57:
	.loc 1 219 40
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r3, [r3]
	uxtb	r0, r3
	.loc 1 219 51
	ldr	r3, .L63
	ldrh	r2, [r3]
	ldr	r3, .L63+16
	umull	r1, r3, r3, r2
	lsrs	r1, r3, #5
	mov	r3, r1
	lsls	r3, r3, #4
	add	r3, r3, r1
	lsls	r3, r3, #1
	subs	r3, r2, r3
	uxth	r3, r3
	uxtb	r3, r3
	add	r3, r3, r0
	uxtb	r1, r3
	.loc 1 219 22
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 219 51
	sxtb	r2, r1
	.loc 1 219 33
	strb	r2, [r3]
	.loc 1 220 17
	b	.L45
.L64:
	.align	2
.L63:
	.word	stage_cycles
	.word	mutation_degression
	.word	-736280107
	.word	715827883
	.word	-252645135
.L56:
	.loc 1 222 40
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r3, [r3]
	uxtb	r2, r3
	.loc 1 222 51
	ldr	r3, .L65
	ldrh	r3, [r3]
	uxtb	r3, r3
	add	r3, r3, r2
	uxtb	r1, r3
	.loc 1 222 22
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 222 51
	sxtb	r2, r1
	.loc 1 222 33
	strb	r2, [r3]
	.loc 1 223 17
	b	.L45
.L55:
	.loc 1 225 40
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r3, [r3]
	uxtb	r2, r3
	.loc 1 225 51
	ldr	r3, .L65
	ldrh	r3, [r3]
	uxtb	r3, r3
	subs	r3, r2, r3
	uxtb	r1, r3
	.loc 1 225 22
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 225 51
	sxtb	r2, r1
	.loc 1 225 33
	strb	r2, [r3]
	.loc 1 226 17
	b	.L45
.L54:
	.loc 1 228 40
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r3, [r3]
	uxtb	r0, r3
	.loc 1 228 51
	ldr	r3, .L65
	ldrh	r2, [r3]
	ldr	r3, .L65+4
	umull	r1, r3, r3, r2
	lsrs	r1, r3, #5
	mov	r3, r1
	lsls	r3, r3, #4
	add	r3, r3, r1
	lsls	r3, r3, #1
	subs	r3, r2, r3
	uxth	r3, r3
	uxtb	r3, r3
	subs	r3, r0, r3
	uxtb	r1, r3
	.loc 1 228 22
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 228 51
	sxtb	r2, r1
	.loc 1 228 33
	strb	r2, [r3]
	.loc 1 229 17
	b	.L45
.L53:
	.loc 1 231 28
	bl	rand
	mov	r3, r0
	.loc 1 231 35 discriminator 1
	ldr	r2, [r7]
	udiv	r2, r3, r2
	ldr	r1, [r7]
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 231 26 discriminator 1
	strh	r3, [r7, #10]	@ movhi
	.loc 1 232 27
	bl	rand
	mov	r3, r0
	.loc 1 232 34 discriminator 1
	rsbs	r2, r3, #0
	and	r3, r3, #7
	and	r2, r2, #7
	it	pl
	rsbpl	r3, r2, #0
	.loc 1 232 25 discriminator 1
	strh	r3, [r7, #8]	@ movhi
	.loc 1 233 35
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #8]
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 233 22
	ldrh	r3, [r7, #10]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 233 35
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 233 33
	strb	r2, [r3]
	.loc 1 234 17
	b	.L45
.L52:
	.loc 1 236 28
	bl	rand
	mov	r3, r0
	.loc 1 236 35 discriminator 1
	ldr	r2, [r7]
	udiv	r2, r3, r2
	ldr	r1, [r7]
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 236 26 discriminator 1
	strh	r3, [r7, #10]	@ movhi
	.loc 1 237 27
	bl	rand
	mov	r2, r0
	.loc 1 237 34 discriminator 1
	ldr	r3, .L65+8
	smull	r1, r3, r3, r2
	add	r3, r3, r2
	asrs	r1, r3, #2
	asrs	r3, r2, #31
	subs	r1, r1, r3
	mov	r3, r1
	lsls	r3, r3, #3
	subs	r3, r3, r1
	subs	r1, r2, r3
	.loc 1 237 25 discriminator 1
	mov	r3, r1	@ movhi
	strh	r3, [r7, #8]	@ movhi
	.loc 1 238 35
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #8]
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 238 22
	ldrh	r3, [r7, #10]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 238 35
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 238 33
	strb	r2, [r3]
	.loc 1 239 35
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #8]
	adds	r3, r3, #1
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 239 22
	ldrh	r3, [r7, #10]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 239 35
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 239 33
	strb	r2, [r3]
	.loc 1 240 17
	b	.L45
.L51:
	.loc 1 242 28
	bl	rand
	mov	r3, r0
	.loc 1 242 35 discriminator 1
	ldr	r2, [r7]
	udiv	r2, r3, r2
	ldr	r1, [r7]
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 242 26 discriminator 1
	strh	r3, [r7, #10]	@ movhi
	.loc 1 243 27
	bl	rand
	mov	r2, r0
	.loc 1 243 34 discriminator 1
	ldr	r3, .L65+12
	smull	r1, r3, r3, r2
	asrs	r1, r3, #1
	asrs	r3, r2, #31
	subs	r1, r1, r3
	mov	r3, r1
	lsls	r3, r3, #2
	add	r3, r3, r1
	subs	r1, r2, r3
	.loc 1 243 25 discriminator 1
	mov	r3, r1	@ movhi
	strh	r3, [r7, #8]	@ movhi
	.loc 1 244 35
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #8]
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 244 22
	ldrh	r3, [r7, #10]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 244 35
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 244 33
	strb	r2, [r3]
	.loc 1 245 35
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #8]
	adds	r3, r3, #1
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 245 22
	ldrh	r3, [r7, #10]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 245 35
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 245 33
	strb	r2, [r3]
	.loc 1 246 35
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #8]
	adds	r3, r3, #2
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 246 22
	ldrh	r3, [r7, #10]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 246 35
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 246 33
	strb	r2, [r3]
	.loc 1 247 35
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	ldrh	r3, [r7, #8]
	adds	r3, r3, #3
	movs	r2, #1
	lsl	r3, r2, r3
	sxtb	r2, r3
	.loc 1 247 22
	ldrh	r3, [r7, #10]
	ldr	r0, [r7, #4]
	add	r3, r3, r0
	.loc 1 247 35
	eors	r2, r2, r1
	sxtb	r2, r2
	.loc 1 247 33
	strb	r2, [r3]
	.loc 1 248 17
	b	.L45
.L50:
	.loc 1 250 28
	bl	rand
	mov	r3, r0
	.loc 1 250 35 discriminator 1
	ldr	r2, [r7]
	udiv	r2, r3, r2
	ldr	r1, [r7]
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 250 26 discriminator 1
	strh	r3, [r7, #10]	@ movhi
	.loc 1 251 40
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 251 22
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 251 51
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 251 33
	strb	r2, [r3]
	.loc 1 252 17
	b	.L45
.L49:
	.loc 1 254 28
	bl	rand
	mov	r3, r0
	.loc 1 254 35 discriminator 1
	ldr	r2, [r7]
	udiv	r2, r3, r2
	ldr	r1, [r7]
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 254 48 discriminator 1
	uxth	r3, r3
	.loc 1 254 26 discriminator 1
	subs	r3, r3, #1
	strh	r3, [r7, #10]	@ movhi
	.loc 1 255 40
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 255 22
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 255 51
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 255 33
	strb	r2, [r3]
	.loc 1 256 44
	ldrh	r3, [r7, #10]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 256 22
	ldrh	r3, [r7, #10]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 256 59
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 256 37
	strb	r2, [r3]
	.loc 1 257 17
	b	.L45
.L66:
	.align	2
.L65:
	.word	stage_cycles
	.word	-252645135
	.word	-1840700269
	.word	1717986919
.L48:
	.loc 1 259 28
	bl	rand
	mov	r3, r0
	.loc 1 259 35 discriminator 1
	ldr	r2, [r7]
	udiv	r2, r3, r2
	ldr	r1, [r7]
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 259 48 discriminator 1
	uxth	r3, r3
	.loc 1 259 26 discriminator 1
	subs	r3, r3, #4
	strh	r3, [r7, #10]	@ movhi
	.loc 1 260 40
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 260 22
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 260 51
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 260 33
	strb	r2, [r3]
	.loc 1 261 44
	ldrh	r3, [r7, #10]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 261 22
	ldrh	r3, [r7, #10]
	adds	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 261 59
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 261 37
	strb	r2, [r3]
	.loc 1 262 44
	ldrh	r3, [r7, #10]
	adds	r3, r3, #2
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 262 22
	ldrh	r3, [r7, #10]
	adds	r3, r3, #2
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 262 59
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 262 37
	strb	r2, [r3]
	.loc 1 263 44
	ldrh	r3, [r7, #10]
	adds	r3, r3, #3
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	ldrsb	r1, [r3]
	.loc 1 263 22
	ldrh	r3, [r7, #10]
	adds	r3, r3, #3
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 263 59
	mov	r2, r1
	mvns	r2, r2
	sxtb	r2, r2
	.loc 1 263 37
	strb	r2, [r3]
	.loc 1 265 17
	b	.L45
.L46:
	.loc 1 268 28
	bl	rand
	mov	r3, r0
	.loc 1 268 35 discriminator 1
	ldr	r2, [r7]
	udiv	r2, r3, r2
	ldr	r1, [r7]
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 268 26 discriminator 1
	strh	r3, [r7, #10]	@ movhi
	.loc 1 269 22
	ldrh	r3, [r7, #10]
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	.loc 1 269 33
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 270 17
	nop
.L45:
	.loc 1 273 29
	bl	rand
	mov	r2, r0
	.loc 1 273 36 discriminator 1
	ldr	r3, .L67
	smull	r1, r3, r3, r2
	asrs	r1, r3, #1
	asrs	r3, r2, #31
	subs	r1, r1, r3
	mov	r3, r1
	lsls	r3, r3, #1
	add	r3, r3, r1
	lsls	r3, r3, #2
	subs	r1, r2, r3
	.loc 1 273 27 discriminator 1
	mov	r3, r1	@ movhi
	strh	r3, [r7, #18]	@ movhi
	.loc 1 211 36 discriminator 2
	ldrh	r3, [r7, #22]
	adds	r3, r3, #1
	strh	r3, [r7, #22]	@ movhi
.L44:
	.loc 1 211 22 discriminator 1
	ldrh	r2, [r7, #22]
	ldrh	r3, [r7, #12]
	cmp	r2, r3
	bcc	.L59
.L18:
	.loc 1 277 17
	ldr	r3, .L67+4
	ldrh	r3, [r3]
	adds	r3, r3, #1
	uxth	r2, r3
	ldr	r3, .L67+4
	strh	r2, [r3]	@ movhi
	.loc 1 278 1
	nop
	adds	r7, r7, #24
	.cfi_def_cfa_offset 8
	mov	sp, r7
	.cfi_def_cfa_register 13
	@ sp needed
	pop	{r7, pc}
.L68:
	.align	2
.L67:
	.word	715827883
	.word	stage_cycles
	.cfi_endproc
.LFE5:
	.size	mutator, .-mutator
	.section	.rodata
	.align	2
.LC1:
	.ascii	"ERROR: Failed to copy seed to input buffer. \000"
	.text
	.align	1
	.global	dequeue_seed
	.syntax unified
	.thumb
	.thumb_func
	.type	dequeue_seed, %function
dequeue_seed:
.LFB6:
	.loc 1 281 38
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #16
	.cfi_def_cfa_offset 24
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	str	r0, [r7, #4]
	.loc 1 291 18
	ldr	r3, .L77
	ldrh	r2, [r3]
	ldr	r3, .L77+4
	ldrh	r3, [r3]
	.loc 1 291 7
	cmp	r2, r3
	bne	.L70
	.loc 1 292 19
	ldr	r3, .L77
	movs	r2, #0
	strh	r2, [r3]	@ movhi
.L70:
	.loc 1 295 20
	ldr	r3, .L77+8
	ldrh	r3, [r3]
	.loc 1 295 6
	cmp	r3, #250
	bne	.L71
	.loc 1 296 15
	ldr	r3, .L77+8
	movs	r2, #0
	strh	r2, [r3]	@ movhi
	.loc 1 297 11
	ldr	r3, .L77
	ldrh	r3, [r3]
	adds	r3, r3, #1
	uxth	r2, r3
	ldr	r3, .L77
	strh	r2, [r3]	@ movhi
.L71:
	.loc 1 299 17
	ldr	r3, .L77
	ldrh	r3, [r3]
	mov	r2, r3
	ldr	r3, .L77+12
	ldrb	r3, [r3]
	sxtb	r3, r3
	.loc 1 299 6
	cmp	r2, r3
	blt	.L72
	.loc 1 301 12
	ldr	r3, .L77
	movs	r2, #0
	strh	r2, [r3]	@ movhi
	.loc 1 302 15
	ldr	r3, .L77+16
	ldrh	r3, [r3]
	adds	r3, r3, #1
	uxth	r2, r3
	ldr	r3, .L77+16
	strh	r2, [r3]	@ movhi
.L72:
	.loc 1 309 18
	ldr	r3, .L77+16
	ldrh	r3, [r3]
	.loc 1 309 6
	cmp	r3, #5
	bls	.L73
	.loc 1 310 31
	ldr	r3, .L77+20
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 310 10
	cmp	r3, #9
	bhi	.L74
	.loc 1 311 31
	ldr	r3, .L77+20
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	adds	r3, r3, #1
	uxth	r2, r3
	ldr	r3, .L77+20
	strh	r2, [r3]	@ movhi
.L74:
	.loc 1 313 31
	ldr	r3, .L77+20
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	.loc 1 313 10
	cmp	r3, #10
	bne	.L73
	.loc 1 314 32
	ldr	r3, .L77+20
	movs	r2, #1
	strh	r2, [r3]	@ movhi
.L73:
	.loc 1 317 52
	ldr	r3, .L77
	ldrh	r3, [r3]
	.loc 1 317 42
	lsls	r3, r3, #7
	ldr	r2, .L77+24
	add	r3, r3, r2
	.loc 1 317 14
	movs	r2, #128
	mov	r1, r3
	ldr	r0, [r7, #4]
	bl	memcpy
	str	r0, [r7, #12]
	.loc 1 319 7
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bne	.L76
	.loc 1 320 9
	ldr	r0, .L77+28
	bl	puts
.L76:
	.loc 1 323 1
	nop
	adds	r7, r7, #16
	.cfi_def_cfa_offset 8
	mov	sp, r7
	.cfi_def_cfa_register 13
	@ sp needed
	pop	{r7, pc}
.L78:
	.align	2
.L77:
	.word	seed_head
	.word	seed_tail
	.word	stage_cycles
	.word	current_seed_num
	.word	pool_loops
	.word	mutation_degression
	.word	local_pool
	.word	.LC1
	.cfi_endproc
.LFE6:
	.size	dequeue_seed, .-dequeue_seed
	.align	1
	.global	bubble_coverage
	.syntax unified
	.thumb
	.thumb_func
	.type	bubble_coverage, %function
bubble_coverage:
.LFB7:
	.loc 1 326 23
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	.cfi_def_cfa_offset 4
	.cfi_offset 7, -4
	sub	sp, sp, #12
	.cfi_def_cfa_offset 16
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	.loc 1 327 13
	movs	r3, #0
	strh	r3, [r7, #6]	@ movhi
	.loc 1 328 2
	nop
	.loc 1 329 1
	adds	r7, r7, #12
	.cfi_def_cfa_offset 4
	mov	sp, r7
	.cfi_def_cfa_register 13
	@ sp needed
	pop	{r7}
	.cfi_restore 7
	.cfi_def_cfa_offset 0
	bx	lr
	.cfi_endproc
.LFE7:
	.size	bubble_coverage, .-bubble_coverage
	.section	.rodata
	.align	2
.LC2:
	.ascii	"ERROR: Coverage map is zeroed but flag is set. \000"
	.text
	.align	1
	.global	main_harness_loop
	.syntax unified
	.thumb
	.thumb_func
	.type	main_harness_loop, %function
main_harness_loop:
.LFB8:
	.loc 1 332 25
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #8
	.cfi_def_cfa_offset 16
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	.loc 1 340 20
	movs	r3, #35
	strh	r3, [r7, #4]	@ movhi
	.loc 1 342 5
	ldr	r0, .L87
	bl	setup
	.loc 1 345 5
	ldr	r0, .L87+4
	bl	setjmp
	.loc 1 350 12
	ldr	r3, .L87+8
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	adds	r3, r3, #1
	uxth	r2, r3
	ldr	r3, .L87+8
	strh	r2, [r3]	@ movhi
	.loc 1 351 2
	ldr	r3, .L87+12
	ldr	r3, [r3]
	mov	r0, r3
	bl	dequeue_seed
	.loc 1 353 5
	ldr	r3, .L87+12
	ldr	r3, [r3]
	ldrsh	r2, [r7, #4]
	mov	r1, r2
	mov	r0, r3
	bl	mutator
	.loc 1 357 5
	ldr	r3, .L87+12
	ldr	r3, [r3]
	ldrsh	r2, [r7, #4]
	mov	r1, r2
	mov	r0, r3
	bl	branch_test
	.loc 1 359 8
	ldr	r3, .L87+16
	ldrsb	r3, [r3]
	.loc 1 359 7
	cmp	r3, #0
	beq	.L83
	.loc 1 359 38 discriminator 1
	ldr	r3, .L87+20
	ldrh	r3, [r3]	@ movhi
	.loc 1 360 25
	ldr	r3, .L87+24
	ldrh	r3, [r3]	@ movhi
	uxth	r3, r3
	adds	r3, r3, #1
	uxth	r2, r3
	ldr	r3, .L87+24
	strh	r2, [r3]	@ movhi
	.loc 1 361 9
	bl	bubble_coverage
	.loc 1 364 15
	movs	r3, #0
	strh	r3, [r7, #6]	@ movhi
	.loc 1 364 9
	b	.L84
.L85:
	.loc 1 365 26
	ldrh	r3, [r7, #6]
	.loc 1 365 30
	ldr	r2, .L87+20
	movs	r1, #0
	strh	r1, [r2, r3, lsl #1]	@ movhi
	.loc 1 364 47 discriminator 3
	ldrh	r3, [r7, #6]
	adds	r3, r3, #1
	strh	r3, [r7, #6]	@ movhi
.L84:
	.loc 1 364 22 discriminator 1
	ldrh	r3, [r7, #6]
	cmp	r3, #199
	bls	.L85
	.loc 1 367 26
	ldr	r3, .L87+20
	movw	r2, #65535
	strh	r2, [r3]	@ movhi
	.loc 1 368 28
	ldr	r3, .L87+28
	ldr	r2, .L87+20
	str	r2, [r3]
	.loc 1 369 22
	ldr	r3, .L87+16
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 370 29
	ldr	r3, .L87+32
	movs	r2, #1
	strh	r2, [r3]	@ movhi
	b	.L86
.L83:
	.loc 1 372 13
	ldr	r3, .L87+16
	ldrsb	r3, [r3]
	.loc 1 372 12
	cmp	r3, #0
	beq	.L86
	.loc 1 373 9
	ldr	r0, .L87+36
	bl	puts
.L86:
	.loc 1 376 5
	movs	r1, #1
	ldr	r0, .L87+4
	bl	longjmp
.L88:
	.align	2
.L87:
	.word	branch_test
	.word	saved_context
	.word	iterations
	.word	current_testcase
	.word	isIncreasing
	.word	coverage_list
	.word	intersting_cases
	.word	coverage_list_head
	.word	mutation_degression
	.word	.LC2
	.cfi_endproc
.LFE8:
	.size	main_harness_loop, .-main_harness_loop
	.align	1
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
.LFB9:
	.loc 1 381 15
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	.loc 1 383 5
	bl	main_harness_loop
	movs	r3, #0
	.loc 1 384 1
	mov	r0, r3
	pop	{r7, pc}
	.cfi_endproc
.LFE9:
	.size	main, .-main
.Letext0:
	.file 2 "/home/santiago/Research/microfuzz/toolchains/arm/arm-none-eabi/include/machine/_default_types.h"
	.file 3 "/home/santiago/Research/microfuzz/toolchains/arm/lib/gcc/arm-none-eabi/13.3.1/include/stddef.h"
	.file 4 "/home/santiago/Research/microfuzz/toolchains/arm/arm-none-eabi/include/sys/_stdint.h"
	.file 5 "/home/santiago/Research/microfuzz/toolchains/arm/arm-none-eabi/include/machine/setjmp.h"
	.file 6 "/home/santiago/Research/microfuzz/toolchains/arm/arm-none-eabi/include/setjmp.h"
	.file 7 "/home/santiago/Research/microfuzz/toolchains/arm/arm-none-eabi/include/string.h"
	.file 8 "/home/santiago/Research/microfuzz/toolchains/arm/arm-none-eabi/include/stdlib.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x49a
	.2byte	0x5
	.byte	0x1
	.byte	0x4
	.4byte	.Ldebug_abbrev0
	.uleb128 0x10
	.4byte	.LASF59
	.byte	0x1d
	.4byte	.LASF60
	.4byte	.LASF61
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x4
	.4byte	.LASF2
	.byte	0x2
	.byte	0x29
	.byte	0x17
	.4byte	0x32
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x4
	.4byte	.LASF3
	.byte	0x2
	.byte	0x37
	.byte	0x18
	.4byte	0x4c
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.4byte	.LASF4
	.uleb128 0x4
	.4byte	.LASF5
	.byte	0x2
	.byte	0x39
	.byte	0x19
	.4byte	0x5f
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.4byte	.LASF6
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x4
	.4byte	.LASF8
	.byte	0x2
	.byte	0x4f
	.byte	0x19
	.4byte	0x79
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF9
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.4byte	.LASF10
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.4byte	.LASF11
	.uleb128 0x11
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF12
	.uleb128 0x4
	.4byte	.LASF13
	.byte	0x3
	.byte	0xd6
	.byte	0x17
	.4byte	0x95
	.uleb128 0x3
	.byte	0x8
	.byte	0x4
	.4byte	.LASF14
	.uleb128 0x12
	.byte	0x4
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF15
	.uleb128 0x5
	.4byte	0xbd
	.uleb128 0x13
	.uleb128 0x4
	.4byte	.LASF16
	.byte	0x4
	.byte	0x14
	.byte	0x12
	.4byte	0x26
	.uleb128 0x8
	.4byte	0xbe
	.uleb128 0x4
	.4byte	.LASF17
	.byte	0x4
	.byte	0x20
	.byte	0x13
	.4byte	0x40
	.uleb128 0x4
	.4byte	.LASF18
	.byte	0x4
	.byte	0x24
	.byte	0x14
	.4byte	0x53
	.uleb128 0x8
	.4byte	0xdb
	.uleb128 0x4
	.4byte	.LASF19
	.byte	0x4
	.byte	0x30
	.byte	0x14
	.4byte	0x6d
	.uleb128 0x14
	.4byte	.LASF20
	.byte	0x5
	.2byte	0x1b2
	.byte	0x11
	.4byte	0x105
	.uleb128 0xa
	.4byte	0x80
	.4byte	0x115
	.uleb128 0x9
	.4byte	0x95
	.byte	0x13
	.byte	0
	.uleb128 0xa
	.4byte	0xca
	.4byte	0x12b
	.uleb128 0x9
	.4byte	0x95
	.byte	0xe
	.uleb128 0x9
	.4byte	0x95
	.byte	0x7f
	.byte	0
	.uleb128 0x8
	.4byte	0x115
	.uleb128 0x1
	.4byte	.LASF21
	.byte	0x1a
	.byte	0x11
	.4byte	0x12b
	.uleb128 0x5
	.byte	0x3
	.4byte	local_pool
	.uleb128 0x1
	.4byte	.LASF22
	.byte	0x1b
	.byte	0x11
	.4byte	0xca
	.uleb128 0x5
	.byte	0x3
	.4byte	current_seed_num
	.uleb128 0x1
	.4byte	.LASF23
	.byte	0x1d
	.byte	0x13
	.4byte	0xe7
	.uleb128 0x5
	.byte	0x3
	.4byte	intersting_cases
	.uleb128 0x1
	.4byte	.LASF24
	.byte	0x1f
	.byte	0xa
	.4byte	0x174
	.uleb128 0x5
	.byte	0x3
	.4byte	current_testcase
	.uleb128 0x5
	.4byte	0xbe
	.uleb128 0x1
	.4byte	.LASF25
	.byte	0x21
	.byte	0xa
	.4byte	0xdb
	.uleb128 0x5
	.byte	0x3
	.4byte	seed_head
	.uleb128 0x1
	.4byte	.LASF26
	.byte	0x22
	.byte	0xa
	.4byte	0xdb
	.uleb128 0x5
	.byte	0x3
	.4byte	seed_tail
	.uleb128 0x1
	.4byte	.LASF27
	.byte	0x23
	.byte	0xa
	.4byte	0xdb
	.uleb128 0x5
	.byte	0x3
	.4byte	stage_cycles
	.uleb128 0x1
	.4byte	.LASF28
	.byte	0x24
	.byte	0xa
	.4byte	0xdb
	.uleb128 0x5
	.byte	0x3
	.4byte	pool_loops
	.uleb128 0x1
	.4byte	.LASF29
	.byte	0x26
	.byte	0x13
	.4byte	0xe7
	.uleb128 0x5
	.byte	0x3
	.4byte	iterations
	.uleb128 0x1
	.4byte	.LASF30
	.byte	0x27
	.byte	0x13
	.4byte	0xe7
	.uleb128 0x5
	.byte	0x3
	.4byte	mutation_degression
	.uleb128 0xa
	.4byte	0xe7
	.4byte	0x1ef
	.uleb128 0x9
	.4byte	0x95
	.byte	0xc7
	.byte	0
	.uleb128 0x8
	.4byte	0x1df
	.uleb128 0x1
	.4byte	.LASF31
	.byte	0x29
	.byte	0x13
	.4byte	0x1ef
	.uleb128 0x5
	.byte	0x3
	.4byte	coverage_list
	.uleb128 0x1
	.4byte	.LASF32
	.byte	0x2a
	.byte	0x15
	.4byte	0x216
	.uleb128 0x5
	.byte	0x3
	.4byte	coverage_list_head
	.uleb128 0x5
	.4byte	0xe7
	.uleb128 0x1
	.4byte	.LASF33
	.byte	0x2b
	.byte	0xc
	.4byte	0x22c
	.uleb128 0x5
	.byte	0x3
	.4byte	sut_start_address
	.uleb128 0x5
	.4byte	0xec
	.uleb128 0x1
	.4byte	.LASF34
	.byte	0x2c
	.byte	0x8
	.4byte	0xbe
	.uleb128 0x5
	.byte	0x3
	.4byte	isIncreasing
	.uleb128 0x1
	.4byte	.LASF35
	.byte	0x2f
	.byte	0x9
	.4byte	0xf8
	.uleb128 0x5
	.byte	0x3
	.4byte	saved_context
	.uleb128 0xc
	.4byte	.LASF36
	.byte	0x6
	.byte	0xf
	.4byte	0x269
	.uleb128 0x2
	.4byte	0x269
	.uleb128 0x2
	.4byte	0x8e
	.byte	0
	.uleb128 0x5
	.4byte	0x80
	.uleb128 0x7
	.4byte	.LASF37
	.byte	0x6
	.byte	0x14
	.byte	0x5
	.4byte	0x8e
	.4byte	0x284
	.uleb128 0x2
	.4byte	0x269
	.byte	0
	.uleb128 0x7
	.4byte	.LASF38
	.byte	0x1
	.byte	0x31
	.byte	0x10
	.4byte	0xcf
	.4byte	0x29f
	.uleb128 0x2
	.4byte	0x174
	.uleb128 0x2
	.4byte	0xcf
	.byte	0
	.uleb128 0x7
	.4byte	.LASF39
	.byte	0x7
	.byte	0x1f
	.byte	0x9
	.4byte	0xaf
	.4byte	0x2bf
	.uleb128 0x2
	.4byte	0xaf
	.uleb128 0x2
	.4byte	0xb8
	.uleb128 0x2
	.4byte	0x9c
	.byte	0
	.uleb128 0x15
	.4byte	.LASF62
	.byte	0x8
	.byte	0x90
	.byte	0x5
	.4byte	0x8e
	.uleb128 0x7
	.4byte	.LASF40
	.byte	0x7
	.byte	0x21
	.byte	0x9
	.4byte	0xaf
	.4byte	0x2eb
	.uleb128 0x2
	.4byte	0xaf
	.uleb128 0x2
	.4byte	0x8e
	.uleb128 0x2
	.4byte	0x9c
	.byte	0
	.uleb128 0xc
	.4byte	.LASF41
	.byte	0x8
	.byte	0x5d
	.4byte	0x2fc
	.uleb128 0x2
	.4byte	0x8e
	.byte	0
	.uleb128 0x7
	.4byte	.LASF42
	.byte	0x8
	.byte	0x5a
	.byte	0x7
	.4byte	0xaf
	.4byte	0x317
	.uleb128 0x2
	.4byte	0x9c
	.uleb128 0x2
	.4byte	0x9c
	.byte	0
	.uleb128 0x16
	.4byte	.LASF63
	.byte	0x1
	.2byte	0x17d
	.byte	0x5
	.4byte	0x8e
	.4byte	.LFB9
	.4byte	.LFE9-.LFB9
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x17
	.4byte	.LASF44
	.byte	0x1
	.2byte	0x14c
	.byte	0x6
	.4byte	.LFB8
	.4byte	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x362
	.uleb128 0xd
	.4byte	.LASF43
	.2byte	0x152
	.byte	0xd
	.4byte	0xcf
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0xe
	.ascii	"i\000"
	.2byte	0x153
	.byte	0xb
	.4byte	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.byte	0
	.uleb128 0x18
	.4byte	.LASF45
	.byte	0x1
	.2byte	0x146
	.byte	0x6
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x387
	.uleb128 0xe
	.ascii	"i\000"
	.2byte	0x147
	.byte	0xd
	.4byte	0xcf
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.byte	0
	.uleb128 0x19
	.4byte	.LASF47
	.byte	0x1
	.2byte	0x119
	.byte	0x6
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x3be
	.uleb128 0x1a
	.4byte	.LASF49
	.byte	0x1
	.2byte	0x119
	.byte	0x1c
	.4byte	0x174
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0xd
	.4byte	.LASF46
	.2byte	0x120
	.byte	0xf
	.4byte	0x3be
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x5
	.4byte	0xcf
	.uleb128 0x1b
	.4byte	.LASF48
	.byte	0x1
	.byte	0x5d
	.byte	0x6
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x462
	.uleb128 0xb
	.4byte	.LASF50
	.byte	0x5d
	.byte	0x17
	.4byte	0x174
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0xb
	.4byte	.LASF51
	.byte	0x5d
	.byte	0x25
	.4byte	0x9c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xf
	.ascii	"i\000"
	.byte	0x64
	.byte	0xe
	.4byte	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.uleb128 0xf
	.ascii	"j\000"
	.byte	0x64
	.byte	0x11
	.4byte	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x6
	.4byte	.LASF52
	.byte	0x65
	.byte	0xe
	.4byte	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -22
	.uleb128 0x6
	.4byte	.LASF53
	.byte	0x66
	.byte	0xe
	.4byte	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x6
	.4byte	.LASF54
	.byte	0x67
	.byte	0xe
	.4byte	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x6
	.4byte	.LASF55
	.byte	0x68
	.byte	0xd
	.4byte	0xcf
	.uleb128 0x2
	.byte	0x91
	.sleb128 -14
	.uleb128 0x6
	.4byte	.LASF56
	.byte	0x6a
	.byte	0xe
	.4byte	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x6
	.4byte	.LASF57
	.byte	0x6b
	.byte	0xe
	.4byte	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -18
	.byte	0
	.uleb128 0x1c
	.4byte	.LASF64
	.byte	0x1
	.byte	0x41
	.byte	0x9
	.4byte	0xcf
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x48b
	.uleb128 0xb
	.4byte	.LASF58
	.byte	0x41
	.byte	0x16
	.4byte	0xaf
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x1d
	.4byte	.LASF65
	.byte	0x1
	.byte	0x3d
	.byte	0x6
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 6
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x87
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF13:
	.ascii	"size_t\000"
.LASF65:
	.ascii	"crash_void\000"
.LASF11:
	.ascii	"long long unsigned int\000"
.LASF37:
	.ascii	"setjmp\000"
.LASF2:
	.ascii	"__int8_t\000"
.LASF17:
	.ascii	"int16_t\000"
.LASF10:
	.ascii	"long long int\000"
.LASF0:
	.ascii	"signed char\000"
.LASF45:
	.ascii	"bubble_coverage\000"
.LASF60:
	.ascii	"./fuzz.c\000"
.LASF59:
	.ascii	"GNU C17 13.3.1 20240614 -mcpu=cortex-m4 -mfloat-abi"
	.ascii	"=soft -mthumb -march=armv7e-m -g\000"
.LASF44:
	.ascii	"main_harness_loop\000"
.LASF7:
	.ascii	"long int\000"
.LASF39:
	.ascii	"memcpy\000"
.LASF18:
	.ascii	"uint16_t\000"
.LASF26:
	.ascii	"seed_tail\000"
.LASF43:
	.ascii	"test_case_size\000"
.LASF27:
	.ascii	"stage_cycles\000"
.LASF8:
	.ascii	"__uint32_t\000"
.LASF22:
	.ascii	"current_seed_num\000"
.LASF3:
	.ascii	"__int16_t\000"
.LASF52:
	.ascii	"rnd_byte\000"
.LASF53:
	.ascii	"rnd_bit\000"
.LASF12:
	.ascii	"unsigned int\000"
.LASF9:
	.ascii	"long unsigned int\000"
.LASF62:
	.ascii	"rand\000"
.LASF54:
	.ascii	"rnd_value\000"
.LASF35:
	.ascii	"saved_context\000"
.LASF57:
	.ascii	"mutation_amount\000"
.LASF6:
	.ascii	"short unsigned int\000"
.LASF34:
	.ascii	"isIncreasing\000"
.LASF58:
	.ascii	"function_pointer\000"
.LASF14:
	.ascii	"long double\000"
.LASF61:
	.ascii	"/home/santiago/Research/microfuzz/on_board/F44RE/sr"
	.ascii	"c\000"
.LASF49:
	.ascii	"test_case\000"
.LASF29:
	.ascii	"iterations\000"
.LASF64:
	.ascii	"setup\000"
.LASF38:
	.ascii	"branch_test\000"
.LASF48:
	.ascii	"mutator\000"
.LASF30:
	.ascii	"mutation_degression\000"
.LASF24:
	.ascii	"current_testcase\000"
.LASF31:
	.ascii	"coverage_list\000"
.LASF47:
	.ascii	"dequeue_seed\000"
.LASF33:
	.ascii	"sut_start_address\000"
.LASF51:
	.ascii	"input_size\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF4:
	.ascii	"short int\000"
.LASF55:
	.ascii	"mutation_pick\000"
.LASF41:
	.ascii	"exit\000"
.LASF50:
	.ascii	"input\000"
.LASF28:
	.ascii	"pool_loops\000"
.LASF19:
	.ascii	"uint32_t\000"
.LASF21:
	.ascii	"local_pool\000"
.LASF15:
	.ascii	"char\000"
.LASF25:
	.ascii	"seed_head\000"
.LASF5:
	.ascii	"__uint16_t\000"
.LASF23:
	.ascii	"intersting_cases\000"
.LASF16:
	.ascii	"int8_t\000"
.LASF40:
	.ascii	"memset\000"
.LASF20:
	.ascii	"jmp_buf\000"
.LASF42:
	.ascii	"calloc\000"
.LASF32:
	.ascii	"coverage_list_head\000"
.LASF56:
	.ascii	"start\000"
.LASF63:
	.ascii	"main\000"
.LASF46:
	.ascii	"result\000"
.LASF36:
	.ascii	"longjmp\000"
	.ident	"GCC: (Arm GNU Toolchain 13.3.Rel1 (Build arm-13.24)) 13.3.1 20240614"
