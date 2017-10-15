[BITS 16] ; 16 bit mode
[ORG 0x7C00] ; Boot loader start address

Boot:
    xor ax, ax   ; AX=0
    mov ds, ax   ; DS=0
    mov es, ax   ; ES=0
    add ax, 0x9000
    mov ss, ax
    mov sp, 0xF000  ; Bottom of stack at 0x9000:0xF000
    ; Reset the drive, dl contains drive number
    mov ah, 0x00
    int 0x13
    mov ax, 0x07E0
    mov es, ax ; Load to 0x07E0 : 0x00
    mov bx, 0x00
ReadDrive:
    mov ah, 0x02
    mov al, 0x01 ; Read 1 sector
    mov ch, 0x00 ; Read on cylinder 0
    mov cl, 0x02 ; Read sector 2
    mov dh, 0x00 ; Head number 0
    int 0x13

    jnc Success
    ; Print error (character F)
    mov al, 0x46
    call PrintChar
    jmp ReadDrive ; Retry

Success:
    jmp 0x07E0:0x00 ; Jump to 2nd stage bootloader

PrintChar: ; Prints a single character
    pusha
    mov ah, 0x09
    mov bh, 0x00
    mov bl, 0x0F
    mov cx, 0x01
    int 0x10
    popa
    ret

TIMES 510 - ($ - $$) db 0
DW 0xAA55 ; Boot signature
