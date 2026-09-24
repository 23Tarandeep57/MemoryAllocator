#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#define packed __attribute__((__packed__))
#define unused __attribute__((__unused__))
#define MaxWords ((1024*1024*1024/4)-1)
#define ErrNoMem 1

typedef uint32_t word;
typedef void heap;
struct packed s_header {
	word w:30;
	bool allocated:1;
	bool unused reserved:1;
};
typedef struct packed s_header header;

#define reterr(x) do { errno = (x); return (void *)0; } while (0)
#define findBlock(x) findBlock_((header*)memspace, x, 0)
#define show() show_((header*) memspace) 

void show_(header*);
header* findBlock_(header*, word, word);
void* mkalloc(word, header*);
void* own_malloc(uint32_t); // no of bytes
int main(int, char**);
