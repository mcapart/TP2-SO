#include <stdint.h>
#include <video_driver.h>
#include <keyboardDriver.h> 
#include <lib.h>
#include <memory_manager.h>
#include <scheduler.h>
#include <sem.h>
#include <pipes.h>


extern uint8_t getHou();
extern uint8_t getMin();
extern uint8_t getSec();
extern void setRegCalc(uint64_t rip, uint64_t rbp);
extern void setRegShell(uint64_t rip, uint64_t rbp);
extern void setApp(int i);

void sys_delete(); //rax = 5 => syscall delete
void sys_newLine(); //rax = 6 => syscall newLine
void sys_currentHour(uint64_t * value);
void sys_currentMin(uint64_t * value);
void sys_currentSec(uint64_t * value);
void sys_cpuInfo(char * vendor, uint32_t * version); //rax = 10 => syscall cpuInfo
void sys_getTemperature(uint64_t * targetTemp, uint64_t * statusTemp ); // rax = 11 => syscall temperature
void sys_saveReg(); 
void sys_getReg(uint64_t * v);
void sys_getMem(uint8_t mem, uint8_t * v);
void sys_saveReturn(uint64_t rip, uint64_t rbp, int app);
void sys_sleep(uint64_t rdi);


uint64_t systemCall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,  uint64_t r8, uint64_t rax){
    switch(rax){
        case 1: return (currentProcess()->fg == 1 || getRealFD((int)rdi) != 0 )? read_fd(rdi,(char*) rsi, rdx): 0;
        case 2:  return write_fd(rdi,(char *)rsi, rdx);
        case 3: return (uint64_t) malloc(rdi);
        case 4: free((void *)rdi);
                break;      
        case 5: sys_delete();
                break;
        case 6: sys_newLine();
                break;
        case 7: sys_currentHour((uint64_t *) rdi);
                break;
        case 8: sys_currentMin((uint64_t *) rdi);
                break;
        case 9: sys_currentSec((uint64_t *) rdi);
                break;
        case 10: sys_cpuInfo((char *) rdi, (uint32_t *) rsi);
                break;
        case 11: sys_getTemperature((uint64_t *) rdi, (uint64_t *) rsi);
                break;
        case 12: sys_saveReg();
                break;
        case 13: sys_getReg((uint64_t *) rdi);
                break;
        case 14: sys_getMem((uint8_t) rdi, (uint8_t *) rsi);
                break;
        case 15: sys_saveReturn(rdi, rsi, rdx);
                break;
        case 16: return create_proces(rdi, rsi, (char **)rdx,rcx, r8);
        case 17: return kill(rdi);
        case 18: print_processes();
                break;
        case 19: return switch_state(rdi);  
        case 20: return currentPid();
        case 21: sys_sleep(rdi);
                break;   
        case 22: return changePriority(rdi, rsi); 
        case 23: return usedMem();
        case 24: return (uint64_t) sem_open((char *)rdi, rsi);
        case 25: return sem_close((char *)rdi);
        case 26: return sem_wait((char *)rdi);
        case 27: return sem_post((char *)rdi);
        case 28: print_sem();
                break;
        case 29: return pipe((char *)rdi);
        case 30: return closeFD(rdi);
        case 31: print_pipes();
                break;
        case 32: return dup2(rdi, rsi);
        case 33: giveCpu();
                break;
        case 34: return blockProcess(rdi);
        case 35: return make_available(rdi);

    }
    return 0;
}





void sys_delete(){
    deleteChar();
}

void sys_newLine(){
    newLine();
}

void sys_currentHour(uint64_t * value){
    value[0] = getHou();
}

void sys_currentMin(uint64_t * value){
    value[0] = getMin();
}

void sys_currentSec(uint64_t  * value){
    value[0] = getSec();
}
            
void sys_cpuInfo(char * vendor , uint32_t * version){
    cpuVendor(vendor);
    cpuVersion(version);
}

void sys_getTemperature(uint64_t * targetTemp, uint64_t * statusTemp){
    getTemperature(targetTemp, statusTemp);
}

void sys_saveReg(){
    saveReg();
}


void sys_getReg(uint64_t * v){
  getReg(v);
}

void sys_getMem(uint8_t mem, uint8_t * v){
    saveMemory(mem);
    getMemory(v);
}

void sys_saveReturn(uint64_t rip, uint64_t rbp, int app){
   
    if(app == 0){
        
        setRegCalc(rip, rbp);
    }
    else if(app == 1){
        setRegShell(rip, rbp);
    }
}

void sys_sleep(uint64_t rdi){
    _sti();
    sleepProcess(rdi);
}

