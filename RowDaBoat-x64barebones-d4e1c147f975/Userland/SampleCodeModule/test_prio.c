#include <stdint.h>
#include <stdio.h>
#include <lib.h>

#define MINOR_WAIT 99999999                             // TODO: To prevent a process from flooding the screen
#define WAIT      999999999                          // TODO: Long enough to see theese processes beeing run at least twice

static void bussy_wait(uint64_t n){
  
  uint64_t i;
  for (i = 0; i < n; i++);
}
static uint64_t my_getpid(){
  return currentPid();
}


static void endless_loop(){
  uint64_t pid = my_getpid();
  char aux[50];
  numToChar(pid, aux);
  while(1){
    print(aux);
    print("  ");
    bussy_wait(MINOR_WAIT);
  }
}


static uint64_t my_create_process(char * name){
  char ** argv = malloc(16);
  argv[0] = "test_loop";
  return create_process((uint64_t)&endless_loop, 1, argv, 1, 0);
}

static uint64_t my_nice(uint64_t pid, uint64_t newPrio){
  return changePriority(pid, newPrio);
}

static uint64_t my_kill(uint64_t pid){
  return kill(pid);
}

static uint32_t my_block(uint32_t pid){
  return block(pid);
}

static uint32_t my_unblock(uint32_t pid){
  return unblock(pid);
}



#define TOTAL_PROCESSES 6

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process("endless_loop");

  bussy_wait(WAIT);
  newLine();
  print("CHANGING PRIORITIES...");
  newLine();

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        my_nice(pids[i], 1); //lowest priority 
        break;
      case 1:
        my_nice(pids[i], 3); //medium priority
        break;
      case 2:
        my_nice(pids[i], 5); //highest priority
        break;
    }
  }

  bussy_wait(WAIT);
  newLine();
  print("BLOCKING...");
  newLine();

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...");
  newLine();
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        my_nice(pids[i], 3); //medium priority 
        break;
      case 1:
        my_nice(pids[i], 3); //medium priority
        break;
      case 2:
        my_nice(pids[i], 3); //medium priority
        break;
    }
  }

  bussy_wait(WAIT);
  print("UNBLOCKING...");
  newLine();
  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  newLine();
  print("KILLING...");
  newLine();

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);

  print("DONE");
  newLine();
}


