/* sampleCodeModule.c */
#include <lib.h>
#include <calculator.h>
#include <shell.h>

char * v = (char*)0xB8000 + 79 * 2;

static int app = 1;
int main() {
	
	start();
	saveReturn(1);
	saveReturn(0);
	initApp(app);

	while(1){
		switch (app)
		{
		case 0 : app = calculator();
			break;	
		case 1 : app = shell();
			break;
		default:
			break;
		}
		
	}
	return 0;



}