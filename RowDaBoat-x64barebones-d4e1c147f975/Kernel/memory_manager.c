#include "memory_manager.h"
#include <stdint.h> 
#include <stddef.h>
#include <video_driver.h>
#include "lib.h"

#ifdef MM_BUDDY

#define FREE 0
#define USED 1
#define PART_USED 2
#define NODES 255 
static uint8_t memory_tree[NODES] = {FREE};
static uint64_t memory_used = 0;

static uint64_t usedMem(){
    return memory_used;
}

void * mallocRec(uint64_t size, unsigned int node, uint64_t current_size, void * address){
    void * resp = NULL;
 
    if(size <= current_size / 2 && (2 * node + 1) < NODES){
        if(memory_tree[2*node+1] == PART_USED){
            resp = mallocRec(size, 2 * node + 1, current_size / 2, address );
        } 
        if(resp == NULL && memory_tree[2 * node + 2] == PART_USED){
            resp = mallocRec(size, 2 * node + 2, current_size/2, address + current_size/2);

        }

        if(resp == NULL){
            if(memory_tree[2*node+1] == FREE){
                resp = mallocRec(size, 2 * node + 1, current_size / 2, address );
            } 
            else if(memory_tree[2 * node + 2] == FREE){
                resp = mallocRec(size, 2 * node + 2, current_size/2, address + current_size/2);

            }
        }

        if(resp != NULL){
            if(memory_tree[2 * node + 1] == USED && memory_tree[2 * node + 2] == USED){
                memory_tree[node] = USED;
            }
            else if(memory_tree[2 * node + 1] == USED || memory_tree[2 * node + 2] == USED){
                memory_tree[node] = PART_USED;
            }
        }
        return resp;

    }

    if(memory_tree[node] == PART_USED){
        return NULL;
    }
    
    memory_tree[node] = USED;
    memory_used += current_size;
    return address;
    
}

void * malloc(uint64_t size){
   if(memory_tree[0] == USED ||  MEM_SIZE - memory_used < size){
       return NULL;
   }
   return mallocRec(size, 0, MEM_SIZE, START_MM );
}

int find_first_father_node(uint64_t address, int* current_size){
    // le quito el offset a las direcciones para trabajar con numeros más pequeños como en el ejemplo del árbol de la página web
    address -= START_MM;
    int temp_node = 0;
    int temp_size = MEM_SIZE; //comienzo en la altura 0
    uint64_t temp_address = 0;
    while(temp_node < NODES){
        if(address == temp_address){
            *current_size = temp_size;
            return temp_node;
        }
        temp_size /= 2; //bajo un nivel (aumenta la altura del árbol)
        if(address >= temp_address + temp_size){
            temp_address += temp_size;
            temp_node = 2 * temp_node + 2;
        }
        else{
            temp_node = 2 * temp_node + 1;
        }
    }
    // en caso de error retorno -1
    return -1;
}

int find_current(int node, int * current_size){
    // si el nodo que recibo esta libre, todos sus nodos sucesores también lo estaran, por lo que el nodo a eliminar ya estaba libre
    if(memory_tree[node] == FREE){
        return -1;
    }
    // si el nodo izquierdo es mayor que la máxima cantidad de bloques disponibles, ese nodo tiene que ser nodo hoja si o si
    if((2 * node + 1) >= NODES || memory_tree[2 * node + 1] == FREE){
        memory_tree[node] = FREE;
        return node;
    }
    // si no encontre al nodo todavia, bajo un nivel más
    *current_size /= 2;
    return find_current(2 * node + 1, current_size);
}

void free(void * add){
    int node_size;
    int father_node;
    // busco el primer nodo que tenga el mismo address que el address que me pasaron
    father_node = find_first_father_node(add, &node_size);
    // valido que haya encontrado algun nodo con ese address, sino no encontre ningun nodo el address es inválido
    if(father_node < 0){
        return;
    }
    int current_node;
    // busco el nodo que quiero eliminar (puede ser el mismo o sucesor del encontrado anteriormente)
    current_node = find_current(father_node, &node_size);
    // valido que el nodo que quiero eliminar realmente este ocupado y no vacio
    if(current_node < 0){
        return;
    }
    // pongo el estado del nodo en FREE
    memory_tree[current_node] = FREE;
    // actualizo la en cantidad de memoria usada
    memory_used -= node_size;
    int aux_node = current_node;
    // recorro a los nodos predesores y voy actualizando sus estados
    while(aux_node != 0){
        // actualizo al nodo padre, abuelo, etc .. hasta llegar al nodo raiz
        aux_node = (aux_node - 1) / 2;
        if(memory_tree[aux_node] == USED ){
            memory_tree[aux_node] = PART_USED;
        }
        else if(memory_tree[2 * aux_node + 1] == FREE && memory_tree[2 * aux_node + 2] == FREE){
            memory_tree[aux_node] = FREE;
        }
    }
    return;
}

#endif

#ifdef MM_BITMAP 
typedef struct{
    int occupied; // 0 libre 1 ocupado
    uint64_t blocks; //bloques que ocupa
}block;

block bitmap[BITMAP_SIZE]; 
uint64_t memory_used = 0;


void * malloc(uint64_t n){
   
    void *result = NULL;
    if(MEM_SIZE - memory_used < n){
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
    memory_used += BLOCK_SIZE * blocksNeeded;

    return result;
}

void free( void * add){
    int pos = ((uint64_t)add - START_MM)/BLOCK_SIZE;
    int blocksNeeded = bitmap[pos].blocks;
    for(int i =0;i<blocksNeeded;i++){
        bitmap[pos + i].occupied = 0;
        memory_used-=BLOCK_SIZE;
    } 
}

uint64_t usedMem(){
    return memory_used;
}


#endif