#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define public __attribute__((visibility("default")))
#define private static
#define packed __attribute__((__packed__))
#define unused __attribute__((__unused__))
#define MaxWords ((1024*1024*1024/4)-1)

#define ErrHeadPrepFail 3
#define ErrDoubleFree 2
#define ErrNoMem 1
#define NoErr 0

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

private bool nxtHeaderPrep(word, header*);
public bool destroy(void*);
private void show_(header*);
private header* findBlock_(header*, word, word);
private void* mkalloc(word, header*);
private void* own_malloc(uint32_t); // no of bytes
int main(int, char**);
