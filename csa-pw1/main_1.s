        .intel_syntax noprefix
	.text
	.globl	makeArrayB
	.type	makeArrayB, @function
makeArrayB:
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-24], rdi         # rdi = a
        mov     QWORD PTR [rbp-32], rsi         # rsi = b
        mov     DWORD PTR [rbp-36], edx         # edx = n
        mov     DWORD PTR [rbp-4], 0            # int i = 0
        mov     DWORD PTR [rbp-8], 0            # int j = 0
        mov     DWORD PTR [rbp-12], -1          # int first_pos = -1
        jmp     .L2
.L5:
        mov     eax, DWORD PTR [rbp-4]          # eax =  i
        cdqe
        lea     rdx, [0+rax*4]
        mov     rax, QWORD PTR [rbp-24]         # rax = a
        add     rax, rdx                        # rax = a[i]
        mov     eax, DWORD PTR [rax]            # eax = a[i]
        test    eax, eax                        # if (a[i] <= 0)
        jle     .L3
        mov     eax, DWORD PTR [rbp-4]          # eax = i
        mov     DWORD PTR [rbp-12], eax         # first_pos = i
        jmp     .L4
.L3:                                            # ++i
        add     DWORD PTR [rbp-4], 1
.L2:
        mov     eax, DWORD PTR [rbp-4]          # eax = i 
        cmp     eax, DWORD PTR [rbp-36]         # i < n        
        jl      .L5
.L4:
        mov     eax, DWORD PTR [rbp-36]         # eax = n
        sub     eax, 1                          # eax = n - 1
        mov     DWORD PTR [rbp-4], eax          # i = n - 1
        jmp     .L6
.L8:
        mov     eax, DWORD PTR [rbp-4]          # eax = i
        cmp     eax, DWORD PTR [rbp-12]         # i == first_pos
        je      .L7
        mov     eax, DWORD PTR [rbp-4]          # eax = i
        cdqe
        lea     rdx, [0+rax*4]
        mov     rax, QWORD PTR [rbp-24]         # rax = a
        add     rax, rdx                        # rax = a[i]
        mov     edx, DWORD PTR [rbp-8]          # edx = j
        movsx   rdx, edx                        # rdx = j
        lea     rcx, [0+rdx*4]                  # rcx = j
        mov     rdx, QWORD PTR [rbp-32]         # rdx = b
        add     rdx, rcx                        # rdx = b[j]
        mov     eax, DWORD PTR [rax]            # eax = a[i]
        mov     DWORD PTR [rdx], eax            # b[j] = a[i]
        add     DWORD PTR [rbp-8], 1            # ++j
.L7:
        sub     DWORD PTR [rbp-4], 1            # --i
.L6:
        cmp     DWORD PTR [rbp-4], 0            # i >= 0
        jns     .L8
        cmp     DWORD PTR [rbp-12], -1          # first_pos == -1
        jne     .L9
        mov     eax, 0                          # return 0
        jmp     .L10
.L9:
        mov     eax, 1                          # return 1
.L10:
        pop     rbp
        ret
.LC0:
        .string "%d "
        .text
        .globl	outputArray
	.type	outputArray, @function
outputArray:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        mov     QWORD PTR [rbp-24], rdi         # int *arr
        mov     DWORD PTR [rbp-28], esi         # int n
        mov     DWORD PTR [rbp-4], 0            # int i = 0
        jmp     .L12
.L13:
        mov     eax, DWORD PTR [rbp-4]          # eax = i
        cdqe
        lea     rdx, [0+rax*4]                  # rdx = i
        mov     rax, QWORD PTR [rbp-24]         # rax = arr
        add     rax, rdx                        # rax = arr[i]
        mov     eax, DWORD PTR [rax]            # eax = arr[i]
        mov     esi, eax                        # esi = arr[i]
        lea	rax, .LC0[rip]                  # rax = "%d "
	mov	rdi, rax                        # rdi = "%d "
        mov     eax, 0
        call    printf@PLT                      # printf("%d ", arr[i])
        add     DWORD PTR [rbp-4], 1            # ++i
.L12:
        mov     eax, DWORD PTR [rbp-4]          # eax = i
        cmp     eax, DWORD PTR [rbp-28]         # i < n
        jl      .L13
        mov     edi, 10                         # rdi = '\n'
        call    putchar@PLT                     # printf('\n')
        nop
        leave
        ret
        .globl	main
	.type	main, @function
main:
        push    rbp
        mov     rbp, rsp
        push    rbx
        sub     rsp, 56
        mov     DWORD PTR [rbp-52], edi         # int argc
        mov     QWORD PTR [rbp-64], rsi         # char **argv
        mov     eax, DWORD PTR [rbp-52]         # eax = argc
        sub     eax, 1                          # eax = argc - 1
        cdqe
        sal     rax, 2
        mov     rdi, rax                        # rdi = (argc - 1)
        call    malloc@PLT
        mov     QWORD PTR [rbp-32], rax         # int *a = malloc((argc - 1) * sizeof(int))
        mov     eax, DWORD PTR [rbp-52]         # eax = argc
        sub     eax, 1                          # eax = argc - 1
        cdqe
        sal     rax, 2
        mov     rdi, rax                        # rdi = (argc - 1)
        call    malloc@PLT
        mov     QWORD PTR [rbp-40], rax         # int *b = malloc((argc - 1) * sizeof(int))
        mov     DWORD PTR [rbp-20], 1           # int i = 1
        jmp     .L15
.L16:
        mov     eax, DWORD PTR [rbp-20]         # eax = i
        cdqe
        lea     rdx, [0+rax*8]
        mov     rax, QWORD PTR [rbp-64]         # rax = argv
        add     rax, rdx                        # rax = argv[i]
        mov     rax, QWORD PTR [rax]            # rax = argv[i]
        mov     edx, DWORD PTR [rbp-20]         # edx = i
        movsx   rdx, edx                        # rdx = i
        sal     rdx, 2
        lea     rcx, [rdx-4]                    # rcx = i - 1
        mov     rdx, QWORD PTR [rbp-32]         # rdx = a
        lea     rbx, [rcx+rdx]                  # rbx = a[i - 1]
        mov     rdi, rax                        # rdi = argv[i]
        call    atoi@PLT
        mov     DWORD PTR [rbx], eax            # a[i - 1] = atoi(argv[i])
        add     DWORD PTR [rbp-20], 1           # ++i
.L15:
        mov     eax, DWORD PTR [rbp-20]
        cmp     eax, DWORD PTR [rbp-52]         # i < argc
        jl      .L16
        mov     eax, DWORD PTR [rbp-52]         # eax = argc
        lea     edx, [rax-1]
        mov     rcx, QWORD PTR [rbp-40]         # rcx = b
        mov     rax, QWORD PTR [rbp-32]         # rax = a
        mov     rsi, rcx                        # rsi = b
        mov     rdi, rax                        # rdi = a
        call    makeArrayB
        mov     DWORD PTR [rbp-44], eax         # int size_dif = makeArrayB(a, b, argc - 1)
        mov     eax, DWORD PTR [rbp-52]         # eax = argc
        sub     eax, 1                          # eax = argc - 1
        sub     eax, DWORD PTR [rbp-44]         # eax = argc - 1 - size_dif
        mov     edx, eax                        # edx = argc - 1 - size_dif
        mov     rax, QWORD PTR [rbp-40]         # rax = b
        mov     esi, edx                        # esi = argc - 1 - size_dif
        mov     rdi, rax                        # rdi = b
        call    outputArray                     # outputArray(b, argc - 1 - size_dif)
        mov     eax, 0                          # return 0
        mov     rbx, QWORD PTR [rbp-8]
        leave
        ret
        