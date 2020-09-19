#include <calculator.h>
#include <lib.h>

static int numOp(char c){
    switch (c)
    {
    case '+':
        return 0;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 3;
    case '(':
        return 4;
    case ')':
        return 5;
    default:
        return -1;
    }
}
int CANT_DECIMALES=1000000;

static int precedenceMatriz[5][6]={
                {1, 1,0,0,0,1},
                {1,1,0,0,0,1},
                {1,1,1,1,0,1},
                {1,1,1,1,0,1},
                {0,0,0,0,0,0}
        };


static int getPrecendence(char tope,char current){
    int topeIndex, currentIndex;
    topeIndex = numOp(tope);
    currentIndex = numOp(current);
    if (topeIndex == -1 || currentIndex == -1)
    {
        return -1;
    }
    return precedenceMatriz[topeIndex][currentIndex];
}

static int operate(double num1, double num2, char operator, int * flag){
    double resp;
    switch (operator){
        case '+':resp = num1 + num2;break;
        case '-':resp = num1 - num2;break;
        case '/':
            if(num2 != 0){
                resp = num1 / num2;break;
            }
            newLine();
            print("No se puede dividir por 0");
            flag[0] = 0;
            break;
        case '*':resp = num1 * num2;break;
        default: break;
    }
    return (int) (resp * CANT_DECIMALES); 
}

//devuelve 1 si c es un numero, 0 si no
static int is_number(char c){
    if(c < '0' || c > '9'){
        return 0;
    }
    return 1;
}

static int numChar(char * c, int entero, int decimal){
    double resp = 0;
    int i = 0;
    for(; i < entero; i++)
    {
        resp += c[i] - '0';
        resp = resp * 10;
    }

    resp = resp / 10;

    for(double aux = 10; i- entero < decimal && i < entero + 6; i++){
        resp = resp + (c[i] - '0') / aux;
        aux = aux * 10;
    }
    return (int) (resp * CANT_DECIMALES); 
}

static int evaluator(char * c, int * flag){
    char op[100]= {0};
    int itOp = 0;
    double num[100]= {0};
    int itNum = 0;
    for(int i = 0; c[i] != '='; i++){
    
        while(c[i] == ' '){
            i++;
        }

        if(is_number(c[i]) || (numOp(c[i])== 1 && is_number(c[i+1]))){
            
            int punto = 0;
            char resp[20];
            int itResp = 0;
            int cantEnt = 0;
            int cantDec = 0;
            int negativo = 0;
            
            if(c[i] == '-'){
                negativo = 1;
                i++;
            }
            while(c[i] != ' '){
                if(is_number(c[i])){
                    resp[itResp] = c[i];
                    if(punto){
                        cantDec++;
                    }
                    else{
                        cantEnt++;
                    }
                    itResp++;
                }
                else if(c[i] == '.' && !punto){
                    punto = 1;
                }
                else{
                    newLine();
                    print("No es un numero en base 10");
                    flag[0] = 0;
                    return 0;
                }
                i++;
            }
           if(!negativo){
                num[itNum] = (double) numChar(resp,cantEnt,cantDec) / CANT_DECIMALES;
            }
            else{
                num[itNum] = -1 * ((double) numChar(resp,cantEnt,cantDec) / CANT_DECIMALES);
            }
            itNum++;
            for(int i =0;i<itResp;i++){
                resp[i] = 0;
            }
            itResp = 0;
           

                
        }
        else if(numOp(c[i]) != -1){
            if(c[i+1] != ' '){
                newLine();
                print("Los operadores se separan con espacios");
                flag[0] = 0;
                return 0;
            }
            if(itOp == 0){
                op[itOp] = c[i];
                itOp++;
            }
            else
            {
                while(itOp != 0 && getPrecendence(op[itOp-1], c[i])){
                    if(itNum < 2){
                        newLine();
                        print("No es una expresion valida (demasiados operadores para la cantidad de numeros)");
                        flag[0] = 0;
                        return 0;
                    }
                    double resp = (double) operate(num[itNum-2],num[itNum-1],op[itOp-1], flag) / CANT_DECIMALES;
                    if(flag[0] == 0){
                        return 0;
                    }
                    num[itNum-2] = resp;
                    itOp--;
                    itNum--;
                }
                if(c[i] == ')'){
                    if(itOp == 0 || op[itOp-1] != '('){
                        newLine();
                        print("Los parentesis no estan apariados");
                        flag[0] = 0;
                        return 0;
                    }
                    itOp--;
                }
                else
                {
                    op[itOp++] = c[i];
                }
            }

        }
        else if(c[i] == '='){
            i--;
        }
        else{
            newLine();
            print("No es una expresion valida");
            flag[0] = 0;
            return 0;
        }
       
      
    }
   
    while(itOp > 0){
        if(op[itOp-1] == '('){
            newLine();
            print("Los parentesis no estan apariados");
            flag[0] = 0;
            return 0;
        }
         if(itNum < 2){
            newLine();
            print("No es una expresion valida (demasiados operadores para la cantidad de numeros)");
            flag[0] = 0;
            return 0;
        }
        double resp = (double) operate(num[itNum-2],num[itNum-1],op[itOp-1], flag) / CANT_DECIMALES;
        if(flag[0] == 0){
            return 0;
        }
        num[itNum-2] = resp;
        itOp--;
        itNum--;
    }
    if(itNum > 1){
            newLine();
            print("No es una expresion valida (demasiados numeros para la cantidad de operadores)");
            flag[0] = 0;
            return 0;
    }

   
    int v = num[0] * CANT_DECIMALES;
   
   
    return v;
}

static char buffer[70] = {0};
static int n = 0;


int calculator(){


    char text[10] = {0};
    
    while(text[0]!= '=' && n < 70){
        getChar(text);
        if(text[0] == 8 && n > 0){
            n--;
            buffer[n] = 0;
            deleteChar();
        }
        else if(text[0] == -2){
            changeApp(1);
            return 1;
        }
        else if(text[0] == -3){
            deleteAll(n);
            n = 0;
        }
        else if(is_number(text[0]) || numOp(text[0]) != -1 || text[0] == ' ' || text[0] == '=' || text[0] == '.')
        {
            buffer[n] = text[0];
            n++;
            print(text);
        }
        
    }
    if(n == 70){
        buffer[n-1] = '=';
        print("=");
    }
    int flag = 1;
    double v = evaluator(buffer, &flag) / 1000000.0;
    if(flag){
        char t[40];
        numToChar(v, t);
        print(" ");
        print(t);
    }
    buffer[n] = 0;
    n = 0;
    newLine();
    return 0;
    
   
}

void reStartCalculator(){
    buffer[n] = 0;
    n = 0;
}
