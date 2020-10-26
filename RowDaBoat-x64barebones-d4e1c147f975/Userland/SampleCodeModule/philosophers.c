#include <philosopher.h>
#include <lib.h>

void block_philos();
void kill_philos();

char * sem_table = "philo_sem";
phylo_struct phylo[MAX_PHILOSOPHERS];
int philos = 0;

int left(int n){
    return (n+ philos-1) % philos;
}

int right(int n){
    return (n+1) % philos;
}

void print_philos(){
    for(int j = 0; j < philos; j++){
        switch(phylo[j].state){
            case TERMINO:
                print("M");
                break;
            case COMIENDO:
                print("C");
                break;
            case PENSANDO:
                print("P");
                break;
            case HAMBRIENTO:
                print("H");
                break;
            default:
                print(".");
                break;
        }
    }
    newLine();
}

void print_tutorial(){
    print("M representa a un filosofo que va a morir.");
    newLine();
    print("C representa a un filosofo comiendo.");
    newLine();
    print("P representa a un filosofo pensando.");
    newLine();
    print("H representa a un filosofo hambriento.");
    newLine();
    print("presionar \"a\" para agregar un nuevo filosofo a la mesa (a menos que ya esten la maxima cantidad de filosofos sentados).");
    newLine();
    print("presionar \"r\" para remover a un nuevo filosofo de la mesa (a menos que hayan la minima cantidad de filosofos sentados).");
    newLine();
    print("presionar \"q\" para terminar el proceso.");
    newLine();
}

void pensar(){
    sleep(1);
}
void comer(){
    sleep(1);
}

void test(int n){
    if(phylo[n].state == TERMINO){
        return;
    }
    if(phylo[n].state == HAMBRIENTO && phylo[left(n)].state != COMIENDO && phylo[right(n)].state != COMIENDO){
        phylo[n].state = COMIENDO;
        sleep(1);

       int aux= sem_post(phylo[n].waiter);
      
    }
}

void agarrar_tenedor(int n){
    if(phylo[n].state == TERMINO){
        return;
    }
    int aux = sem_wait(sem_table);
   
    phylo[n].state = HAMBRIENTO;
    test(n);

    aux = sem_post(sem_table);
   
    aux = sem_wait(phylo[n].waiter);


}

void dejar_tenedor(int n){
    if(phylo[n].state == TERMINO){
        return;
    }
    int aux = sem_wait(sem_table);
   
    phylo[n].state = PENSANDO;
    test(left(n));
    test(right(n));
    aux = sem_post(sem_table);
   
}

void philosopher( int argc, char ** argv){
    
  
    if(argc < 4){
        return -1;
    }
    int n = argv[2];
    phylo[n].state = PENSANDO;
    while(phylo[n].state != TERMINO){
        pensar();
        agarrar_tenedor(n);
        print_philos();
        comer();
        dejar_tenedor(n);
        print_philos();
    }
    
}

void create_philosopher_process(){
     phylo[philos].n = philos;
        phylo[philos].state = PENSANDO;
   
        numToChar(philos, phylo[philos].waiter);
        sem_open(phylo[philos].waiter, 0);
        char ** argv = malloc(sizeof(char *)*6);
         argv[0] = "philosopher";
         argv[1] = phylo;
         argv[2] = philos;
         argv[3] = &philos;
         phylo[philos].pid = create_process((uint64_t)&philosopher, 4, argv, 1, 0);
}

int phylo_table(){
    
    print_tutorial();
    char c;
    sem_open(sem_table, 1);
    for(philos = 0; philos<MIN_PHILOSOPHERS;philos++){
        create_philosopher_process();
    }
    print("Comenzando a comer");
    newLine();
    newLine();
    getChar(&c);
    while( c!='q') {
        if(c == 'a'){
            sem_wait(sem_table);  
            if(philos < MAX_PHILOSOPHERS){
                
                create_philosopher_process();
                philos++;
              
                

            }else{
                newLine();
                print("No hay mas espacio para agregar mas filosofos a la mesa");
                newLine();
            }
            sem_post(sem_table);

        }
        if(c == 'r'){
            sem_wait(sem_table);
            if(philos > MIN_PHILOSOPHERS){
                philos--;
                phylo[philos].state = TERMINO;
                kill(phylo[philos].pid);
                sem_close(phylo[philos].waiter);
                
            }else{
                newLine();
                print("Se alcanzo la minima cantidad de filosofos en la mesa, no es posible eliminar a uno");
                newLine();
            }
            sem_post(sem_table);
        

        }
        uint64_t i = 0;
        while(i<99999999){
            i++;
        }
        getChar(&c);
    }
    block_philos();
    kill_philos();

    sem_close(sem_table);
    print("Finalizado");
    newLine();
  
    return 0;


}

void block_philos(){
    for(int i = 0;i<philos;i++){
        block(phylo[i].pid);
    }
}
void kill_philos(){
    while(philos > 0){
        philos--;
        kill(phylo[philos].pid);
        sem_close(phylo[philos].waiter);
    }
}

