#include <keyboardDriver.h>
#include <lib.h>
#include <video_driver.h>
#include <interrupts.h>


#define KEYS 59
#define PRESSED 1
#define RELEASED 2
#define ERRROR -1

#define L_SHFT 0x2A
#define R_SHFT 0x36
#define B_SPACE 0x0E
#define SPACE 0x39
#define CAPS_LCK 0x3A
#define ENTER 0x1C
#define CTRL 0x1D

#define IS_LETTER(c) (c >= 'a' && c <= 'z' ? 1 : 0)

#define ABS(num) (num >= 0 ? num : num * -1)

static uint8_t action(uint8_t scanCode);

static char buffer[70] = {0};
static int actBuffer = 0;
static char bufferAux[70];
static int actBufferAux = 0;

static const char pressCodes[KEYS][2] =
    {{0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'},
    {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('},
    {'0', ')'}, {'-', '_'}, {'=', '+'}, {0, 0}, {0, 0}, {'q', 'Q'}, 
    {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, 
    {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, 
    {']', '}'}, {0, 0}, {0, 0}, {'a', 'A'}, {'s', 'S'}, 
    {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'},
    {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\''}, {'`', '~'},
    {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, 
    {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, 
    {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

static uint8_t scanCode, currentAction, specialChars[] = {0, 0}, capsLock = 0, control=0;

int color[3] = {255, 255, 255};

void keyboard_handler(){

    if (hasKey())
    {
        scanCode = getKey();
        currentAction = action(scanCode);
        if (currentAction == PRESSED)
        {
           
            switch (scanCode)
            {
            case L_SHFT:
                specialChars[0] = 1;
                break;

            case R_SHFT:
                specialChars[1] = 1;
                break;

            case ENTER:
                buffer[actBuffer] = 10;
                actBuffer++;
                break;

            case CAPS_LCK:
                capsLock = capsLock == 1 ? 0 : 1;
                break;

            case SPACE:

                buffer[actBuffer] = ' ';
                actBuffer++;
                break;

            case B_SPACE:
                buffer[actBuffer] = 8;
                actBuffer ++;
                break;

            case CTRL:
                control = 1;
                break;

            default:
                if(control == 1 && scanCode == 2){
                    buffer[actBuffer] = -2;
                    actBuffer++;  
                }
                else if(control == 1 && scanCode == 3){
                    saveReg();
                }
                else if(control == 1 && scanCode == 4)
                {
                    buffer[actBuffer] = -3;
                    actBuffer++;
                }
                
                else{
                    if (pressCodes[scanCode][0])
                    {
                        if (!IS_LETTER(pressCodes[scanCode][0]))
                        {
                        buffer[actBuffer] = (pressCodes[scanCode][specialChars[0] | specialChars[1]]);
                    
                        actBuffer++;
                    

                        }
                        else
                        {
                        buffer[actBuffer] = (pressCodes[scanCode][ABS(capsLock - (specialChars[0] | specialChars[1]))]);
                        actBuffer++;
                        }
                    }
                }
            }
            
        }
        else if (currentAction == RELEASED)
            {
                switch (scanCode)
                {
                case L_SHFT | 0x80:
                    specialChars[0] = 0;
                    break;
                case R_SHFT | 0x80:
                    specialChars[1] = 0;
                    break;
                case CTRL | 0x80:
                    control = 0;
                    break;
                }
                
            }
        
    }
}

static uint8_t action(uint8_t scanCode)
{
    if (scanCode >= 0x01 && scanCode <= 0x3A)
        return PRESSED;
    else if (scanCode >= 0x81 && scanCode <= 0xBA)
        return RELEASED;

    return ERRROR;
}

void  getNChar(char * resp, int n){
   while(actBuffer == 0){
       _hlt();
   }
    int i = 0;
    while (i<n && actBuffer > i) {
        resp[i] = buffer[i];
        i++;
    }
    resp[i] = 0;

    for(int j=n, k=0;j<actBuffer;j++, k++){
        buffer[k] = buffer[j];
    }
    actBuffer-=i;

  
}

void changeBuffer(){
    char * aux = buffer;
    memcpy(buffer, bufferAux, 70);
    memcpy(bufferAux, aux, 70);
    
    int aux2 = actBuffer;
    actBuffer = actBufferAux;
    actBufferAux = aux2;

}