#ifndef PHYLO_H
#define PHYLO_H

#define MAX_LENGTH 30
#define MIN_PHILOSOPHERS 5
#define MAX_PHILOSOPHERS 10

typedef struct phylo_struct {
   int state;
   int n;
   char waiter[10];
   int pid;
}phylo_struct;

enum states {PENSANDO, HAMBRIENTO, COMIENDO, TERMINO};

int phylo_table();



#endif