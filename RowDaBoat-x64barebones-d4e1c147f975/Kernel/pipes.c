#include <pipes.h>

fd_struct * fd_list[MAX_FD] = {NULL};
static int errorColor[3] = {0, 0, 255};
static int color[3] = {255, 255, 255};

int find_fd(char * name);

int find_fd_place();


fd_struct * addFD(char * name, int isPipe){
     if(name == NULL){
        writeWord("Name cannot be NULL", 1.5, errorColor);
    }
    int index = find_fd_place();
    if (index == -1)
        writeWord("There is no more space for fd", 1.5, errorColor);
    
    fd_struct * newFD = (fd_struct *) malloc(sizeof( fd_struct));
    char nameR[MAX_LENGTH_NAME_FD] = {0};
    char nameW[MAX_LENGTH_NAME_FD] = {0};
    nameR[0] = 'R';
    nameW[0] = 'W';
    memcpy(&(nameR[1]), name, strLen(name));
    memcpy(&(nameW[1]), name, strLen(name));
    newFD->sem_r = sem_open(nameR, 0);
    newFD->sem_w = sem_open(nameW, MAX_LENGTH_BUFFER_FD);
    newFD->process_counter = 1;
    memcpy(newFD->name, name, MAX_LENGTH_NAME_FD);
    newFD->isPipe = isPipe;
    newFD->fd = index;
    newFD->index_w = 0;
    newFD->index_r = 0;
    fd_list[index] = newFD;
    
    return fd_list[index];

}


void fd_init(){

    fd_struct * fd = addFD("STDIN", 0);
    fd_struct * fd2 = addFD("STDOUT", 0);
    fd_struct * fd3 = addFD("STDERR", 0);
    if(fd == NULL || fd2 == NULL || fd3 == NULL){
        writeWord("Erors while initializing pipes", 1.5, errorColor);
        newLine();
        return;
    }
  
}



int pipe(char * name){
     if(name == NULL){
        return -1;
    }
    int index = find_fd(name);
    if (index == -1){
        fd_struct * newFD;
        newFD = addFD(name, 1);
        if(newFD == NULL){
            return -1;
        }
        int err = add_fd_process(newFD->fd, currentProcess());
        if(err == -1){
            return -1;
        }
        int fd = newFD->fd;
        return fd;
    }
    else{
        int err = add_fd_process(index, currentProcess());
        if(err == -1){
            return -1;
        }
           fd_list[index]->process_counter++;
        return index;
    }
 
}

int closeFD(int fd){
    if(fd < 0 || fd >= MAX_FD || fd_list[fd] == NULL){
        return -1;
    }
    remove_fd_process(fd);
    fd_list[fd]->process_counter--;
    if(fd_list[fd]->process_counter > 0){
        return 0;
    }
    free(fd_list[fd]);
    fd_list[fd] = NULL;
    return 0;
}

int write_fd(int fd, const char * str, int length){
    if(length == 0){
        return 0; 
    }
    if(length <0 || str == NULL){
        return -1;
    }

    int realFD = getRealFD(fd);
    if(realFD == -1 || fd_list[realFD] == NULL){
        return -1;
    }
    int j = 0;
    int count = length;
    int null_char = 0;
    int index_buff;
    switch(realFD){
        case STDOUT:
            while(length > 0){
                if(*str != '\0')
                    writeChar(*str, 1.5, color);
                length--;
                str++;
            }
            return count - length;
            
        case STDERR:
            while(length > 0){
                if(*str != 0){
                    writeChar(*str, 1.5, errorColor);
                }
                str++;
                length--;
            }
            return count - length; 
        default: 
            index_buff = fd_list[realFD]->index_w;
            null_char = 0;
            while(!null_char && j<length){
                sem_wait(fd_list[realFD]->sem_w->name);
                fd_list[realFD]->buffer[index_buff%MAX_LENGTH_BUFFER_FD] = str[j];

                if(str[j] == 0){
                    null_char = 1;
                }
                j++;
                index_buff++;
              
                sem_post(fd_list[realFD]->sem_r->name);
            }
            fd_list[realFD]->index_w = index_buff % MAX_LENGTH_BUFFER_FD;
            return j;
    }
    
}  

void print_pipes(){
    int i;
    for(i = 0; i < MAX_FD; i++){
        if(fd_list[i] != NULL && fd_list[i]->isPipe == 1){
            writeWord("Pipe name: ", 1.5, color);
            writeWord(fd_list[i]->name, 1.5, color);
            newLine();
            writeWord("Linked processes: ", 1.5, color);
            char num[20];
            numToChar(fd_list[i]->process_counter, num);
            writeWord(num, 1.5, color);
            newLine();
            writeWord("Blocked by sem_read: ", 1.5, color);
            block_queue * temp = fd_list[i]->sem_r->first_blocked_process;
            int j = 1;
            if(temp == NULL){
                writeWord("0 proces blocked", 1.5, color);
                newLine();
            }else{
                newLine();
            }
            while(temp != NULL){
                writeWord("     PID: ", 1.5, color);
                char num2[20];
                numToChar( temp->pid, num2);
                writeWord(num2, 1.5, color);
                newLine();
                temp = temp->next;
                j++;
            }
            writeWord("Blocked by sem_write: ", 1.5, color);
            temp = fd_list[i]->sem_w->first_blocked_process;
            if(temp == NULL){
                writeWord("0 proces blocked", 1.5, color);
                newLine();
            }else{
                newLine();
            }
            while(temp != NULL){
                writeWord("     PID: ", 1.5, color);
                char num3[20];
                numToChar( temp->pid, num3);
                writeWord(num3, 1.5, color);
                newLine();
                temp = temp->next;
                
            }
            newLine();
        }
    }
}


int read_fd(int fd,  char * str, int length){
    if(length == 0){
        return 0; 
    }
    if(length <0 ){
        return -1;
    }

    int realFD = getRealFD(fd);
    if(realFD == -1 || fd_list[realFD] == NULL){
        return -1;
    }
    int j = 0;
    int count = length;
    int null_char = 0;
    int index_buff;
    switch(realFD){
        case STDIN:
            while(count > 0 && !null_char){
                getNChar(str, 1);
                if(*str == '\0')
                    null_char = 1;
                str++;
                count--;
            }
                
            return length - count;
        default:
            index_buff = fd_list[realFD]->index_r;
            while(!null_char && j< length){
                sem_wait(fd_list[realFD]->sem_r->name);
                str[j] = fd_list[realFD]->buffer[index_buff % MAX_LENGTH_BUFFER_FD];
                if(str[j] == 0){
                    null_char = 1;
                }
                j++;
                index_buff++;
              
                sem_post(fd_list[realFD]->sem_w->name);
            }
            fd_list[realFD]->index_r = index_buff % MAX_LENGTH_BUFFER_FD;
            return j;
    }
    
}  


int find_fd_place(){
    int i;
    for(i = 0; i < MAX_FD; i++){
        if(fd_list[i] == NULL){
            return i;
        }
    }
    return -1;
}

int find_fd( char* name){
    int i;
    for(i = 0; i < MAX_FD; i++){
        if(strComp(fd_list[i]->name, name) == 0){
            return i;
        }
    }
    return -1;
}


