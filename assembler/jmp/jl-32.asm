; Copyright (C) Rong Tao @Sylincom Beijing, 2021年 02月 05日 星期五 08:47:08 CST. 
global _start 
section .text

_start:
	mov ecx, 99	; set ecx to 99
	mov ebx, 42 ; exit status os 42
	mov eax, 1	; sys_exit
	cmp ecx, 100; compare ecx to 100
	jl skip		; jump if less than
	mov ebx, 13	; exit status is 13
skip:
	int 0x80	; irq
