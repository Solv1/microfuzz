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
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "branch.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hit the D case\000"
	.align	2
.LC1:
	.ascii	"Hit the E case\000"
	.align	2
.LC2:
	.ascii	"Hit the A case \000"
	.align	2
.LC3:
	.ascii	"Hit the D case \000"
	.align	2
.LC4:
	.ascii	"Hit the E case \000"
	.align	2
.LC5:
	.ascii	"Hit the F case \000"
	.align	2
.LC6:
	.ascii	"Congrats you got to the end :)\000"
	.text
	.align	1
	.global	branch_test
	.syntax unified
	.thumb
	.thumb_func
	.type	branch_test, %function
branch_test:
.LFB3:
	.loc 1 5 49
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
	mov	r3, r1
	strh	r3, [r7, #2]	@ movhi
	.loc 1 7 9
	ldr	r3, [r7, #4]
	ldrsb	r3, [r3]
	.loc 1 7 4
	cmp	r3, #68
	bne	.L2
	.loc 1 8 3
	ldr	r0, .L4
	bl	puts
	.loc 1 9 10
	ldr	r3, [r7, #4]
	adds	r3, r3, #1
	ldrsb	r3, [r3]
	.loc 1 9 5
	cmp	r3, #69
	bne	.L2
	.loc 1 10 4
	ldr	r0, .L4+4
	bl	puts
	.loc 1 11 11
	ldr	r3, [r7, #4]
	adds	r3, r3, #2
	ldrsb	r3, [r3]
	.loc 1 11 6
	cmp	r3, #65
	bne	.L2
	.loc 1 12 5
	ldr	r0, .L4+8
	bl	puts
	.loc 1 13 12
	ldr	r3, [r7, #4]
	adds	r3, r3, #3
	ldrsb	r3, [r3]
	.loc 1 13 7
	cmp	r3, #68
	bne	.L2
	.loc 1 14 6
	ldr	r0, .L4+12
	bl	puts
	.loc 1 15 14
	ldr	r3, [r7, #4]
	adds	r3, r3, #4
	ldrsb	r3, [r3]
	.loc 1 15 9
	cmp	r3, #66
	bne	.L2
	.loc 1 16 7
	ldr	r0, .L4
	bl	puts
	.loc 1 17 14
	ldr	r3, [r7, #4]
	adds	r3, r3, #5
	ldrsb	r3, [r3]
	.loc 1 17 9
	cmp	r3, #69
	bne	.L2
	.loc 1 18 8
	ldr	r0, .L4+4
	bl	puts
	.loc 1 19 15
	ldr	r3, [r7, #4]
	adds	r3, r3, #6
	ldrsb	r3, [r3]
	.loc 1 19 10
	cmp	r3, #69
	bne	.L2
	.loc 1 20 9
	ldr	r0, .L4+16
	bl	puts
	.loc 1 21 16
	ldr	r3, [r7, #4]
	adds	r3, r3, #7
	ldrsb	r3, [r3]
	.loc 1 21 11
	cmp	r3, #70
	bne	.L2
	.loc 1 22 10
	ldr	r0, .L4+20
	bl	puts
	.loc 1 23 10
	ldr	r0, .L4+24
	bl	puts
.L2:
	.loc 1 32 9
	movs	r3, #0
	.loc 1 33 1
	mov	r0, r3
	adds	r7, r7, #8
	.cfi_def_cfa_offset 8
	mov	sp, r7
	.cfi_def_cfa_register 13
	@ sp needed
	pop	{r7, pc}
.L5:
	.align	2
.L4:
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.cfi_endproc
.LFE3:
	.size	branch_test, .-branch_test
.Letext0:
	.file 2 "/home/santiago/Research/microfuzz/toolchains/arm/arm-none-eabi/include/machine/_default_types.h"
	.file 3 "/home/santiago/Research/microfuzz/toolchains/arm/arm-none-eabi/include/sys/_stdint.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0xe2
	.2byte	0x5
	.byte	0x1
	.byte	0x4
	.4byte	.Ldebug_abbrev0
	.uleb128 0x4
	.4byte	.LASF17
	.byte	0x1d
	.4byte	.LASF18
	.4byte	.LASF19
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.4byte	.LASF2
	.byte	0x2
	.byte	0x29
	.byte	0x17
	.4byte	0x32
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x2
	.4byte	.LASF3
	.byte	0x2
	.byte	0x37
	.byte	0x18
	.4byte	0x4c
	.uleb128 0x1
	.byte	0x2
	.byte	0x5
	.4byte	.LASF4
	.uleb128 0x1
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x1
	.byte	0x4
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF7
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF8
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF9
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF10
	.uleb128 0x1
	.byte	0x8
	.byte	0x4
	.4byte	.LASF11
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF12
	.uleb128 0x2
	.4byte	.LASF13
	.byte	0x3
	.byte	0x14
	.byte	0x12
	.4byte	0x26
	.uleb128 0x2
	.4byte	.LASF14
	.byte	0x3
	.byte	0x20
	.byte	0x13
	.4byte	0x40
	.uleb128 0x6
	.4byte	.LASF20
	.byte	0x1
	.byte	0x5
	.byte	0x9
	.4byte	0x9e
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xdf
	.uleb128 0x3
	.4byte	.LASF15
	.byte	0x1e
	.4byte	0xdf
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x3
	.4byte	.LASF16
	.byte	0x2c
	.4byte	0x9e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -14
	.byte	0
	.uleb128 0x7
	.byte	0x4
	.4byte	0x92
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
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
	.uleb128 0x2
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
	.uleb128 0x3
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0x21
	.sleb128 5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x4
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
	.uleb128 0x5
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
	.uleb128 0x6
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
	.uleb128 0x7
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
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
.LASF8:
	.ascii	"long long int\000"
.LASF3:
	.ascii	"__int16_t\000"
.LASF19:
	.ascii	"/home/santiago/Research/microfuzz/on_board/F44RE/sr"
	.ascii	"c\000"
.LASF18:
	.ascii	"branch.c\000"
.LASF14:
	.ascii	"int16_t\000"
.LASF7:
	.ascii	"long unsigned int\000"
.LASF9:
	.ascii	"long long unsigned int\000"
.LASF17:
	.ascii	"GNU C17 13.3.1 20240614 -mcpu=cortex-m4 -mfloat-abi"
	.ascii	"=soft -mthumb -march=armv7e-m -g\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF12:
	.ascii	"char\000"
.LASF13:
	.ascii	"int8_t\000"
.LASF6:
	.ascii	"long int\000"
.LASF20:
	.ascii	"branch_test\000"
.LASF2:
	.ascii	"__int8_t\000"
.LASF5:
	.ascii	"short unsigned int\000"
.LASF0:
	.ascii	"signed char\000"
.LASF16:
	.ascii	"size\000"
.LASF11:
	.ascii	"long double\000"
.LASF4:
	.ascii	"short int\000"
.LASF10:
	.ascii	"unsigned int\000"
.LASF15:
	.ascii	"args\000"
	.ident	"GCC: (Arm GNU Toolchain 13.3.Rel1 (Build arm-13.24)) 13.3.1 20240614"
