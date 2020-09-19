#include <lib.h>
#include <stdint.h>
#include <shell.h>
#define CANT_FUNC 8

char fun[CANT_FUNC][40] = {"get time","get cpu info", "get cpu temperature", "inforeg", "try divide", "try invalid opcode", "help", "printMem"};
char descFun[CANT_FUNC][100] = {": Imprime la hora actual del sistema", ": Imprime informacion del cpu", ": Imprime la temperatura del cpu", ": Imprime el contenido de los registros cuando fueron guardados", ": ejemplo de excepcion de dividir por 0", ": ejemplo de excepcion de codigo invalido", ": Imprime las funciones disponibles y su descripcion", ": Volcado de memoria desde la posicion pasada como argumento"};
static char buffer[70] = {0};
static int n = 0;

static void help(){
    newLine();
    for(int i = 0; i < CANT_FUNC; i++){
        print(fun[i]);
        print(descFun[i]);
        newLine();
        newLine();
    }
}

static void printTime(){
    uint64_t h;
    uint64_t m;
    uint64_t s;
    getHour(&h);
    getMin(&m);
    getSec(&s);
    if(h < 3){
        switch (h)
        {
        case 0: h = 21;
            break;
        case 1: h = 22;
            break;
        case 2: h = 23;
            break;
        case 3: h = 24;
            break;
        
        default:
            break;
        }
    }
    else
    {
        h-=3;
    }
    char ch[3];
    char cm[3];
    char cs[3];
    numToChar(h, ch);
    numToChar(m, cm);
    numToChar(s, cs);
    print(ch);
    print(":");
    print(cm);
    print(":");
    print(cs);
    newLine();
}

static void cpuInfo(){
    char text[70];
    char text2[70];
    char text3[70];
    uint32_t num;
    getCpuVendor(text, &num);
    int modelo = (num & 0xFF0) >> 4;
    int familia = modelo >> 4;
    modelo = modelo & 0x0f;
    numToChar(modelo, text2);
    numToChar(familia, text3);
    print("CPU:");
    print(text);
    newLine();
    print("Modelo:");
    print(text2);
    newLine();
    print("Familia:");
    print(text3);
    newLine();
}

static int strComp(char * c1, char * c2){
    int i = 0;
    int j = 0;
    while(c1[j] == ' ' && c1[j] != 0){
            j++;
    }
    for(; c1[j] != 0 && c2[i] != 0; i++, j++){
        if(c1[j] != c2[i]){
            return c1[j] - c2[i];
        }
    }
    while(c1[j] == ' ' && c1[j] != 0){
        j++;
    }
    if (c1[j])
    {
        return 1;
    }
    if (c2[i])
    {
        return -1;
    }
    return 0;
}

static int valid(char d){
    return (d >= '0' && d <= '9') || (d>= 'a' && d <= 'f');
}

static int specialStrComp(char * c1, char * c2, char * arg){
    int i = 0;
    int j = 0;
    int n = 0;
    while(c1[j] == ' ' && c1[j] != 0){
            j++;
    }
    for(; c1[j] != 0 && c2[i] != 0; i++, j++){
        if(c1[j] != c2[i]){
            return c1[j] - c2[i];
        }
    }
    if (c2[i])
    {
        return -1;
    }
    if(c1[j] != ' '){
        return 1;
    }
    while(c1[j] == ' ' && c1[j] != 0){
        j++;
    }
    if(!valid(c1[j])){
        return 1;
    }
    while(valid(c1[j])){
        arg[n] = c1[j];
        j++;
        n++;
    }
    while(c1[j] == ' ' && c1[j] != 0){
        j++;
    }
    if (c1[j])
    {
        return 1;
    }
    arg[n] = 0;
    return 0;
}

static int getFunction(char * c){
    for(int i = 0; i < CANT_FUNC - 1; i++){
        if(strComp(c,fun[i]) == 0){
            newLine();
            return i;
        }
    }
    return -1;
}

static void getCpuTemp(){
    uint64_t targetTemp;
    uint64_t statusTemp;
    getTemp(&targetTemp, &statusTemp);
    targetTemp = ((targetTemp >> 16) & 0xff );
    statusTemp = ((statusTemp >> 16) & 0x7f );
    char text[70];
    targetTemp = targetTemp - statusTemp;
    numToChar(targetTemp, text);
    print(text);
    newLine();
    return;
}

static void inforeg(){
    char regs[16][7] = {"rax: ", "rbx: ", "rcx: ", "rdx: ", "rbp: ", "rdi: ", "rsi: ", "r8: ", "r9: ", "r10: ", "r11: ", "r12: ", "r13: ", "r14: ", "r15: ", "rsp: "};
    uint64_t v[16] = {0};
     getReg(v);
    char text[70];
    for(int i=0;i<16;i++){
        numToChar(v[i], text);
        print(regs[i]);
        print(text);
        newLine();
    }
}

static void printMem(uint8_t mem){
    uint8_t vec[32] = {0};
    getMem(mem, vec);
    char text[70];
    for(int i=0;i<32;i++){
        if(i!=0 && i%4==0){
            newLine();
        }
        numToCharHex(vec[i], text);
        print(text);
        print("  ");
    } 
    newLine();
}

static void tryDivide(){
    print("intento dividir por 0");
    newLine();
    int i = 1 / 0;
}

static void tryInvalidOpcodes(){
    print("intento un codigo de operacion invalido");
    newLine();
    tryInvalidOpcode();
}



static int startFunction(char * c){ 
    int i = getFunction(c);
    char arg[20];
    if(specialStrComp(c, fun[CANT_FUNC-1], arg) == 0){
        newLine();
        uint8_t num;
        charToNumHex(arg, &num);
        printMem(num);
        return 1;
        
    }
     

    if(i == 0){ 
        printTime();
        return 1;
    }
    if(i == 1) {
        cpuInfo();
        return 1;
    }
    if(i == 2) {
        getCpuTemp();
        return 1;
    }
    if(i == 3) {
        inforeg();
        return 1;
    }
    if(i==4){
        tryDivide();
        return 1;
    }
    if(i==5){
        tryInvalidOpcodes();
        return 1;
    }
    if(i == 6){
        help();
        return 1;
    }

    return 0;
    
}



int shell(){
    char text[10] = {0};
    while(text[0]!= 10 && n < 70){
        getChar(text);
        if(text[0] == 8 && n > 0){
            n--;
            buffer[n] = 0;
            deleteChar();
        }
        else if(text[0] == -2){
            changeApp(0);
            return 0;
        }
       else if(text[0] == -3){
            deleteAll(n);
            n = 0;
        }
        else if(text[0] != 8)
        {
            buffer[n] = text[0];
            n++;
            print(text);
        }
    }
    buffer[n-1] = 0;
    int flag = startFunction(buffer);
    newLine();
    if(!flag){
        print("Comando Invalido");
        newLine();
        newLine();
        
    }
    n = 0;
    buffer[0] = 0;
    text[0] = 0;


   return 1;
    
}

void reStartShell(){
    buffer[n] = 0;
    n = 0;
}
