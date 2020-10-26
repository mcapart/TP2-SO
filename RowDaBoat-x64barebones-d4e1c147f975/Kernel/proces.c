#include <proces.h>
#include <lib.h>
#include <memory_manager.h>
#include <stddef.h>
#include <video_driver.h>
#include <scheduler.h>



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

int create_proces(uint64_t codeEntry, int argc, char ** argv, uint8_t priority, uint8_t fg){
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

    process->pid = pid++;
    if(pid-1 != 0){
        process->ppid = currentPid();
        processStruct * father = currentProcess(); 
        for(int i = 0; i < MAX_FD_PROCESS; i++){
            process->fd_array[i][0] = father->fd_array[i][0];
            process->fd_array[i][1] = father->fd_array[i][1];
        }
    }else{
        process->ppid = -1;
        for(int i=0;i<MAX_FD_PROCESS;i++){
            process->fd_array[i][0] = -1;
            process->fd_array[i][1] = -1;
        }
        add_fd_process(STDIN, process);
        add_fd_process(STDOUT, process);
        add_fd_process(STDERR, process);

    }
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
    process->fg = fg;

    

    
    
    if(addToList(process)){
        writeWord("Error while creating process.", 1.5, errorColor);
        return -1;
    }
    return pid-1;  
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

int add_fd_process(int fd, processStruct * p){
    if(p == NULL){
        return -1;
    }
    for(int i=0;i<MAX_FD_PROCESS;i++){
        if(p->fd_array[i][0] == -1){
            p->fd_array[i][0] = fd;
            p->fd_array[i][1] = fd;
            return 0;
        }
    }
    return -1;
}

void dup2(int fd1, int fd2){
    processStruct * p = currentProcess();
    if(p == NULL){
        return -1;
    }
     for(int i=0;i<MAX_FD_PROCESS;i++){
        if(p->fd_array[i][0] == fd2){
            p->fd_array[i][1] = fd1;
            return 0;
        }
    }
    return -1;
}

int getRealFD(int fd){
      processStruct * p = currentProcess();
    if(p == NULL){
        return -1;
    }
     for(int i=0;i<MAX_FD_PROCESS;i++){
        if(p->fd_array[i][0] == fd){
           return p->fd_array[i][1];
            
        }
    }
    return -1;
}

int remove_fd_process(int fd){
    processStruct * p = currentProcess();
    if(p == NULL){
        return -1;
    }
     for(int i=0;i<MAX_FD_PROCESS;i++){
        if(p->fd_array[i][0] == fd){
            p->fd_array[i][0] = -1;
            
        }
    }
    return 0;

}
