GLOBAL print 
GLOBAL getChar 
GLOBAL changeApp
GLOBAL start
GLOBAL deleteChar
GLOBAL newLine
GLOBAL getHour
GLOBAL getMin
GLOBAL getSec
GLOBAL getCpuVendor
GLOBAL getTemp
GLOBAL saveReg
GLOBAL getReg
GLOBAL getMem
GLOBAL saveReturn
GLOBAL tryInvalidOpcode

section .text

; void print(char *)
print:
    push rbp
    mov rbp, rsp

    mov rax, 2
    mov rsi, rdi
    int 80h
    
    mov rsp, rbp
    pop rbp
    ret
           
;void getChar(char *);
getChar:
    push rbp
    mov rbp, rsp

    mov rax, 1
    mov rsi, rdi 
    mov rdx, 1   
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void changeApp();
changeApp:
   push rbp
   mov rbp, rsp

   mov rax, 3
   int 80h

   mov rsp, rbp
   pop rbp

    ret

;void start();
start:
    push rbp
    mov rbp, rsp

    mov rax, 4  
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void deleteChar();
deleteChar:
    push rbp
    mov rbp, rsp

    mov rax, 5  
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void newLine();
newLine:
    push rbp
    mov rbp, rsp

    mov rax, 6  
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void getHour(uint64_t * v);
getHour:
    push rbp
    mov rbp, rsp

    mov rax, 7  
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void getMin(uint64_t * v);
getMin:
    push rbp
    mov rbp, rsp

    mov rax, 8 
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void getSec(uint64_t * v);
getSec:
  push rbp
    mov rbp, rsp

    mov rax, 9  
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getCpuVendor:
    push rbp
    mov rbp, rsp

    mov rax, 10
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getTemp:
    push rbp
    mov rbp, rsp

    mov rax, 11
    int 80h

    mov rsp, rbp
    pop rbp
    ret

saveReg:
    push rbp
    mov rbp, rsp

    mov rax, 12
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getReg:
    push rbp
    mov rbp, rsp

    mov rax, 13
    int 80h
    
    mov rsp, rbp
    pop rbp
    ret

getMem:
    push rbp
    mov rbp, rsp

    mov rax, 14
    int 80h
    
    mov rsp, rbp
    pop rbp
    ret

saveReturn:
    push rbp
    mov rsi, rbp
    mov rbp, rsp
    
    mov rdx, rdi
    mov rax, 15
    mov rdi, [rbp + 8]
    int 80h

    mov rsp, rbp
    pop rbp
    ret

tryInvalidOpcode:
    push rbp
    mov rbp, rsp

    ud2

    mov rsp, rbp
    pop rbp


