[BITS 64]
[ORG 0x7C00]

__bootloader_main:
	mov ah, 0x0E
	mov al, 'H'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'e'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'l'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'l'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'o'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, ','
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, ' '
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'w'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'o'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'r'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'l'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 'd'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, '!'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 0xE ; '\r'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	mov ah, 0x0E
	mov al, 0xA ; '\n'
	mov bh, 0x01
	mov bl, 0
	int 0x10
	loop:
		jmp loop

times 510-($-$$) db 0
dw 0xAA55