#include "my_malloc.h"

extern heap *memspace;

header* findBlock_(header* hdr, word allocation, word n) {
	if (n+allocation > (MaxWords-2))
		reterr(ErrNoMem);
	bool ok;
	void* mem;
	header* hdr_;
	word n_;

	ok = (!(hdr->w))?true:
		(!(hdr->allocated) && hdr->w >= allocation) ? true : 
		false;

	if (ok) 
		return hdr;

	mem = (void*)hdr + 4 + hdr->w*4;
	hdr_ = (header*) mem;
	n_ = n + hdr->w;
	return findBlock_(hdr_, allocation, n_);
}

void* mkalloc(word words, header *hdr) {
	void *ret, *bytesin;
	word wordsin;

	bytesin = (void*) (((void*)hdr) - memspace);
	wordsin = ((word)bytesin/4) + 1;
	
	if (words > (MaxWords - wordsin))
		reterr(ErrNoMem);

	hdr->w = words;
	hdr->allocated = true;
	ret = (void*)hdr + 4;
	return ret;
}

void* own_malloc(uint32_t size) {
	word words;
	header *hdr;
	void* allocated_memory;

	words = (!(size%4)) ? size/4 : size/4+1;

	hdr = findBlock(words);
	if (!hdr)
		return (void*) 0;
	
	if (words > MaxWords)
		reterr(ErrNoMem);

	allocated_memory = mkalloc(words, hdr);
	if (!allocated_memory) 
		return (void*)0;

	return allocated_memory;
}

void show_(header* hdr) {
	header* p;
	uint32_t n;
	void* mem;
	for(n=1, p=hdr; p->w; mem=(void*)p+(p->w+1)*4, p=mem, n++) 
		printf("Allocation : %d = %d %s words\n",
		 n, p->w, (p->allocated) ? "Allocated":"free"
		);

}

int main(int argc, char* argv[]) {
	uint8_t* p1, *p2, *p3;
	p1 = own_malloc(7);
	p2 = own_malloc(200);
	p3 = own_malloc(20);
	printf("Memspace = %p\n", (void*)memspace);
	printf("Allocated1 = %p\n", (void *)p1);
	printf("ALlocated2= %p\n", (void*)p2);
	printf("Allocated3 = %p\n", (void*)p3);
	show();
	return 0;
}
