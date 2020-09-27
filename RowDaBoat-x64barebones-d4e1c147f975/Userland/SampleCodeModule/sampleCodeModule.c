/* sampleCodeModule.c */
#include <lib.h>
#include <calculator.h>
#include <shell.h>
#include <lib.h>

char * v = (char*)0xB8000 + 79 * 2;

int main() {
	print("estoy en el main");
	while(1){
		shell();
	}
	return 0;

}