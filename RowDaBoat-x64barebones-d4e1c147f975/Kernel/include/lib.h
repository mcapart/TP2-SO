#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

void cpuVersion(uint32_t * result);

void getTemperature(uint64_t * targetTemp, uint64_t * statusTemp);

void saveReg();

void getReg(uint64_t * v);

void saveMemory(uint8_t mem);

void getMemory(uint8_t * v);

void numToChar(double v, char * text);

#endif