include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS=$(SOURCES:.c=.o)


all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM)  
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c  $(OBJECTS) $(OBJECTS_ASM)  -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./Include -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(SOURCES) $(OBJECTS_ASM)  -o ../0000-sampleCodeModule.elf

%.o: %.c
	$(GCC) $(GCCFLAGS)  -I./Include  -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@


clean:
	rm -rf *.o

.PHONY: all clean print
