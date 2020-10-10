#include <proces.h>
#include <lib.h>
#include <memory_manager.h>
#include <stddef.h>
#include <video_driver.h>



#define STACK 18
#define ARGC 8
#define ARGV 11
#define PID 12
#define RIP 15
#define CODE 9


int static errorColor[3] = {0, 0, 255};



uint64_t initStack[] = {15,14,13,12,11,10,9,8,ARGC,CODE,5,ARGV,PID,2,1, RIP,0x08,0x202,STACK,0x0};
int pid=0; 

void process_wrapper(int codeEntry(uint64_t argc, char ** argv), uint64_t argc, char ** argv, int program_pid){
    codeEntry(argc, argv);
    kill(program_pid);
}

int create_proces(uint64_t codeEntry, int argc, char ** argv, uint8_t priority){
    processStruct * process = (processStruct *) malloc(sizeof(processStruct));
    if(process == NULL){
        writeWord("Error while creating process.", 1.5, errorColor);
        return -1;
    }
    if(priority <= 0 || priority > PRIORITY_MAX){
        
        writeWord("Invalid priority  ", 1.5, errorColor);
        char num[10] = {0};
        numToChar(priority, num);
        writeWord(num, 1.5, errorColor);
        return -1;
    }
    
    process->priority = priority;

    process->pid = pid;
    process->MemPointer = (uint64_t) malloc(PROCESS_STACK_SIZE);
    if(process->MemPointer == 0){
        writeWord("Error while creating process.", 1.5, errorColor);
        return -1;
    }
    uint64_t StartStack = process->MemPointer + PROCESS_STACK_SIZE - 8;

    
    process->sp = initializeStack(StartStack, codeEntry, argc, argv, process->pid);
    process->bp = StartStack; 
    process->ip = codeEntry;
    process->argv = argv;

    
    
    if(addToList(process)){
        writeWord("Error while creating process.", 1.5, errorColor);
        return -1;
    }
    return pid++;  
}

uint64_t initializeStack(uint64_t stack, uint64_t code, uint64_t argc, char ** argv, uint64_t pid){
    uint64_t sp = stack-20*8;
    initStack[ARGC] = argc;
    initStack[ARGV] = argv;
    initStack[STACK] = stack;
    initStack[PID] = pid;
    initStack[CODE] = code;
    initStack[RIP] =  (uint64_t)process_wrapper;
  
    memcpy((uint64_t*)sp, initStack, 20*8);
    return sp;
}

uint64_t killProcess(processStruct * process){
    free((void*)process->argv);
    free((void *)process->MemPointer);
    free(process);
    return 0;
}