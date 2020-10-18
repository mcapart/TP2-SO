#ifndef _PIPES_H
#define _PIPES_H

#include <stdint.h>
#include <memory_manager.h>
#include <video_driver.h>
#include <sem.h>
#include <stddef.h>
#include <lib.h>
#include <keyboardDriver.h>

#define MAX_LENGTH_NAME_FD 40
#define MAX_LENGTH_BUFFER_FD 200
#define MAX_FD 120
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct fd_struct{
    char name[MAX_LENGTH_NAME_FD];
    char buffer[MAX_LENGTH_BUFFER_FD];
    int fd;
    int isPipe;
    int index_r;
    int index_w;
    int process_counter;
    sem_struct * sem_r;
    sem_struct * sem_w;
}fd_struct;

void fd_init();

fd_struct * addFD(char * name, int isPipe);

int pipe(char * name);

int read_fd(int fd,  char * str, int length);

void print_pipes();

int closeFD(int fd);

int write_fd(int fd, const char * str, int length);
#endif