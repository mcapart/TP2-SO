#include <lib.h>

#include <shell.h>
#include <stddef.h>

static int app;

static void reverse(char * c, int l){
    int start = 0; 
    int end = l -1; 
    while (start < end) 
    { 
        char aux = c[start];
        c[start] = c [end];
        c[end] = aux;
        start++; 
        end--; 
    } 
}

void numToChar(double num, char * c){
    int entera = num;
    double decimal = num - entera;
    int i = 0;
    if(num < 0){
        entera = entera * -1;
        decimal = decimal * -1;
    }
    if(entera == 0){
        c[i] = '0';
        i++;
    }
    for(; i < 99 && entera > 0; i++){
        int valor = entera % 10;
        c[i] = valor + '0';
        entera = entera/10;
    }
    if(num < 0){
        c[i] = '-';
        i++;
    }
    reverse(c,i);
    if(decimal != 0){
        c[i] = '.';
      	i++;
        for(int j = 0;i < 100 && j < 4; i++, j++){
            int aux = decimal * 10;
            c[i] = (aux % 10) + '0';
            decimal = decimal * 10;
        }
    }
    c[i] = 0;
}

void initApp(int app){
    
        reStartShell();
    
}

int getApp(){
    return app;
}
void setApp(int a){
    app = a;
}

void numToCharHex(uint8_t num, char * c){
    int i = 0;
    if(num == 0){
        c[i] = '0';
        i++;
        c[i] = '0';
        i++;
    }
    for(; i < 99 && num > 0; i++){
        int valor = num % 16;
        c[i] = (valor < 10 ) ? valor + '0' : valor + 'a' - 10;
        num = num/16;
    }
    reverse(c,i);
    c[i] = 0;
}

int charToNumHex(char * d, uint8_t * num ){
    double resp = 0;
    int i = 0;
    for(; (d[i] >= '0' && d[i] <= '9') || (d[i]>= 'a' && d[i] <= 'f'); i++)
    {
        resp += (d[i] >= '0' && d[i] <= '9')? d[i] - '0' : d[i] - 'a' + 10;
        resp = resp * 16;
    }
    if(d[i] != 0){
        return 0;
    }

    resp = resp / 16;

    *num = resp;
    return 1;
}

int charToNum(char * d, uint64_t * num){
    double resp = 0;
    int i = 0;
    for(; (d[i] >= '0' && d[i] <= '9'); i++)
    {
        resp += d[i] - '0';
        resp = resp * 10;
    }
    if(d[i] != 0){
        return 0;
    }

    resp = resp / 10;

    *num = resp;
    return 1;
}

void deleteAll(int n){
    while(n > 0){
        deleteChar();
        n--;
    }
}

void getChar(char* c){
    read(0, c, 1);
}

int strLen(char * s){
    if(s == NULL){
        return 0;
    }
    int i = 0;
    while(s[i] != 0){
        i++;
    }
    return i;
}

void print(char * s){
    write(1, s, strLen(s));
}