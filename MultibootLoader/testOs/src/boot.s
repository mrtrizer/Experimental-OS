# 1 "boot.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "boot.S"
# 1 "./config.h" 1
# 2 "boot.S" 2

.text

.global _start
_start:
 jmp multibootInit

.align 4
 multibootHeader:
 .long 0x1BADB002
 .long 0x00000001
 .long -(0x1BADB002 + 0x00000001)

multibootInit:
 movl $(stack + 0x400), %esp
 pushl $0
 popf

 pushl %ebx
 pushl %eax

 call main

 loop: hlt
 jmp loop

 .comm stack, 0x400
