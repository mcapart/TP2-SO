#include <lib.h>
#include <stdint.h>
#include <shell.h>
#include <test_util.h>
#include <philosopher.h>
#include <stddef.h>
#define CANT_FUNC 25
#define FOREGROUND 1
#define BACKGROUND 0
#define MAX_CAT 200

char fun[CANT_FUNC][40] = {
    "get time",
    "get cpu info",
    "get cpu temperature",
    "inforeg",
    "try divide",
    "try invalid opcode",
    "help",
    "ps",
    "loop",
    "mem",
    "sem",
    "pipe",
    "test mem",
    "test process",
    "test prio",
    "test sync",
    "test no sync",
    "phylo",
    "cat",
    "filter",
    "wc",
    "nice",
    "kill",
    "block",
    "printMem"};
char descFun[CANT_FUNC][150] = {
    ": Imprime la hora actual del sistema",
    ": Imprime informacion del cpu",
    ": Imprime la temperatura del cpu",
    ": Imprime el contenido de los registros cuando fueron guardados",
    ": Ejemplo de excepcion de dividir por 0",
    ": Ejemplo de excepcion de codigo invalido",
    ": Imprime las funciones disponibles y su descripcion",
    ": Imprime una lista de todos los procesos con su pid, su estado y el BP",
    ": Crea un proceso loop que imprime su pid despues de una cantidad de segundo",
    ": Iprime el estado de la memoria",
    ": Imprime una lista con todos los semaforos activos, su estado, y los procesos bloqueados",
    ": Imprime una lista con todos los pipes activos, y los procesos blockeados",
    ": Hace un testeo de la memoria",
    ": Hace un testeo de creacion, bloqueo, desbloqueo y muerte de los procesos",
    ": Hace un test de los cambios de prioridad",
    ": Hace un test de semaforos",
    ": Test que muestra que pasa si no usas semafors",
    ": Problema de los filosofos",
    ": Imprime lo que escribis en el STDIN",
    ": Imprime lo que escribis en el STDIN filtrando las vocales",
    ": Cuenta la cantidad de lineas del STDIN",
    ": Recibe como parametro el ID de un proceso y una nueva prioridad y le asgina a ese procesos esa prioridad",
    ": Recibe como parametro el ID de un proceso, y lo mata",
    ": Recibe como parametro el ID de un proceso, y lo bloquea",
    ": Volcado de memoria desde la posicion pasada como argumento"};

static char buffer[70] = {0};
static int n = 0;

static void help()
{
    newLine();
    for (int i = 0; i < CANT_FUNC; i++)
    {
        print(fun[i]);
        print(descFun[i]);
        newLine();
        newLine();
    }
}

static void print_Mem()
{
    uint64_t memUsed = mem();

    print("Memoria ocupada: ");
    char num[20];
    numToChar(memUsed, num);
    print(num);
    newLine();
}

static void printTime()
{
    uint64_t h;
    uint64_t m;
    uint64_t s;
    getHour(&h);
    getMin(&m);
    getSec(&s);
    if (h < 3)
    {
        switch (h)
        {
        case 0:
            h = 21;
            break;
        case 1:
            h = 22;
            break;
        case 2:
            h = 23;
            break;
        case 3:
            h = 24;
            break;

        default:
            break;
        }
    }
    else
    {
        h -= 3;
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

static void cpuInfo()
{
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

static int strComp(char *c1, char *c2,int* amp)
{
    int i = 0;
    int j = 0;
    while (c1[j] == ' ' && c1[j] != 0)
    {
        j++;
    }
    for (; c1[j] != 0 && c2[i] != 0; i++, j++)
    {
        if (c1[j] != c2[i])
        {
            return c1[j] - c2[i];
        }
    }

    if(c2[i] == 0 && c1[j]=='&'){
        *amp = 1;
        j++;
    }
    
    while (c1[j] == ' ' && c1[j] != 0)
    {
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

static int valid(char d, int isHexa)
{
    return (d >= '0' && d <= '9') || (isHexa && d >= 'a' && d <= 'f');
}

static int specialStrComp(char *c1, char *c2, char *arg, int isHex)
{
    int i = 0;
    int j = 0;
    int n = 0;
    while (c1[j] == ' ' && c1[j] != 0)
    {
        j++;
    }
    for (; c1[j] != 0 && c2[i] != 0; i++, j++)
    {
        if (c1[j] != c2[i])
        {
            return c1[j] - c2[i];
        }
    }
    if (c2[i])
    {
        return -1;
    }
    if (c1[j] != ' ')
    {
        return 1;
    }
    while (c1[j] == ' ' && c1[j] != 0)
    {
        j++;
    }
    if (!valid(c1[j], isHex))
    {
        return 1;
    }
    while (valid(c1[j], isHex))
    {
        arg[n] = c1[j];
        j++;
        n++;
    }
    while (c1[j] == ' ' && c1[j] != 0)
    {
        j++;
    }
    if (c1[j])
    {
        return 1;
    }
    arg[n] = 0;
    return 0;
}

static int specialStrComp2(char *c1, char *c2, char *arg1, char *arg2, int isHex)
{
    int i = 0;
    int j = 0;
    int n = 0;
    int s = 0;
    while (c1[j] == ' ' && c1[j] != 0)
    {
        j++;
    }
    for (; c1[j] != 0 && c2[i] != 0; i++, j++)
    {
        if (c1[j] != c2[i])
        {
            return c1[j] - c2[i];
        }
    }
    if (c2[i])
    {
        return -1;
    }
    if (c1[j] != ' ')
    {
        return 1;
    }
    while (c1[j] == ' ' && c1[j] != 0)
    {
        j++;
    }
    if (!valid(c1[j], isHex))
    {
        return 1;
    }
    while (valid(c1[j], isHex))
    {
        arg1[n] = c1[j];
        j++;
        n++;
    }
    while (c1[j] == ' ' && c1[j] != 0)
    {
        j++;
    }
    arg1[n] = 0;
    if (!valid(c1[j], isHex))
    {
        return 1;
    }
    while (valid(c1[j], isHex))
    {
        arg2[s] = c1[j];
        j++;
        s++;
    }
    while (c1[j] == ' ' && c1[j] != 0)
    {
        j++;
    }

    if (c1[j])
    {
        return 1;
    }
    arg2[s] = 0;
    return 0;
}

static int getFunction(char *c, int * amp)
{
    for (int i = 0; i < CANT_FUNC - 1; i++)
    {
        if (strComp(c, fun[i], amp) == 0)
        {
            return i;
        }
        
    }
    return -1;
}

static void getCpuTemp()
{
    uint64_t targetTemp;
    uint64_t statusTemp;
    getTemp(&targetTemp, &statusTemp);
    targetTemp = ((targetTemp >> 16) & 0xff);
    statusTemp = ((statusTemp >> 16) & 0x7f);
    char text[70];
    targetTemp = targetTemp - statusTemp;
    numToChar(targetTemp, text);
    print(text);
    newLine();
    return;
}

static void inforeg()
{
    char regs[16][7] = {"rax: ", "rbx: ", "rcx: ", "rdx: ", "rbp: ", "rdi: ", "rsi: ", "r8: ", "r9: ", "r10: ", "r11: ", "r12: ", "r13: ", "r14: ", "r15: ", "rsp: "};
    uint64_t v[16] = {0};
    getReg(v);
    char text[70];
    for (int i = 0; i < 16; i++)
    {
        numToChar(v[i], text);
        print(regs[i]);
        print(text);
        newLine();
    }
}

static void printMem(uint8_t mem)
{
    uint8_t vec[32] = {0};
    getMem(mem, vec);
    char text[70];
    for (int i = 0; i < 32; i++)
    {
        if (i != 0 && i % 4 == 0)
        {
            newLine();
        }
        numToCharHex(vec[i], text);
        print(text);
        print("  ");
    }
    newLine();
}

static void tryDivide()
{
    print("intento dividir por 0");
    newLine();
    int i = 1 / 0;
}

static void tryInvalidOpcodes()
{
    print("intento un codigo de operacion invalido");
    newLine();
    tryInvalidOpcode();
}

static void loop()
{

    //Ahora esta haciendo una espera activa pero la idea es cambiarlo
    //cuando tengamos implementado un sleep o algo por el estilo
    uint64_t pid = currentPid();
    //cada 5 o 10 segundos imprime

    char num[20];
    
    while (1)
    {
        /* j=0;
        while(j<99999999){
            j++;
        }*/
        sleep(3);
        newLine();
        print("Hola! :) este es mi PID: ");
        numToChar(pid, num);
        print(num);
        print("$");
        newLine();

        
    }
    
}

static void cat()
{
    char buffer[MAX_CAT] = {0};

    int i = 0;
    char *buff = buffer;
    do
    {
        read(0, buff, 1);
        if (buffer[i] == 10)
        {
            newLine();
        }
        if (buffer[i] == 8 && i > 0)
        {
            i--;
            buff--;
            buffer[i] = 0;
            deleteChar();
        }
        else
        {
            write(1, buff, 1);
        }
        buff++;
        i++;
    } while (i < MAX_CAT && buffer[i - 1] != '$');

    newLine();
    int j = 0;
    buff = buffer;
    while (j < i && buffer[j] != '$')
    {
        if (*buff == 10)
        {
            newLine();
        }
        else
        {
            write(1, buff, 1);
        }
        buff++;
        j++;
    }
}


int isVocal(char a)
{
    return (a == 'a' || a == 'A' || a == 'e' || a == 'E' || a == 'i' || a == 'I' || a == 'o' || a == 'O' || a == 'u' || a == 'U');
}

static void filter()
{
    while(1){
        char buffer[MAX_CAT] = {0};

        int i = 0;
        char *buff = buffer;
        do
        {
            read(0, buff, 1);
            if(buffer[i] == '#'){
                return;
            }
            if (buffer[i] == 10)
            {
                newLine();
            }
            if (buffer[i] == 8 && i > 0)
            {
                i--;
                buff--;
                buffer[i] = 0;
                deleteChar();
            }
            else
            {
                write(1, buff, 1);
            }
            buff++;
            i++;
        } while (i < MAX_CAT && buffer[i - 1] != '$');

        newLine();
        int j = 0;
        buff = buffer;
        while (j < i && buffer[j] != '$')
        {
            if (*buff == 10)
            {
                newLine();
            }
            else
            {
                if (!isVocal(*buff))
                {
                    write(1, buff, 1);
                }
            }
            buff++;
            j++;
        }
        newLine();
        newLine();
    }
}

static void wc()
{
    char buffer[MAX_CAT] = {0};

    int i = 0;
    int lines = 1;
    char *buff = buffer;
    do
    {
        read(0, buff, 1);
        if (buffer[i] == 10)
        {
            newLine();
            lines++;

        }
        if (buffer[i] == 8 && i > 0)
        {
            i--;
            buff--;
            buffer[i] = 0;
            deleteChar();
        }
        else
        {
            write(1, buff, 1);
        }
        buff++;
        i++;
    } while (i < MAX_CAT && buffer[i - 1] != '$');

    newLine();
    char num[20] = {0};
    numToChar(lines, num);
    print("Cantidad de lines: ");
    print(num);
    newLine();
   

}

static void createLoopProces()
{
    print("Creando proceso");
    newLine();
    char **argv = malloc(16);
    argv[0] = "loop";
    create_process((uint64_t)&loop, 1, argv, 1, BACKGROUND);
}

char * isPipeThere(char* c){
    if(*c == '!'){
        return NULL;
    }
    for(; *c !=0;){
        if(*c == '!'){
            *c = 0;
            c++;
            return c;
        }
        c++;
    }
    return NULL;
}

static int startFunction(char *c)
{
     int amp = 0;
    char * c1 = isPipeThere(c);
    if(c1 != NULL){
        int left = getFunction(c, &amp);
        int right = getFunction(c1, &amp);
        if(left == -1 || right == -1){
            newLine();
            return 0;
        }
        
        if(left == 8){
            if(right == 19){
                int fd = pipe("loop_pipe");
                int aux;
                aux = dup2(fd, 1);
                if(aux == -1){
                    print("Algo malo");
                }
                char **argv2 = malloc(16);
                argv2[0] = "loop";
                create_process((uint64_t)&loop, 1, argv2, 3, BACKGROUND);
                aux =dup2(1, 1);
                if(aux == -1){
                    print("Algo malo 2");
                }
                aux = dup2(fd, 0);
                if(aux == -1){
                    print("Algo malo");
                }    
                char **argv = malloc(16);
                argv[0] = "filter";
                create_process((uint64_t)&filter, 1, argv, 3, BACKGROUND);
                dup2(0, 0);
                close_pipe(fd);
                return 1;
            }
            if(right == 20){
                int fd = pipe("loop_pipe");
                dup2(fd, 1);

                createLoopProces();
                dup2(1, 1);
                dup2(fd, 0);    
                char **argv = malloc(16);
                argv[0] = "wc";
                create_process((uint64_t)&wc, 1, argv, 3, BACKGROUND);
                dup2(0, 0);
                return 1;
                
            }
        }
        if(left == 17){
            if(right ==19){
                int fd = pipe("phylo_pipe");
                dup2(fd, 1);
                char **argv = malloc(16);
                argv[0] = "phylo";
                create_process((uint64_t)&phylo_table, 1, argv, 3, BACKGROUND);
                dup2(1, 1);
                dup2(fd, 0);
                char **argv2 = malloc(16);
                argv2[0] = "filter";
                create_process((uint64_t)&filter, 1, argv2, 3, BACKGROUND);
                dup2(0, 0);
                return 1;

            }
            if(right == 20){
                int fd = pipe("phylo_pipe");
                dup2(fd, 1);
                char **argv = malloc(16);
                argv[0] = "phylo";
                create_process((uint64_t)&phylo_table, 1, argv, 3, BACKGROUND);
                dup2(1, 1);
                dup2(fd, 0);    
                char **argv2 = malloc(16);
                argv2[0] = "wc";
                create_process((uint64_t)&wc, 1, argv2, 3, BACKGROUND);
                dup2(0, 0);
                return 1;
            }
        }
        return 0;
    }
    int i = getFunction(c, &amp);
    char arg[20];
    char arg2[20];
    if (specialStrComp(c, fun[CANT_FUNC - 1], arg, 1) == 0)
    {
        newLine();
        uint8_t num;
        charToNumHex(arg, &num);
        printMem(num);
        return 1;
    }
    if (specialStrComp(c, fun[CANT_FUNC - 2], arg, 0) == 0)
    {
        newLine();
        uint64_t num;
        charToNum(arg, &num);
        switch_state(num);
        return 1;
    }
    if (specialStrComp(c, fun[CANT_FUNC - 3], arg, 0) == 0)
    {
        newLine();
        uint64_t num;
        charToNum(arg, &num);
        kill(num);
        return 1;
    }
    if (specialStrComp2(c, fun[CANT_FUNC - 4], arg, arg2, 0) == 0)
    {
        newLine();
        uint64_t pid;
        charToNum(arg, &pid);
        uint64_t num;
        charToNum(arg2, &num);
        changePriority(pid, num);
        return 1;
    }

    if (i == 0)
    {
        printTime();
        return 1;
    }
    if (i == 1)
    {
        cpuInfo();
        return 1;
    }
    if (i == 2)
    {
        getCpuTemp();
        return 1;
    }
    if (i == 3)
    {
        inforeg();
        return 1;
    }
    if (i == 4)
    {
        tryDivide();
        return 1;
    }
    if (i == 5)
    {
        tryInvalidOpcodes();
        return 1;
    }
    if (i == 6)
    {
        help();
        return 1;
    }
    if (i == 7)
    {
        ps();
        return 1;
    }
    if (i == 8)
    {
        createLoopProces();
        return 1;
    }
    if (i == 9)
    {
        print_Mem();
        return 1;
    }
    if (i == 10)
    {
        print_sem();
        return 1;
    }
    if (i == 11)
    {
        print_pipes();
        return 1;
    }
    if (i == 12)
    {
        char **argv = malloc(16);
        argv[0] = "test mm";
        create_process((uint64_t)&test_mm, 1, argv, 1, BACKGROUND);
        return 1;
    }
    if (i == 13)
    {
        char **argv = malloc(16);
        argv[0] = "test process";
        create_process((uint64_t)&test_processes, 1, argv, 1, BACKGROUND);
        return 1;
    }
    if (i == 14)
    {
        char **argv = malloc(16);
        argv[0] = "test prio";
        create_process((uint64_t)&test_prio, 1, argv, 2, BACKGROUND);
        return 1;
    }
    if (i == 15)
    {
        char **argv = malloc(16);
        argv[0] = "test sync";
        create_process((uint64_t)&test_sync, 1, argv, 2, BACKGROUND);
        return 1;
    }
    if (i == 16)
    {
        char **argv = malloc(16);
        argv[0] = "test no sync";
        create_process((uint64_t)&test_no_sync, 1, argv, 2, BACKGROUND);
        return 1;
    }
    if (i == 17)
    {
        char **argv = malloc(16);
        argv[0] = "phylo";
        if(!amp){
            create_process((uint64_t)&phylo_table, 1, argv, 3, FOREGROUND);
            return 1;
        }
        create_process((uint64_t)&phylo_table, 1, argv, 3, BACKGROUND); 
        return 1;
    }
    if (i == 18)
    {
        if(amp){
            print("No se puede correr cat en background, no tiene sentido...");
            return 0;
        }
        char **argv = malloc(16);
        argv[0] = "cat";
        create_process((uint64_t)&cat, 1, argv, 3, FOREGROUND);
        return 1;
    }
    if (i == 19)
    {
        if(amp){
            print("No se puede correr filter en background, no tiene sentido...");
            return 0;
        }
        char **argv = malloc(16);
        argv[0] = "filter";
        create_process((uint64_t)&filter, 1, argv, 3, FOREGROUND);
        return 1;
    }
     if (i == 20)
    {
        if(amp){
            print("No se puede correr wc en background, no tiene sentido...");
            return 0;
        }
        char **argv = malloc(16);
        argv[0] = "wc";
        create_process((uint64_t)&wc, 1, argv, 3, FOREGROUND);
        return 1;
    }
    

    return 0;
}

int shell()
{
    print("> ");
    char text[10] = {0};
    while (text[0] != 10 && n < 70)
    {
        getChar(text);

        if (text[0] == 8 && n > 0)
        {
            n--;
            buffer[n] = 0;
            deleteChar();
        }
        else if (text[0] == -3)
        {
            deleteAll(n);
            n = 0;
        }
        else if (text[0] != 8)
        {
            buffer[n] = text[0];
            n++;
            print(text);
        }
    }
    buffer[n - 1] = 0;
    newLine();
    int flag = startFunction(buffer);
    newLine();
    if (!flag)
    {
        print("Comando Invalido");
        newLine();
        newLine();
    }
    n = 0;
    buffer[0] = 0;
    text[0] = 0;

    return -1;
}

void reStartShell()
{
    buffer[n] = 0;
    n = 0;
}
