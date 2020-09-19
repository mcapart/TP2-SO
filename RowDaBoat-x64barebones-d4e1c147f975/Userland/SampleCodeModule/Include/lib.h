#ifndef LIB_H
#define LIB_H

#include <stdint.h>


void print(char * c);

void getChar(char * c);

void changeApp(int i);

void start();
 
void deleteChar();

void newLine();

void numToChar(double num, char * c);

void getHour(uint64_t * v);

void getMin(uint64_t * v);

void getSec(uint64_t * v);

void getCpuVendor(char * c, uint32_t * v);

void getTemp(uint64_t * targetTemp, uint64_t * statusTemp);

void saveReg();

void getReg(uint64_t * v);

void getMem(uint8_t mem, uint8_t * v);

void initApp(int app);

int getApp();

void setApp(int a);

void saveReturn(int app);

void tryInvalidOpcode();

int charToNumHex(char * d, uint8_t * num );

void numToCharHex(uint8_t num, char * c);

void deleteAll(int n);

#endif