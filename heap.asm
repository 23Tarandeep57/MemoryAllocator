bits 32
global memspace
%define HeapSize (1024*1024*1024/4)

section .data alloc noexec write progbits 
	memspace:
		dd _memspace

section .heap alloc noexec nobits write
	_memspace:
		heap_size equ HeapSize
		resd heap_size

Section .note.GNU-stack noalloc
