	.file	"main.c"
	.intel_syntax noprefix
	.text
	.globl	makeArrayB
	.type	makeArrayB, @function
makeArrayB:
.LFB6:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	DWORD PTR -36[rbp], edx
	mov	DWORD PTR -12[rbp], 0
	mov	DWORD PTR -8[rbp], 0
	mov	DWORD PTR -4[rbp], -1
	jmp	.L2
.L5:
	mov	eax, DWORD PTR -12[rbp]
	cdqe
	lea	rdx, 0[0+rax*4]
	mov	rax, QWORD PTR -24[rbp]
	add	rax, rdx
	mov	eax, DWORD PTR [rax]
	test	eax, eax
	jle	.L3
	mov	eax, DWORD PTR -12[rbp]
	mov	DWORD PTR -4[rbp], eax
	jmp	.L4
.L3:
	add	DWORD PTR -12[rbp], 1
.L2:
	mov	eax, DWORD PTR -12[rbp]
	cmp	eax, DWORD PTR -36[rbp]
	jl	.L5
.L4:
	mov	eax, DWORD PTR -36[rbp]
	sub	eax, 1
	mov	DWORD PTR -12[rbp], eax
	jmp	.L6
.L8:
	mov	eax, DWORD PTR -12[rbp]
	cmp	eax, DWORD PTR -4[rbp]
	je	.L7
	mov	eax, DWORD PTR -12[rbp]
	cdqe
	lea	rdx, 0[0+rax*4]
	mov	rax, QWORD PTR -24[rbp]
	add	rax, rdx
	mov	edx, DWORD PTR -8[rbp]
	movsx	rdx, edx
	lea	rcx, 0[0+rdx*4]
	mov	rdx, QWORD PTR -32[rbp]
	add	rdx, rcx
	mov	eax, DWORD PTR [rax]
	mov	DWORD PTR [rdx], eax
	add	DWORD PTR -8[rbp], 1
.L7:
	sub	DWORD PTR -12[rbp], 1
.L6:
	cmp	DWORD PTR -12[rbp], 0
	jns	.L8
	cmp	DWORD PTR -4[rbp], -1
	jne	.L9
	mov	eax, 0
	jmp	.L10
.L9:
	mov	eax, 1
.L10:
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	makeArrayB, .-makeArrayB
	.section	.rodata
.LC0:
	.string	"%d "
	.text
	.globl	outputArray
	.type	outputArray, @function
outputArray:
.LFB7:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	DWORD PTR -28[rbp], esi
	mov	DWORD PTR -4[rbp], 0
	jmp	.L12
.L13:
	mov	eax, DWORD PTR -4[rbp]
	cdqe
	lea	rdx, 0[0+rax*4]
	mov	rax, QWORD PTR -24[rbp]
	add	rax, rdx
	mov	eax, DWORD PTR [rax]
	mov	esi, eax
	lea	rax, .LC0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	add	DWORD PTR -4[rbp], 1
.L12:
	mov	eax, DWORD PTR -4[rbp]
	cmp	eax, DWORD PTR -28[rbp]
	jl	.L13
	mov edi, 10
    call	putchar@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	outputArray, .-outputArray
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 56
	.cfi_offset 3, -24
	mov	DWORD PTR -52[rbp], edi
	mov	QWORD PTR -64[rbp], rsi
	mov	eax, DWORD PTR -52[rbp]
	sub	eax, 1
	cdqe
	sal	rax, 2
	mov	rdi, rax
	call	malloc@PLT
	mov	QWORD PTR -32[rbp], rax
	mov	eax, DWORD PTR -52[rbp]
	sub	eax, 1
	cdqe
	sal	rax, 2
	mov	rdi, rax
	call	malloc@PLT
	mov	QWORD PTR -24[rbp], rax
	mov	DWORD PTR -40[rbp], 1
	jmp	.L15
.L16:
	mov	eax, DWORD PTR -40[rbp]
	cdqe
	lea	rdx, 0[0+rax*8]
	mov	rax, QWORD PTR -64[rbp]
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR -40[rbp]
	movsx	rdx, edx
	sal	rdx, 2
	lea	rcx, -4[rdx]
	mov	rdx, QWORD PTR -32[rbp]
	lea	rbx, [rcx+rdx]
	mov	rdi, rax
	call	atoi@PLT
	mov	DWORD PTR [rbx], eax
	add	DWORD PTR -40[rbp], 1
.L15:
	mov	eax, DWORD PTR -40[rbp]
	cmp	eax, DWORD PTR -52[rbp]
	jl	.L16
	mov	eax, DWORD PTR -52[rbp]
	lea	edx, -1[rax]
	mov	rcx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -32[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	makeArrayB
	mov	DWORD PTR -36[rbp], eax
	mov	eax, DWORD PTR -52[rbp]
	sub	eax, 1
	sub	eax, DWORD PTR -36[rbp]
	mov	edx, eax
	mov	rax, QWORD PTR -24[rbp]
	mov	esi, edx
	mov	rdi, rax
	call	outputArray
	mov	eax, 0
	mov	rbx, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
