	.file	"main.c"
	.intel_syntax noprefix
	.text
	.globl	findLastSeq
	.type	findLastSeq, @function
findLastSeq:
.LFB6:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR -40[rbp], rdi
	mov	DWORD PTR -44[rbp], esi
	mov	eax, DWORD PTR -44[rbp]
	cdqe
	mov	rdi, rax
	call	malloc@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	DWORD PTR -24[rbp], 0
	mov	DWORD PTR -20[rbp], 0
	jmp	.L2
.L11:
	mov	DWORD PTR -16[rbp], 1
	jmp	.L3
.L7:
	mov	edx, DWORD PTR -24[rbp]
	mov	eax, DWORD PTR -16[rbp]
	add	eax, edx
	movsx	rdx, eax
	mov	rax, QWORD PTR -40[rbp]
	add	rax, rdx
	movzx	edx, BYTE PTR [rax]
	mov	ecx, DWORD PTR -24[rbp]
	mov	eax, DWORD PTR -16[rbp]
	add	eax, ecx
	cdqe
	lea	rcx, -1[rax]
	mov	rax, QWORD PTR -40[rbp]
	add	rax, rcx
	movzx	eax, BYTE PTR [rax]
	cmp	dl, al
	jle	.L4
	mov	DWORD PTR -20[rbp], 1
	jmp	.L13
.L4:
	mov	DWORD PTR -20[rbp], 0
	jmp	.L6
.L13:
	add	DWORD PTR -16[rbp], 1
.L3:
	mov	eax, DWORD PTR -16[rbp]
	cmp	eax, DWORD PTR -44[rbp]
	jl	.L7
.L6:
	cmp	DWORD PTR -20[rbp], 0
	je	.L8
	mov	DWORD PTR -12[rbp], 0
	jmp	.L9
.L10:
	mov	edx, DWORD PTR -24[rbp]
	mov	eax, DWORD PTR -12[rbp]
	add	eax, edx
	movsx	rdx, eax
	mov	rax, QWORD PTR -40[rbp]
	add	rax, rdx
	mov	edx, DWORD PTR -12[rbp]
	movsx	rcx, edx
	mov	rdx, QWORD PTR -8[rbp]
	add	rdx, rcx
	movzx	eax, BYTE PTR [rax]
	mov	BYTE PTR [rdx], al
	add	DWORD PTR -12[rbp], 1
.L9:
	mov	eax, DWORD PTR -12[rbp]
	cmp	eax, DWORD PTR -44[rbp]
	jl	.L10
.L8:
	add	DWORD PTR -24[rbp], 1
.L2:
	mov	edx, DWORD PTR -24[rbp]
	mov	eax, DWORD PTR -44[rbp]
	add	eax, edx
	cdqe
	lea	rdx, -1[rax]
	mov	rax, QWORD PTR -40[rbp]
	add	rax, rdx
	movzx	eax, BYTE PTR [rax]
	cmp	al, -1
	jne	.L11
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	findLastSeq, .-findLastSeq
	.globl	inputLine
	.type	inputLine, @function
inputLine:
.LFB7:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	edi, 1
	call	malloc@PLT
	mov	QWORD PTR -8[rbp], rax
	call	getchar@PLT
	mov	BYTE PTR -13[rbp], al
	mov	DWORD PTR -12[rbp], 0
	jmp	.L15
.L16:
	mov	eax, DWORD PTR -12[rbp]
	movsx	rdx, eax
	mov	rax, QWORD PTR -8[rbp]
	add	rdx, rax
	movzx	eax, BYTE PTR -13[rbp]
	mov	BYTE PTR [rdx], al
	add	DWORD PTR -12[rbp], 1
	mov	eax, DWORD PTR -12[rbp]
	add	eax, 1
	movsx	rdx, eax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	realloc@PLT
	mov	QWORD PTR -8[rbp], rax
	call	getchar@PLT
	mov	BYTE PTR -13[rbp], al
.L15:
	cmp	BYTE PTR -13[rbp], -1
	jne	.L16
	mov	eax, DWORD PTR -12[rbp]
	movsx	rdx, eax
	mov	rax, QWORD PTR -8[rbp]
	add	rax, rdx
	mov	BYTE PTR [rax], -1
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	inputLine, .-inputLine
	.section	.rodata
.LC0:
	.string	"%d"
.LC1:
	.string	"%s"
	.text
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
	sub	rsp, 48
	mov	DWORD PTR -36[rbp], edi
	mov	QWORD PTR -48[rbp], rsi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	mov	DWORD PTR -28[rbp], 0
	lea	rax, -28[rbp]
	mov	rsi, rax
	lea	rdi, .LC0[rip]
	mov	eax, 0
	call	__isoc99_scanf@PLT
	mov	eax, 0
	call	inputLine
	mov	QWORD PTR -24[rbp], rax
	mov	edx, DWORD PTR -28[rbp]
	mov	rax, QWORD PTR -24[rbp]
	mov	esi, edx
	mov	rdi, rax
	call	findLastSeq
	mov	QWORD PTR -16[rbp], rax
	mov	edi, 10
	call	putchar@PLT
	mov	rax, QWORD PTR -16[rbp]
	mov	rsi, rax
	lea	rdi, .LC1[rip]
	mov	eax, 0
	call	printf@PLT
	mov	edi, 10
	call	putchar@PLT
	mov	eax, 0
	mov	rcx, QWORD PTR -8[rbp]
	xor	rcx, QWORD PTR fs:40
	je	.L20
	call	__stack_chk_fail@PLT
.L20:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
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
