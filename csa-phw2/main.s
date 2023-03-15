	.intel_syntax noprefix
	.text
	.globl	findLastSeq
	.type	findLastSeq, @function
findLastSeq:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	QWORD PTR -40[rbp], rdi			# rdi = str => -40[rbp] = str
	mov	DWORD PTR -44[rbp], esi			# esi = n => -44[rbp] = n
	mov	eax, DWORD PTR -44[rbp]			# eax = n
	cdqe
	mov	rdi, rax						# rdi = n
	call	malloc@PLT
	mov	QWORD PTR -8[rbp], rax			# char *last_seq = (char *)malloc(n * sizeof(char)) => -8[rbp] = last_seq
	mov	DWORD PTR -24[rbp], 0			# int i = 0 => -24[rbp] = i
	mov	DWORD PTR -20[rbp], 0			# is_seq_correct = 0 => -20[rbp] = is_seq_correct
	jmp	.L2
.L11:
	mov	DWORD PTR -16[rbp], 1			# int j = 1 => -16[rbp] = j
	jmp	.L3
.L7:
	mov	edx, DWORD PTR -24[rbp]			# edx = i
	mov	eax, DWORD PTR -16[rbp]			# eax = j
	add	eax, edx						# eax = j + i
	movsx	rdx, eax					# rdx = j + i
	mov	rax, QWORD PTR -40[rbp]			# rax = str
	add	rax, rdx
	movzx	edx, BYTE PTR [rax]			# edx = str[j + i]
	mov	ecx, DWORD PTR -24[rbp]			# ecx = i
	mov	eax, DWORD PTR -16[rbp]			# eax = j
	add	eax, ecx						# eax = j + i
	cdqe
	lea	rcx, -1[rax]					# rcx = j + i - 1
	mov	rax, QWORD PTR -40[rbp]			# rax = str
	add	rax, rcx
	movzx	eax, BYTE PTR [rax]			# eax = str[j + i - 1]
	cmp	dl, al							# if (str[j + i] == str[j + i - 1])
	jle	.L4
	mov	DWORD PTR -20[rbp], 1			# is_seq_correct = 1
	jmp	.L13
.L4:
	mov	DWORD PTR -20[rbp], 0			# is_seq_correct = 0
	jmp	.L6
.L13:
	add	DWORD PTR -16[rbp], 1			# ++j
.L3:
	mov	eax, DWORD PTR -16[rbp]			# eax = j
	cmp	eax, DWORD PTR -44[rbp]			# for (int j = 1; j < n; j++)
	jl	.L7
.L6:
	cmp	DWORD PTR -20[rbp], 0			# if (is_seq_correct)
	je	.L8
	mov	DWORD PTR -12[rbp], 0			# int j = 0 => -12[rbp] = j	
	jmp	.L9
.L10:
	mov	edx, DWORD PTR -24[rbp]			# edx = i
	mov	eax, DWORD PTR -12[rbp]			# eax = j
	add	eax, edx
	movsx	rdx, eax					# rdx = i + j
	mov	rax, QWORD PTR -40[rbp]			# rax = str
	add	rax, rdx
	mov	edx, DWORD PTR -12[rbp]			# edx = j
	movsx	rcx, edx					# rcx = j
	mov	rdx, QWORD PTR -8[rbp]			# rdx = last_seq
	add	rdx, rcx
	movzx	eax, BYTE PTR [rax]			# eax = str[i + j]
	mov	BYTE PTR [rdx], al				# last_seq[j] = str[i + j]
	add	DWORD PTR -12[rbp], 1			# ++j
.L9:
	mov	eax, DWORD PTR -12[rbp]			# eax = j
	cmp	eax, DWORD PTR -44[rbp]			# for (int j = 0; j < n; j++)
	jl	.L10
.L8:
	add	DWORD PTR -24[rbp], 1			# ++i
.L2:
	mov	edx, DWORD PTR -24[rbp]			# edx = i
	mov	eax, DWORD PTR -44[rbp]			# eax = n
	add	eax, edx						# eax = i + n
	cdqe
	lea	rdx, -1[rax]					# rdx = i + n - 1
	mov	rax, QWORD PTR -40[rbp]			# rax = str
	add	rax, rdx						# rax = str[i + n - 1]
	movzx	eax, BYTE PTR [rax]			# eax = str[i + n - 1]	
	cmp	al, -1							# while (str[i + n - 1] == EOF)
	jne	.L11
	mov	rax, QWORD PTR -8[rbp]			# return last_seq => rax = last_seq	
	leave
	ret
	.size	findLastSeq, .-findLastSeq
	.globl	inputLine
	.type	inputLine, @function
inputLine:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	edi, 1
	call	malloc@PLT
	mov	QWORD PTR -8[rbp], rax			# char *str = (char *)malloc(1 * sizeof(char)) => -8[rbp] = str
	call	getchar@PLT
	mov	BYTE PTR -13[rbp], al			# char c = getchar() => -13[rbp] = c
	mov	DWORD PTR -12[rbp], 0			# int i = 0 => -12[rbp] = i
	jmp	.L15
.L16:
	mov	eax, DWORD PTR -12[rbp]			# eax = i
	movsx	rdx, eax					# rdx = i
	mov	rax, QWORD PTR -8[rbp]			# rax = str
	add	rdx, rax
	movzx	eax, BYTE PTR -13[rbp]		# eax = c
	mov	BYTE PTR [rdx], al				# str[i] = c
	add	DWORD PTR -12[rbp], 1			# ++i
	mov	eax, DWORD PTR -12[rbp]			# eax = i
	add	eax, 1
	movsx	rdx, eax					# rdx = i + 1
	mov	rax, QWORD PTR -8[rbp]			# rax = str
	mov	rsi, rdx						# rsi = i + 1
	mov	rdi, rax						# rdi = str
	call	realloc@PLT
	mov	QWORD PTR -8[rbp], rax			# str = (char *)realloc(str, (i + 1) * sizeof(char)) => -8[rbp] = str
	call	getchar@PLT
	mov	BYTE PTR -13[rbp], al			# c = getchar() => -13[rbp] = c
.L15:
	cmp	BYTE PTR -13[rbp], -1			# while (c != EOF)
	jne	.L16
	mov	eax, DWORD PTR -12[rbp]			# eax = i
	movsx	rdx, eax					# rdx = i
	mov	rax, QWORD PTR -8[rbp]			# rax = str
	add	rax, rdx
	mov	BYTE PTR [rax], -1				# str[i] = EOF
	mov	rax, QWORD PTR -8[rbp]			# return str => rax = str
	leave
	ret
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
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	DWORD PTR -36[rbp], edi			# int argc => -36[rbp] = argc
	mov	QWORD PTR -48[rbp], rsi			# char **argv => -48[rbp] = argv
	mov	DWORD PTR -28[rbp], 0			# int n = 0 => -28[rbp] = n
	lea	rax, -28[rbp]					# rax = &n
	mov	rsi, rax						# rsi = &n
	lea	rdi, .LC0[rip]					# rdi = "%d"
	mov	eax, 0							# eax = 0
	call	__isoc99_scanf@PLT			# scanf("%d", &n)
	mov	eax, 0							# eax = 0
	call	inputLine
	mov	QWORD PTR -24[rbp], rax			# char *str = inputLine() => -24[rbp] = str
	mov	edx, DWORD PTR -28[rbp]			# edx = n
	mov	rax, QWORD PTR -24[rbp]			# rax = str
	mov	esi, edx						# esi = n
	mov	rdi, rax						# rdi = str
	call	findLastSeq
	mov	QWORD PTR -16[rbp], rax			# char *last_seq = findLastSeq(str, n) => -16[rbp] = last_seq
	mov	edi, 10							# edi = "\n"
	call	putchar@PLT					# putchar("\n")
	mov	rax, QWORD PTR -16[rbp]			# rax = last_seq
	mov	rsi, rax						# rsi = last_seq
	lea	rdi, .LC1[rip]					# rdi = "%s"
	mov	eax, 0							# eax = 0
	call	printf@PLT					# printf("%s", last_seq)
	mov	edi, 10							# edi = "\n"
	call	putchar@PLT					# putchar("\n")
	mov	eax, 0							# return 0 => eax = 0
	leave
	ret
