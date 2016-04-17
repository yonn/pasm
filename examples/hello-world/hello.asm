section .data
	hello: str "Hello, world!"
	size: dd 13

section .text
;global _start
_start:
	mov edx, [size]
	mov ecx, hello
	mov ebx, 1
	mov eax, 4
	int 0x80

	mov eax, 1
	int 0x80
