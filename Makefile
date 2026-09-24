flags=-O2 -Wall -std=c23

.PHONY: clean 
all: clean my_malloc

my_malloc: my_malloc.o heap.o 
	cc $(flags) $^ -o $@ $(ldflags)

my_malloc.o: my_malloc.c my_malloc.h
	cc $(flags) -c $<

heap.o: heap.asm
	nasm -f elf64 $^

clean:
	rm -f *.o my_malloc
