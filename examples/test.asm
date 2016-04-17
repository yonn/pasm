section .data
	hello: str "Hello, world!"
	x: dd 0

section .text

label:
again:  mov eax, 2
	mov ebx, 1 ; this is a comment
	add eax, ebx
	jmp yet_again

yet_again:
	mov [bx], 0xFF
	mov eax, 0
	int 0x80
