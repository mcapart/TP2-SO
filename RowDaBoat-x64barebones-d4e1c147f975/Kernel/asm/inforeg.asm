GLOBAL saveReg
GLOBAL getReg
section .text
;void saveReg();
saveReg:
    push rbp
    mov QWORD [vector + 32], rbp ;ver
    mov rbp, rsp

    mov QWORD [vector], rax 
    mov QWORD [vector + 8], rbx
    mov QWORD [vector + 16], rcx
    mov QWORD [vector + 24], rdx

    mov QWORD [vector + 40], rdi
    mov QWORD [vector + 48], rsi
    mov QWORD [vector + 56], r8
    mov QWORD [vector + 64], r9
    mov QWORD[vector + 72], r10
    mov QWORD[vector + 80], r11
	mov QWORD[vector + 88], r12
	mov QWORD[vector + 96], r13
	mov QWORD[vector + 104], r14
	mov QWORD[vector + 112], r15
    mov QWORD[vector + 120], rbp ; me estoy guardando es rsp antes de llamarlo

    mov rsp, rbp
    pop rbp
    ret

getReg:
    push rbp
    mov rbp, rsp

    mov rax, [vector];
    mov QWORD [rdi], rax;rax 
    mov rax, [vector + 8];
    mov QWORD [rdi + 8], rax;rax 
    mov rax, [vector + 16];
    mov QWORD [rdi + 16], rax;rax 
    mov rax, [vector + 24];
    mov QWORD [rdi + 24], rax;rax
    mov rax, [vector + 32];
    mov QWORD [rdi + 32], rax;rax
    mov rax, [vector + 40];
    mov QWORD [rdi + 40], rax;rax
    mov rax, [vector + 48];
    mov QWORD [rdi + 48], rax;rax
    mov rax, [vector + 56];
    mov QWORD [rdi + 56], rax;rax
    mov rax, [vector + 64];
    mov QWORD [rdi + 64], rax;rax
    mov rax, [vector + 72];
    mov QWORD [rdi + 72], rax;rax
    mov rax, [vector + 80];
    mov QWORD [rdi + 80], rax;rax
    mov rax, [vector + 88];
    mov QWORD [rdi + 88], rax;rax
    mov rax, [vector + 96];
    mov QWORD [rdi + 96], rax;rax
    mov rax, [vector + 104];
    mov QWORD [rdi + 104], rax;rax
    mov rax, [vector + 112];
    mov QWORD [rdi + 112], rax;rax
    mov rax, [vector + 120];
    mov QWORD [rdi + 120], rax;rax

    mov rsp, rbp
    pop rbp
    ret


section .bss
vector resq 16