
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL setApp
GLOBAL setRegCalc
GLOBAL setRegShell
GLOBAL getErrorRIP

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL int_80h


GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN systemCall

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1

	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	mov rax, [rsp]
	mov [errorRIP], rax
	cmp byte [app], 0
	jne .other
	mov rax, [ripCalc]
	mov QWORD [rsp], rax
	mov rax, [rbpCalc]
	mov QWORD [rsp + 24], rax
	jmp .continue
.other: mov rax, [ripShell]
	mov QWORD [rsp], rax
	mov rax, [rbpShell]
	mov QWORD [rsp + 24], rax

.continue:	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

int_80h:
	push rbp
	mov rbp, rsp
	push rcx
	mov rcx, rax
	call systemCall
	pop rcx
	mov rsp, rbp
	pop rbp 
	sti
	ret 


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;ChangeScreen
_irq06Handler:
	irqHandlerMaster 6



;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

setRegCalc:
	push rbp
	mov rbp, rsp

	mov QWORD [ripCalc], rdi
	mov QWORD [rbpCalc], rsi

	mov rsp, rbp
	pop rbp
	ret

setRegShell:
	push rbp
	mov rbp, rsp

	mov QWORD [ripShell], rdi
	mov QWORD [rbpShell], rsi

	mov rsp, rbp
	pop rbp
	ret
setApp:
	push rbp
	mov rbp, rsp

	mov QWORD [app], rdi

	mov rsp, rbp
	pop rbp
	ret

getErrorRIP:
	push rbp
	mov rbp, rsp

	mov rax, [errorRIP]

	mov rsp, rbp
	pop rbp
	ret

SECTION .bss
	aux resq 1
	ripCalc resq 1
	rbpCalc resq 1
	ripShell resq 1
	rbpShell resq 1
	app resb 1
	errorRIP resq 1