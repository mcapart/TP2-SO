#ifndef PROCES_H
#define PROCES_H

#include <stdint.h>
#include <stdio.h>


#define MAX_PROCESSES 30
#define PROCESS_STACK_SIZE 4*1024 
#define PRIORITY_MAX 5
//ESTADOS



typedef struct processStruct{
    uint64_t MemPointer;
    uint64_t sp;
    int pid;
    uint64_t bp;
    uint64_t ip;
    char ** argv;
    uint8_t priority;
} processStruct;

#include <scheduler.h>
int create_proces(uint64_t codeEntry, int argc, char ** argv, uint8_t priority);
uint64_t initializeStack(uint64_t stack, uint64_t code, uint64_t argc, char ** argv, uint64_t pid);
uint64_t killProcess(processStruct * process);




#endif