#ifndef LIB_H
#define LIB_H

#include <stdint.h>


void print(char * c);

void getChar(char * c);

void deleteChar();

void newLine();

void numToChar(double num, char * c);

int charToNum(char * c, uint64_t * num);

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

void * malloc(uint64_t size);

void free(void * add);

uint64_t create_process(uint64_t codeEntry, uint64_t argc, uint64_t argv, uint8_t priority);

uint64_t kill(uint64_t pid);

void ps();

uint64_t switch_state(uint64_t pid);

uint64_t currentPid();

int sleep(uint64_t sec);
void _sti();

int changePriority(uint64_t pid, uint8_t newPriority);

uint64_t mem();

void sem_open(char * name, uint64_t value);

int sem_close(char * name);

int sem_wait(char * name);

int sem_post(char * name);

void print_sem();

#endif