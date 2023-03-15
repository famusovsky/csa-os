	.file	"main.c"
	.intel_syntax noprefix
	.text
	.globl	ex
	.type	ex, @function
ex:
.LFB6:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	movsd	QWORD PTR -72[rbp], xmm0
	fldz
	fstp	TBYTE PTR -64[rbp]
	fld1
	fchs
	fstp	TBYTE PTR -48[rbp]
	fld1
	fstp	TBYTE PTR -32[rbp]
	mov	DWORD PTR -52[rbp], 0
	jmp	.L2
.L3:
	fld	TBYTE PTR -64[rbp]
	fstp	TBYTE PTR -48[rbp]
	fld	TBYTE PTR -64[rbp]
	fld	TBYTE PTR -32[rbp]
	faddp	st(1), st
	fstp	TBYTE PTR -64[rbp]
	fld	QWORD PTR -72[rbp]
	fld	TBYTE PTR -32[rbp]
	fmulp	st(1), st
	add	DWORD PTR -52[rbp], 1
	fild	DWORD PTR -52[rbp]
	fdivp	st(1), st
	fstp	TBYTE PTR -32[rbp]
.L2:
	fld	TBYTE PTR -64[rbp]
	fld	TBYTE PTR -48[rbp]
	fucomip	st, st(1)
	fstp	st(0)
	jp	.L3
	fld	TBYTE PTR -64[rbp]
	fld	TBYTE PTR -48[rbp]
	fucomip	st, st(1)
	fstp	st(0)
	jne	.L3
	fld	TBYTE PTR -48[rbp]
	fstp	QWORD PTR -80[rbp]
	movsd	xmm0, QWORD PTR -80[rbp]
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	ex, .-ex
	.section	.rodata
.LC4:
	.string	"%lf \n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	mov	QWORD PTR -32[rbp], rsi
	mov	rax, QWORD PTR -32[rbp]
	add	rax, 8
	mov	rax, QWORD PTR [rax]
	mov	rdi, rax
	call	atoi@PLT
	mov	DWORD PTR -4[rbp], eax
	cmp	DWORD PTR -4[rbp], 0
	js	.L6
	cvtsi2sd	xmm0, DWORD PTR -4[rbp]
	call	ex
	lea	rdi, .LC4[rip]
	mov	eax, 1
	call	printf@PLT
	jmp	.L7
.L6:
	mov	eax, DWORD PTR -4[rbp]
	neg	eax
	cvtsi2sd	xmm0, eax
	call	ex
	movsd	xmm1, QWORD PTR .LC5[rip]
	divsd	xmm1, xmm0
	movapd	xmm0, xmm1
	lea	rdi, .LC4[rip]
	mov	eax, 1
	call	printf@PLT
.L7:
	mov	eax, 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC5:
	.long	0
	.long	1072693248
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
