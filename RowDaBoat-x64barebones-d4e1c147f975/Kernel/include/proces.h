#ifndef PROCES_H
#define PROCES_H

#include <stdint.h>
#include <stdio.h>


#define MAX_PROCESSES 30
#define PROCESS_STACK_SIZE 4*1024 
#define PRIORITY_MAX 5
#define MAX_FD_PROCESS 10
#define STDIN 0
#define STDOUT 1
#define STDERR 2
//ESTADOS



typedef struct processStruct{
    uint64_t MemPointer;
    uint64_t sp;
    int pid;
    int ppid;
    uint64_t bp;
    uint64_t ip;
    char ** argv;
    uint8_t priority;
    int fd_array[MAX_FD_PROCESS][2]; // 0 es la tabla del SO 1 es la tabla del proceso
    uint8_t fg;
} processStruct;

#include <scheduler.h>
int create_proces(uint64_t codeEntry, int argc, char ** argv, uint8_t priority, uint8_t fg);

uint64_t initializeStack(uint64_t stack, uint64_t code, uint64_t argc, char ** argv, uint64_t pid);

uint64_t killProcess(processStruct * process);

int dup2(int fd1, int fd2);

int getRealFD(int fd);

int remove_fd_process(int fd);

int add_fd_process(int fd, processStruct * p);
#endif