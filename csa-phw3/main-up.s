	.intel_syntax noprefix
	.text
	.globl	ex
	.type	ex, @function
ex:
	push	rbp
	mov	rbp, rsp
	movsd	QWORD PTR -72[rbp], xmm0	# xmm0 = x => -72[rbp] = x
	fldz
	fstp	TBYTE PTR -64[rbp]			# long double sum = 0 => -64[rbp] = 0
	fld1
	fchs
	fstp	TBYTE PTR -48[rbp]			# long double result_sum = -1 => -48[rbp] = -1
	fld1
	fstp	TBYTE PTR -32[rbp]			# long double step = 1 => -32[rbp] = 1
	mov	r13d, 0							# int n = 0 => r13d = 0
	jmp	.L2
.L3:
	fld	TBYTE PTR -64[rbp]				# st(0) = sum
	fstp	TBYTE PTR -48[rbp]			# result_sum = sum => -48[rbp] = sum
	fld	TBYTE PTR -64[rbp]				# st(0) = sum
	fld	TBYTE PTR -32[rbp]				# st(0) = step, st(1) = sum
	faddp	st(1), st					# sum += step
	fstp	TBYTE PTR -64[rbp]			# -64[rbp] = sum
	fld	QWORD PTR -72[rbp]				# st(0) = x, st(1) = sum
	fld	TBYTE PTR -32[rbp]				# st(0) = step, st(1) = x, st(2) = sum
	fmulp	st(1), st					# x *= step
	add		r13d, 1						# ++n
	mov -4[rsp], r13d
	fild	DWORD PTR -4[rsp]			# st(0) = n, st(1) = x, st(2) = sum
	fdivp	st(1), st					# x /= n
	fstp	TBYTE PTR -32[rbp]			# step = x / n => -32[rbp] = x / n
.L2:
	fld	TBYTE PTR -64[rbp]				# st(0) = sum
	fld	TBYTE PTR -48[rbp]				# st(0) = result_sum, st(1) = sum
	fucomip	st, st(1)					# while (sum != result_sum)
	fstp	st(0)
	jp	.L3
	fld	TBYTE PTR -64[rbp]				# st(0) = sum
	fld	TBYTE PTR -48[rbp]				# st(0) = result_sum, st(1) = sum
	fucomip	st, st(1)					# while (sum != result_sum)
	fstp	st(0)
	jne	.L3
	fld	TBYTE PTR -48[rbp]				# st(0) = result_sum
	fstp	QWORD PTR -80[rbp]			# result_sum => -80[rbp]
	movsd	xmm0, QWORD PTR -80[rbp]	# return result_sum => xmm0 = result_sum
	pop	rbp
	ret
	.size	ex, .-ex
	.section	.rodata
.LC4:
	.string	"%lf \n"
	.text
	.globl	main
	.type	main, @function
main:
	endbr64
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi			# edi = argc => -20[rbp] = argc
	mov	QWORD PTR -32[rbp], rsi 		# rsi = argv => -32[rbp] = argv
	mov	rax, QWORD PTR -32[rbp]			# rax = argv
	add	rax, 8
	mov	rax, QWORD PTR [rax]			# rax = argv[1]
	mov	rdi, rax						# rdi = argv[1]
	call	atoi@PLT
	mov	DWORD PTR -4[rbp], eax			# int x = atoi(argv[1]) => -4[rbp] = eax = atoi(argv[1])
	cmp	DWORD PTR -4[rbp], 0			# if (x < 0) goto .L6
	js	.L6
	cvtsi2sd	xmm0, DWORD PTR -4[rbp]	# xmm0 = x
	call	ex
	lea	rdi, .LC4[rip]					# rdi = "%lf \n"
	mov	eax, 1
	call	printf@PLT					# printf("%lf \n", ex(x))
	jmp	.L7
.L6:
	mov	eax, DWORD PTR -4[rbp]			# eax = x
	neg	eax								# eax = -x
	cvtsi2sd	xmm0, eax				# xmm0 = -x
	call	ex							# xmm0 = ex(-x)
	movsd	xmm1, QWORD PTR .LC5[rip]	# xmm1 = 1
	divsd	xmm1, xmm0					# xmm0 = 1 / ex(-x)
	movapd	xmm0, xmm1					# xmm0 = 1 / ex(-x)
	lea	rdi, .LC4[rip]					# rdi = "%lf \n"
	mov	eax, 1
	call	printf@PLT					# printf("%lf \n", 1 / ex(-x))
.L7:
	mov	eax, 0							# return 0
	leave
	ret
.LC5:
	.long	0
	.long	1072693248
