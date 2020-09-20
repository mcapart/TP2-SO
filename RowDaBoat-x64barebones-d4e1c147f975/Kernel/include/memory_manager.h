#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define START_MM 0x600000
#define MEM_SIZE 10*1024*1024 //10 MB
#define BLOCK_SIZE 1024 
#define BITMAP_SIZE (MEM_SIZE/BLOCK_SIZE)

void * malloc(UINT64_t);
void free(void * add);





#endif