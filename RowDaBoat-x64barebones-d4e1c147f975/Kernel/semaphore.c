#include <sem.h>
#include <stddef.h>
#include <video_driver.h>
#include <lib.h>
#include <proces.h>
#include <scheduler.h>


static int check_name(char * name);

static int find_place();

sem_struct * sem_list[MAX_SEM] = {NULL};
int errorColor[3] = {0, 0, 255};

sem_struct * sem_open(char * name, int value){
  
    if(name == NULL){
        writeWord("You must provide a name", 1.5, errorColor);
        return NULL;
    }

    int index = find_sem(name);
    if(index == -1){
        index = find_place();
        if(index == -1){
            writeWord("No more space for semaphore", 1.5, errorColor);
            return NULL; 
        }
        sem_struct * newSem = (sem_struct *) malloc(sizeof(sem_struct));
        memcpy(newSem->name, name, MAX_NAME);
        newSem->value = value;
        newSem->lock = 0;
        newSem->cant_process =1;
        newSem->first_blocked_process = NULL;
        sem_list[index] = newSem;
         
        return sem_list[index];
    }else{
        
        sem_list[index]->cant_process++;
        return sem_list[index];
    }
}

int sem_close(char * name){
  
    int index = check_name(name);
  
    if(index == -1){
        return -1;
    }

    acquire(&(sem_list[index]->lock));
    if(sem_list[index]->cant_process > 1){
        sem_list[index]->cant_process--;
    }else{
        free(sem_list[index]);
        sem_list[index] = NULL;
    }
    release(&(sem_list[index]->lock));

    return 0;
}



int sem_post(char * name){
    int index = check_name(name);
    if(index == -1){
        return -1;
    }
    
    acquire(&(sem_list[index]->lock));
    if(sem_list[index]->first_blocked_process == NULL){
        if(sem_list[index]->value > 1){
            char num[10];
             numToChar(sem_list[index]->value, num);
             writeWord(num, 1.5, errorColor);
            newLine();
        }
        sem_list[index]->value++;
    }
    else{
        
        block_queue * blocked_process = sem_list[index]->first_blocked_process;
        if(find_process(blocked_process->pid) != -1){
            make_available(blocked_process->pid);
            
            
    
            //tendria que estar bloqueado entonces cambia a available
        }
        sem_list[index]->first_blocked_process = sem_list[index]->first_blocked_process->next;
        free(blocked_process);
        sem_list[index]->value++;
                
       
    }
    release(&(sem_list[index]->lock));
    
  //  writeWord("Termine post", 1.5, errorColor);
    return 0;
}



int sem_wait(char * name){
    int index = check_name(name);
    if(index == -1){
        return -1;
    }



    acquire(&(sem_list[index]->lock));
  
    
    if(sem_list[index]->value < 1){
        int pid = currentPid();
        
        block_queue * blocked_process = (block_queue *) malloc(sizeof(block_queue));
        blocked_process->pid = pid;
        blocked_process->next = NULL;
        if(sem_list[index]->first_blocked_process == NULL){
            sem_list[index]->first_blocked_process = blocked_process;
        }else{
            block_queue * temp = sem_list[index]->first_blocked_process;
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = blocked_process;
        } 
        sem_list[index]->value--;
        release(&(sem_list[index]->lock));
        blockProcess(pid);
    }
    else{
        sem_list[index]->value--;
        release(&(sem_list[index]->lock));
    }
    
    return 0;
}


int find_place(){
    for(int i=0;i<MAX_SEM;i++){
        if(sem_list[i] == NULL){
            return i;
        }
    }
    return -1;
}

int find_sem(char * name){
   
    for(int i=0;i<MAX_SEM; i++){
        if(sem_list[i]!= NULL){
            if(strComp(sem_list[i]->name, name) == 0){
                return i;
            }
        }
    }
    return -1;
}

int check_name(char * name){
    if(name == NULL){
        writeWord("You must provide a name", 1.5, errorColor);
        return -1;
    }
    int index = find_sem(name);
    if(index == -1){
        writeWord("Semaphore does not exist", 1.5, errorColor);
        return -1; 
    }
    return index;
}

void print_sem(){
    int color[3] = { 255, 255, 255};
    int flag = 0;
    for(int i = 0; i < MAX_SEM; i++){
        if(sem_list[i] != NULL){
            flag = 1;
            writeWord("Semaphore name: ", 1.5, color);
            writeWord(sem_list[i]->name, 1.5, color);
            newLine();

            writeWord("Semaphore value: ", 1.5, color);
            char aux[50];
            numToChar(sem_list[i]->value,aux);
            writeWord(aux, 1.5, color);
            newLine();

            char aux2[50];
            writeWord("Number of proces linked: ", 1.5, color);
            numToChar(sem_list[i]->cant_process,aux2);
            writeWord(aux2, 1.5, color);
            newLine();

            writeWord("Semaphore state: ", 1.5, color);
            writeWord( (sem_list[i]->lock == 0)? "unlocked" : "locked", 1.5, color);
            newLine();
            
            block_queue * temp = sem_list[i]->first_blocked_process;
            writeWord("Proces blocked: ", 1.5, color);
            newLine();
            if(temp == NULL){
                writeWord("0 proces blocked", 1.5, color);
                newLine();
            }
            while (temp != NULL)
            {   writeWord("    - Proces PID = ", 1.5, color);
                uint64_t pid = temp->pid;
                char num[20];
                numToChar(pid, num);
                writeWord(num, 1.5, color);
                newLine();
                temp = temp->next;
            }
            

        }
    }
    
    if(flag == 0){
        writeWord("no sem open", 1.5, color);
        newLine();
    }
    newLine();
}