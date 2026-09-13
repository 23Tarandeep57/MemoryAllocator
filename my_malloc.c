#include <unistd.h>
#include <stdio.h>
#include <string.h>

void* heap_memory = NULL;

void* own_malloc(size_t size) {
	void* allocated_memory = sbrk(size);
	
	if (heap_memory == NULL) 
		heap_memory = allocated_memory;

	return allocated_memory;
}

int own_free_all() {
	if (heap_memory == NULL) 
		return -1;
	return brk(heap_memory);
}

int main() {
	char* str = "wubba lubba dub dub!!";
	size_t size = strlen(str) * sizeof(char);
	char* mem = (char*)own_malloc(size);
	strcpy(mem, str);
	own_free_all();
	return 0;
}
