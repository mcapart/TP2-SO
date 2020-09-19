#include <video_driver.h>
#include <stdint.h>
#include <lib.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6

static void zero_division();
static void invalidOpcode();

extern uint64_t getErrorRIP();


void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else if(exception == INVALID_OPCODE_EXCEPTION_ID){
		invalidOpcode();
	}
}

static void zero_division() {

	saveReg();
	int color[3] = {0, 0, 255};
	int color2[3] ={255, 255, 255};
	newLine();
	writeWord("No se puede dividir por 0", 1.5, color );
	newLine();
	char regs[16][7] = {"rax: ", "rbx: ", "rcx: ", "rdx: ", "rbp: ", "rdi: ", "rsi: ", "r8: ", "r9: ", "r10: ", "r11: ", "r12: ", "r13: ", "r14: ", "r15: ", "rsp: "};
    uint64_t v[16] = {0};
     getReg(v);
    char text[70];
	writeWord("rip: ", 1.5, color2);
	numToChar(getErrorRIP(), text);
	writeWord(text, 1.5, color2);
	newLine();
    for(int i=0;i<16;i++){
        numToChar(v[i], text);
        writeWord(regs[i], 1.5, color2);
        writeWord(text, 1.5, color2);
        newLine();
    }
	newLine();

}

static void invalidOpcode(){
	saveReg();
	int color[3] = {0, 0, 255};
	int color2[3] ={255, 255, 255};
	newLine();
	writeWord("Codigo de operacion invalido", 1.5, color );
	newLine();
	char regs[16][7] = {"rax: ", "rbx: ", "rcx: ", "rdx: ", "rbp: ", "rdi: ", "rsi: ", "r8: ", "r9: ", "r10: ", "r11: ", "r12: ", "r13: ", "r14: ", "r15: ", "rsp: "};
    uint64_t v[16] = {0};
     getReg(v);
    char text[70];
	writeWord("rip: ", 1.5, color2);
	numToChar(getErrorRIP(), text);
	writeWord(text, 1.5, color2);
	newLine();
    for(int i=0;i<16;i++){
        numToChar(v[i], text);
        writeWord(regs[i], 1.5, color2);
        writeWord(text, 1.5, color2);
        newLine();
    }
	newLine();
}