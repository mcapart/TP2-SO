GLOBAL getHou
GLOBAL getMin
GLOBAL getSec

section .text
;uint8_t getRTC(int register)
getHou:
    push rbp
    mov rbp,rsp
    mov rax,0
    mov al,4
    out 70h,al
    in al,71h
    mov bl,al
    and al, 0xF0
    and bl,0x0F
    mov cl, 16
    div cl
    mov cl, 10
    mul cl
    add al, bl

    mov rsp,rbp
    pop rbp
    ret

getMin:
    push rbp
    mov rbp,rsp
    mov rax,0
    mov al,2
    out 70h,al
    in al,71h
    mov bl,al
    and al, 0xF0
    and bl,0x0F
    mov cl, 16
    div cl
    mov cl, 10
    mul cl
    add al, bl

    mov rsp,rbp
    pop rbp
    ret

getSec:
    push rbp
    mov rbp,rsp
    mov rax,0
    mov al,0
    out 70h,al
    in al,71h
    mov bl,al
    and al, 0xF0
    and bl,0x0F
    mov cl, 16
    div cl
    mov cl, 10
    mul cl
    add al, bl

    mov rsp,rbp
    pop rbp
    ret