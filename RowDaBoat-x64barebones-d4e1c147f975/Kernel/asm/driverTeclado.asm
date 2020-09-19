GLOBAL hasKey
GLOBAL getKey


section .text

hasKey:
    push rbp
    mov rbp,rsp

    mov rax,0
    in al,64h
    and al,0x1

    leave
    ret

getKey:
    push rbp
    mov rbp,rsp

    mov rax,0
    in al,60h

    leave
    ret