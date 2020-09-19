GLOBAL getTemperature

section .text

getTemperature:
    push rbp
    mov rbp, rsp

    push rcx
    push rdx
    push rax

    mov QWORD [rdi], 6883840

    mov QWORD[rsi], 2285305856
    
	mov rsp, rbp
	pop rbp
    ret



