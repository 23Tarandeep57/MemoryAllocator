#include "my_malloc.h"

extern heap *memspace;

private void coalesence_(header* hdr) {
	header *p0, *p1;
	p0 = hdr;
	void* mem;
	uint32_t n;
	p1 = p0;
	while((p0->w) && (p1->w)) {
		printf("p0 -> %p\n", (void*)p0);
		printf("p1 -> %p\n", (void*)p1);
		if (p0->allocated) {
			mem = p0;
			p0 = (header*) (mem + 4 + (p0->w)*4);
			continue;
		}
		mem = p0;
		p1 = (header*)(mem + 4 + (p0->w) * 4);
		if (p1->allocated) {
			mem = p1;
			p0 = (header*)(mem + 4 + (p1->w)*4);
			continue;
		}
		mem = p1;
		n = p1->w;
		p0->w = p0->w + n + 1;
		p1 = (header*)(mem + 4 + n*4);
		memset(mem, 0, 4);
	}
}

public bool destroy(void* addr) {
	header* p;

	p = (header*)addr - 1;
	//printf("Destroed addr header: %p\n", p);

	if (!(p->w) || !(p->allocated)){
		reterr(ErrDoubleFree);
	} 
	
	uint32_t n;
	n = p->w * 4;
	memset(addr, 0, n);
	p->allocated=false;
	coalesence();

	return true;
}

private header* findBlock_(header* hdr, word allocation, word n) {
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

	mem = ((void*)hdr + 4) + hdr->w*4;
	hdr_ = (header*) mem;
	n_ = n + hdr->w;
	return findBlock_(hdr_, allocation, n_);
}

private bool nxtHeaderPrep(word words, header * hdr) {
	if (!(hdr->w) || hdr->w==words) return true;
	if (words > hdr->w) reterr(ErrNoMem);
	word xtra_mem = hdr->w - words;
	header* nhdr = (header*) (((void*)hdr + 4) + words*4 );
	nhdr->w = xtra_mem - 1;
	nhdr->allocated = false;
	return true;
}

private void* mkalloc(word words, header *hdr) {
	void *ret, *bytesin;
	word wordsin;

	bytesin = (void*) (((void*)hdr) - memspace);
	wordsin = ((word)bytesin/4) + 1;
	
	if (words > (MaxWords - wordsin))
		reterr(ErrNoMem);
	
	bool headprep = nxtHeaderPrep(words, hdr);
	if (!headprep) reterr(ErrHeadPrepFail); 

	hdr->w = words;
	hdr->allocated = true;
	ret = (void*)hdr + 4;
	return ret;
}

public void* own_malloc(uint32_t size) {
	word words;
	header *hdr;
	void* allocated_memory;

	words = (!(size%4)) ? size/4 : size/4+1;

	hdr = findBlock(words);
	//printf("Header : %p\n", (void*)hdr);
	if (!hdr)
		return (void*) 0;
	
	if (words > MaxWords)
		reterr(ErrNoMem);

	allocated_memory = mkalloc(words, hdr);
	if (!allocated_memory) 
		return (void*)0;

	return allocated_memory;
}

private void show_(header* hdr) {
	header* p;
	uint32_t n;
	void* mem;
	for(n=1, p=hdr; p->w; mem=(void*)p+(p->w+1)*4, p=mem, n++) 
		printf("Allocation : %d = %d %s words\n",
		 n, p->w, (p->allocated) ? "Allocated":"free"
		);

	//printf("final addr: %p\n", (void*)p);
}

int main(int argc, char* argv[]) {
	uint8_t* p1, *p2, *p3, *p4;
	bool ret1, ret2;
	p1 = own_malloc(7);
	p2 = own_malloc(200);
	p3 = own_malloc(20);
	ret1 = destroy(p2);
	ret2 = destroy(p3);
	p4 = own_malloc(210);
	printf("Bool1: %s\n", ret1?"true":"false");
	printf("Bool2: %s\n", ret2?"true":"false");
	printf("Memspace = %p\n", (void*)memspace);
	printf("Allocated1 = %p\n", (void *)p1);
	printf("ALlocated2= %p\n", (void*)p2);
	printf("Allocated3 = %p\n", (void*)p3);
	printf("Allocated4 = %p\n", (void*)p4);
	show();
	return 0;
}
