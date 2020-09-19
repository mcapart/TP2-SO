#include <stdint.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}


static void reverse(char * c, int l){
    int start = 0; 
    int end = l -1; 
    while (start < end) 
    { 
        char aux = c[start];
        c[start] = c [end];
        c[end] = aux;
        start++; 
        end--; 
    } 
}

void numToChar(double num, char * c){
    int entera = num;
    double decimal = num - entera;
    int i = 0;
    if(num < 0){
        entera = entera * -1;
        decimal = decimal * -1;
    }
    if(entera == 0){
        c[i] = '0';
        i++;
    }
    for(; i < 99 && entera > 0; i++){
        int valor = entera % 10;
        c[i] = valor + '0';
        entera = entera/10;
    }
    if(num < 0){
        c[i] = '-';
        i++;
    }
    reverse(c,i);
    if(decimal != 0){
        c[i] = '.';
      	i++;
        for(int j = 0;i < 100 && j < 4; i++, j++){
            int aux = decimal * 10;
            c[i] = (aux % 10) + '0';
            decimal = decimal * 10;
        }
    }
    c[i] = 0;
}



