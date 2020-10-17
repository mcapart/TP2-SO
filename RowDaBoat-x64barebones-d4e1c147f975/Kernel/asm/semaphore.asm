GLOBAL acquire
GLOBAL release

; Si lock = 0, la región crítica se encuentra desocupada.
; Si lock = 1, la región crítica se encuentra ocupada. 

acquire:
    mov     rax, 1          
    xchg    rax, [rdi]                           
    cmp     rax, 0          
    jne acquire
    ret

                           
 
release:
    mov     rax, 0
    xchg    rax, [rdi]
    ret