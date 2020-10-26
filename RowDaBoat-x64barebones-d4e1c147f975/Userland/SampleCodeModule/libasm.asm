GLOBAL read
GLOBAL write
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
GLOBAL malloc
GLOBAL free
GLOBAL create_process
GLOBAL kill
GLOBAL ps
GLOBAL switch_state
GLOBAL currentPid
GLOBAL sleep
GLOBAL _sti
GLOBAL changePriority
GLOBAL mem
GLOBAL sem_open
GLOBAL sem_close
GLOBAL sem_wait
GLOBAL sem_post
GLOBAL print_sem
GLOBAL pipe
GLOBAL close_pipe
GLOBAL print_pipes
GLOBAL dup2
GLOBAL giveCPU
GLOBAL block
GLOBAL unblock
GLOBAL blockShell
GLOBAL unblockShell

section .text

_sti:
    sti 
    ret

; int write(int fd, char * s, int length)
write:
    push rbp
    mov rbp, rsp

    mov rax, 2
    int 80h
    
    mov rsp, rbp
    pop rbp
    ret
           
;int read(int fd, char * s, int length)
read:
    push rbp
    mov rbp, rsp

    mov rax, 1   
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void * malloc(size_t size)
malloc:
    push rbp
    mov rbp, rsp

    
    mov rax, 3  
    int 80h
    

    mov rsp, rbp
    pop rbp
    ret

free:
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

;uint64_t create_process(uint64_t codeEntry, uint64_t argc, uint64_t argv)
create_process:
    push rbp
    mov rbp, rsp
    
    mov rax, 16
    int 80h

    mov rsp, rbp
    pop rbp
    ret
    
;uint64_t kill(uint64_t pid);
kill:
    push rbp 
    mov rbp, rsp
    
    mov rax, 17
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void ps();
ps:
    push rbp
    mov rbp, rsp
    
    mov rax, 18
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;uint64_t switch_state(uint64_t pid);
switch_state:
    push rbp
    mov rbp, rsp
 
    mov rax, 19
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;uint64_t currentPid();
currentPid:
    push rbp
    mov rbp, rsp
 
    mov rax, 20
    int 80h

    mov rsp, rbp
    pop rbp
    ret

sleep:
    push rbp
    mov rbp, rsp
    
    mov rax, 21
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;int changePriority(uint64_t pid, uint8_t newPriority)
changePriority:
    push rbp
    mov rbp, rsp
    
    mov rax, 22
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;uint64_t mem();
mem:
     push rbp
    mov rbp, rsp
    
    mov rax, 23
    int 80h

    mov rsp, rbp
    pop rbp
    ret


;void sem_open(char * name, uint64_t value);
sem_open:
 push rbp
    mov rbp, rsp
    
    mov rax, 24
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;int sem_close(char * name);
sem_close:
 push rbp
    mov rbp, rsp
    
    mov rax, 25
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;int sem_wait(char * name);
sem_wait:
 push rbp
    mov rbp, rsp
    
    mov rax, 26
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;int sem_post(char * name);
sem_post:
 push rbp
    mov rbp, rsp
    
    mov rax, 27
    int 80h

    mov rsp, rbp
    pop rbp
    ret
print_sem:
 push rbp
    mov rbp, rsp
    
    mov rax, 28
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;int pipe(int fd)
pipe:
 push rbp
    mov rbp, rsp
    
    mov rax, 29
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;int close_pipe(int fd)
close_pipe:
 push rbp
    mov rbp, rsp
    
    mov rax, 30
    int 80h

    mov rsp, rbp
    pop rbp
    ret

;void  print_pipes()   
print_pipes:
 push rbp
    mov rbp, rsp
    
    mov rax, 31
    int 80h

    mov rsp, rbp
    pop rbp
    ret   

;void dup2(int fd1, int fd2);
dup2:
 push rbp
    mov rbp, rsp
    
    mov rax, 32
    int 80h

    mov rsp, rbp
    pop rbp
    ret  
;void giveCpu();
giveCPU:
 push rbp
    mov rbp, rsp
    
    mov rax, 33
    int 80h

    mov rsp, rbp
    pop rbp
    ret   

block:
 push rbp
    mov rbp, rsp
    
    mov rax, 34
    int 80h

    mov rsp, rbp
    pop rbp
    ret   

unblock:
 push rbp
    mov rbp, rsp
    
    mov rax, 35
    int 80h

    mov rsp, rbp
    pop rbp
    ret   

blockShell:
 push rbp
    mov rbp, rsp
    
    mov rax, 36
    int 80h

    mov rsp, rbp
    pop rbp
    ret   

unblockShell:
 push rbp
    mov rbp, rsp
    
    mov rax, 37
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





