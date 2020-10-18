#include <scheduler.h>
#include <memory_manager.h>
int iterator;
int cant_process;
int first_time = 1;
int shell_pid;
int func_pid;

int static errorColor[3] = {0, 0, 255};
int static color[3]= {255, 255, 255};

#define VAR 25

typedef struct{
    uint64_t pid;
    uint64_t tick_left;
}sleepingProcess;

static processState process_list[MAX_PROCESSES];
static sleepingProcess sleeping_list[MAX_PROCESSES];

static int find_place();

void schedulerInitializer(){
    for(int i=0;i<MAX_PROCESSES;i++){
        process_list[i].state = EMPTY;
    }
    for(int i=0;i<MAX_PROCESSES;i++){
        sleeping_list[i].pid = -1;
    }
    cant_process = 0;
    char ** argv = malloc(16);
    argv[0] = "sampleCOdeModule";

    shell_pid = create_proces(0x400000, 1, argv, 5);
    if(shell_pid == -1){
         writeWord("Error while creating process.", 1.5, errorColor);
         return;
    }

    
    iterator = 0;
    
}

uint8_t addToList(processStruct *process) {
    
    if(cant_process == MAX_PROCESSES){
        return 1;
    }

    int index = find_place();
    if(index == -1){
        return 2;
    }
    process_list[index].process = process;
    process_list[index].state = AVAILABLE;
    process_list[index].quantum_left = process->priority * VAR;
    cant_process++;

    return 0;
} 

int find_place(){
    for(int i = 0; i < MAX_PROCESSES; i++){
        if(process_list[i].state == EMPTY){
            return i;
        }
    }
    
    return -1;
}

uint64_t scheduler(uint64_t sp){
  
    
    if(cant_process == 0){
         writeWord("There are no processes", 1.5, errorColor);
         return -1;
    }
    
    if(first_time){
        first_time--;
    }
    else{
        process_list[iterator].process->sp = sp;
    }


    next_process();

    int i;
    for(i=0;i<MAX_PROCESSES;i++){
        if(sleeping_list[i].pid != -1){
            if(sleeping_list[i].tick_left >0){
                sleeping_list[i].tick_left--;
                //writeWord("p", 1.5, errorColor);
            }
            else{
                int aux = sleeping_list[i].pid;
                sleeping_list[i].pid = -1;
                int index = find_process(aux);
                process_list[index].state = AVAILABLE;
            }
        }
    }
    
   /*if(iterator == 0){
       writeChar('0', 1.5, errorColor);
   }
   else{
       writeChar('1',1.5,errorColor);
   }*/

   uint64_t rsp = process_list[iterator].process->sp;
    return rsp;
}

uint64_t currentPid(){
    return process_list[iterator].process->pid;
}

int process_is_available(processState p){
    return p.state == AVAILABLE;
}

void next_process(){
    
    if(process_list[iterator].state == AVAILABLE && process_list[iterator].quantum_left >0){
        process_list[iterator].quantum_left--;
        
        return;
    }
    process_list[iterator].quantum_left = process_list[iterator].process->priority * VAR;
    iterator++;
    int i;

    for(i = 0 ; i < MAX_PROCESSES && !process_is_available(process_list[iterator%MAX_PROCESSES]) ; i++){
        iterator++;
    }
    if( i == MAX_PROCESSES && !process_is_available(process_list[iterator%MAX_PROCESSES])){
        iterator = 0; //retorno el proceso shell
         
        return;
    }
    iterator = iterator % MAX_PROCESSES;
    
    return;
}


int kill(uint64_t pid){
   
    if(pid == shell_pid){
        writeWord("You cannot kill the shell process", 1.5, errorColor);
         return 1;
    }
    int processIndex = find_process(pid);
    if(processIndex == -1){
         writeWord("The process does not exist", 1.5, errorColor);
         return 2;
    }
    process_list[processIndex].state = EMPTY;
    cant_process--;
    killProcess(process_list[processIndex].process);
    if(process_list[iterator].process->pid == pid){
        timer_interruption();
    }
    return 0;


}

int changePriority(uint64_t pid, uint8_t newPriority ){
    int process = find_process(pid);
    if(process == -1){
        writeWord("THat process does not exist", 1.5, errorColor);
        return -1;
    }
    if(newPriority<=0 || newPriority > PRIORITY_MAX){
        writeWord("That priority does not exist", 1.5, errorColor);
        return -1;
    }
    process_list[process].process->priority = newPriority;
    return 0;

}
int hola(){};

int blockProcess(uint64_t pid){
    int process = find_process(pid);
    if(process == -1){
        writeWord("Error while blocking the process.", 1.5, errorColor);
        return -1;
    }

    process_list[process].state = BLOCKED;
    if(process_list[iterator].process->pid == pid){
        hola();
        timer_interruption();
    }
    return 0;
}

int sleepProcess(uint64_t sec){
    int i=0;
    while(sleeping_list[i].pid != -1 && i<MAX_PROCESSES){
        i++;
    }
    if(i == MAX_PROCESSES){
        writeWord("ERROR", 1.5, errorColor);
        return 1;
    }
    sleeping_list[i].pid = process_list[iterator].process->pid;
    sleeping_list[i].tick_left = sec * 18;
    
    blockProcess(process_list[iterator].process->pid);
}

int find_process(uint64_t pid){
    int found = 0;
    int i = 0;
    while(i < MAX_PROCESSES && !found){
        if(process_list[i].process->pid == pid && process_list[i].state != EMPTY ){
            found = 1;
        }
        else {
            i++;
        }
    }
    return found? i : -1;
}

void print_processes(){
    int i;
    for(i = 0; i < MAX_PROCESSES; i++){
        if(process_list[i].state != EMPTY){
            writeWord("Process Name: ", 1.5, color);
            char ** name = process_list[i].process->argv;
            writeWord(name[0], 1.5, color);
            newLine();
            writeWord("Process PID: ", 1.5, color);
            char num[10] = {0};
            numToChar(process_list[i].process->pid, num);
            writeWord(num, 1.5, color);
            newLine();
            writeWord("Process Priority: ", 1.5, color);
            char num3[10] = {0};
            numToChar(process_list[i].process->priority, num3);
            writeWord(num3, 1.5, color);
            newLine();

            writeWord("State: ", 1.5, color);
            writeWord(state_to_string(process_list[i].state), 1.5, color);
            newLine();
            writeWord("Process BP: ", 1.5, color);
            char num2[30] = {0};
            numToChar(process_list[i].process->bp, num2);
            writeWord(num2, 1.5, color);
            newLine();
            writeWord("Process SP: ", 1.5, color);
            char num4[30] = {0};
            numToChar(process_list[i].process->sp, num4);
            writeWord(num4, 1.5, color);

            newLine();
            newLine();
            
        }
    }
}

char * state_to_string(uint8_t state){
    switch (state){
        case HALT:
            return "HALT";
        case AVAILABLE:
            return "AVAILABLE";
        case BLOCKED:
            return "BLOCKED";
        case EMPTY:
            return "EMPTY";
        default:
            return NULL;
    }
}

int switch_state( int pid ){
    int processIndex = find_process(pid);
    if(processIndex == -1){
        writeWord("Error. The PID given is unknown\n", 1.5, errorColor);
        return 1;
    } 
    else{
        if( process_list[processIndex].state == BLOCKED ){
            process_list[processIndex].state = AVAILABLE;
            return 0;
        }

        blockProcess(pid);
        return 0;
    }
}

int make_available(int pid){
    int processIndex = find_process(pid);
    if(processIndex == -1){
        writeWord("Error. The PID given is unknown\n", 1.5, errorColor);
        return 1;
    } 
    else{
       process_list[processIndex].state = AVAILABLE;
       return 0;
    }
}