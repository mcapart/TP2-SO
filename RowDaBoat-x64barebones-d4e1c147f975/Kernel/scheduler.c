#include <scheduler.h>

int iterator;
int cant_process;
int first_time = 1;
int shell_pid;
int func_pid;

int static errorColor[3] = {0, 0, 255};
int static color[3]= {255, 255, 255};

static processState process_list[MAX_PROCESSES];

void schedulerInitializer(){
    for(int i=0;i<MAX_PROCESSES;i++){
        process_list[i].state = EMPTY;
    }
    cant_process = 0;

    shell_pid = create_proces(0x400000, 0, (uint64_t) "sampleCodeModule");
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
   
    return process_list[iterator].process->sp;
}

uint64_t currentPid(){
    return process_list[iterator].process->pid;
}

int process_is_available(processState p){
    return p.state == AVAILABLE;
}

void next_process(){
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



int blockProcess(uint64_t pid){
    int process = find_process(pid);
    if(process == -1){
        writeWord("Error while blocking the process.", 1.5, errorColor);
        return -1;
    }

    process_list[process].state = BLOCKED;
    if(process_list[iterator].process->pid == pid)
        timer_interruption();
    return 0;
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
            writeWord("Process PID: ", 1.5, color);
            char num[10] = {0};
            numToChar(process_list[i].process->pid, num);
            writeWord(num, 1.5, color);
            newLine();
            writeWord("State: ", 1.5, color);
            writeWord(state_to_string(process_list[i].state), 1.5, color);
            newLine();
            writeWord("Process BP: ", 1.5, color);
            char num2[30] = {0};
            numToChar(process_list[i].process->bp, num2);
            writeWord(num2, 1.5, color);
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