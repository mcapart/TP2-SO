#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <proces.h>
#include <video_driver.h>
#include <interrupts.h> 
#include <lib.h>
#include <stddef.h>


enum process_state {AVAILABLE, BLOCKED, EMPTY};

typedef struct {
    processStruct * process;
    uint8_t state;
    uint8_t quantum_left;
}processState; 

int make_available(int pid);

processStruct  * currentProcess();

void schedulerInitializer();

uint8_t addToList(processStruct * process);

uint64_t scheduler(uint64_t sp);

int find_process(uint64_t pid);

int blockProcess(uint64_t pid);

int kill(uint64_t pid);

char * state_to_string(uint8_t state);

int process_is_available(processState p);

void next_process();

void print_processes();

char * state_to_string(uint8_t state);

int switch_state( int pid );

uint64_t currentPid();

int sleepProcess(uint64_t sec);

int changePriority(uint64_t pid, uint8_t newPriority );

void giveCpu();



#endif