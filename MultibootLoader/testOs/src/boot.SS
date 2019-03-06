#include <config.h>

.text

.global _start
_start:	
	jmp multibootInit

.align 4
	multibootHeader:
	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_HEADER_FLAGS
	.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

multibootInit:
	movl $(stack + STACK_SIZE), %esp
	pushl $0
	popf
	
	pushl %ebx
	pushl %eax
	
	call main

	loop: hlt
	jmp loop
	
	.comm stack, STACK_SIZE
