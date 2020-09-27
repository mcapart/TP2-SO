
#include "memory_manager.h"
#include <stdint.h> 
#include <stddef.h>
#include <video_driver.h>
#include "lib.h"

typedef struct{
    int occupied; // 0 libre 1 ocupado
    uint64_t blocks; //bloques que ocupa
}block;

block bitmap[BITMAP_SIZE]; 
uint64_t usedMem = 0;


void * malloc(uint64_t n){
   
    void *result = NULL;
    if(MEM_SIZE - usedMem < n){
        return result;
    }
    int blocksNeeded = n/BLOCK_SIZE + 1; 
    uint64_t i =0;
    for(int count = 0;i<BITMAP_SIZE && count < blocksNeeded;i++){
        if(!bitmap[i].occupied){
             
            count++;
        }
        else{
            count = 0;
        }
        
    }
    if(i == BITMAP_SIZE){
       
        return result;
    }

    i -= blocksNeeded;
    for(int j = 0; j<blocksNeeded; j++){
        bitmap[i+j].occupied = 1;
        bitmap[i+j].blocks = blocksNeeded;
    }
    result = (void *) (START_MM + (i * BLOCK_SIZE));
    usedMem += n;

    return result;
}

void free( void * add){
    int pos = ((uint64_t)add - START_MM)/BLOCK_SIZE;
    int blocksNeeded = bitmap[pos].blocks;
    for(int i =0;i<blocksNeeded;i++){
        bitmap[pos + i].occupied = 0;
    } 
}








