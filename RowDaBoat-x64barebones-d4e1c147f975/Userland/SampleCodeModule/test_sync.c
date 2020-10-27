#include <stdint.h>
#include <stdio.h>
#include <lib.h>

#define N 1000000
#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"


int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  giveCPU();
  *p = aux;
}

static uint64_t my_sem_open(char *sem_id, uint64_t initialValue){
  return sem_open(sem_id, initialValue);
}

static uint64_t my_sem_wait(char *sem_id){
  return sem_wait(sem_id);
}

static uint64_t my_sem_post(char *sem_id){
  return sem_post(sem_id);
}

static uint64_t my_sem_close(char *sem_id){
  return sem_close(sem_id);
}

void inc(uint64_t argc, char ** argv){
  uint64_t i;
  uint64_t sem = (uint64_t) argv[1];
  int64_t value = (int64_t) argv[2];
  uint64_t n = (uint64_t) argv[3];
  if (sem && !my_sem_open(SEM_ID, 1)){
    print("ERROR OPENING SEM");
    newLine();
    return;
  }
  
  for (i = 0; i < N; i++){
    if (sem) my_sem_wait(SEM_ID);
    slowInc(&global, value);
    if (sem) my_sem_post(SEM_ID);
  }

  if (sem) my_sem_close(SEM_ID);
  
  print("Final value: ");
  char num[20];
  numToChar(global, num);
  print(num);
  newLine();

}


static uint32_t my_create_process(char * name, uint64_t sem, int64_t value, uint64_t n){
   char ** argv = malloc(16);
    argv[0] = "test_loop";
    argv[1] = (char *) sem;
    argv[2] = (char *) value;
    argv[3] = (char *) N;

    return create_process((uint64_t)&inc, 4, argv, 1, 0);

}





void test_sync(){
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITH SEM)");
  newLine();

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process("inc", 1, 1, N);
    my_create_process("inc", 1, -1, N);
  }
}

void test_no_sync(){
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITHOUT SEM)");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process("inc", 0, 1, N);
    my_create_process("inc", 0, -1, N);
  }
}


