[BITS 16]
[ORG 0x0000]

mov ax, cs 
mov ds, ax

Boot2:
    ; Prints the character S to the screen
    mov al, 0x53
    mov ah, 0x09
    mov bh, 0x00
    mov bl, 0x0F
    mov cx, 0x01
    int 0x10
    jmp $ ; Loop forever

TIMES 512 - ($ - $$) db 0 ; Fill rest of block