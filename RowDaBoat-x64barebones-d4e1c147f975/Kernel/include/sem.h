#ifndef SEM_H
#define SEM_H
#include <stdint.h>

#define MAX_NAME 40
#define MAX_SEM 30

typedef struct {
    uint64_t pid;
    struct block_queue * next;
}block_queue;

typedef struct sem_struct{
    char name[MAX_NAME];
    uint64_t lock;
    int value;
    uint64_t cant_process;
    block_queue * first_blocked_process;

}sem_struct;

sem_struct * sem_open(char * name, int value);

int sem_close(char * name);

int sem_post(char * name);

int sem_wait(char * name);

int find_sem(char * name);

void acquire(uint64_t* lock);

void release(uint64_t* lock);

void print_sem();

#endif