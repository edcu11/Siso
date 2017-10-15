[BITS 16]
[ORG 0x0000]

mov ax, cs
mov ds, ax

mainloop:
	call MainMenu
	jmp mainloop

menu db 'Menu: 1. Bienvenida 2. Ordenar 3. Es par?', 0x0D, 0x0A, 0
error_input db 'Escriba un valor correcto!', 0x0D, 0x0A, 0
fallo db 'Error al leer sector', 0x0D, 0x0A, 0

print_string:
	lodsb        ; grab a byte from SI

	or al, al  ; logical or AL by itself
	jz .done   ; if the result is zero, get out

	mov ah, 0x0E
	int 0x10      ; otherwise, print out the character!

	jmp print_string

	.done:
		ret



PrintCharacter:
	mov ah, 0x0E
	int 0x10
	ret

GetCharacter:
	mov ah, 0
	int 0x16
	ret

Println:
	mov al, 0	; null terminator '\0'
	stosb       ; Store string

	;Adds a newline break '\n'
	mov ah, 0x0E
	mov al, 0x0D
	int 0x10
	mov al, 0x0A
	int 0x10
	ret

MainMenu:
	push bp
	call Println
	call Println
	call Println

	mov si, menu
	call print_string

	call GetCharacter
	mov bl, al
	call PrintCharacter
	sub bl, 48 ;convertimos el input a decimal

	mov ah, 0x02
	mov al, 1
	mov dl, 0x00
	mov ch, 0
	mov dh, 0

	cmp bl, 1
	jz callProgram1

	cmp bl, 2
	jz callProgram2

	cmp bl, 3
	jz callProgram3

	;fallo al ingresar valor valido entonces avisamos
	mov si, error_input
	call print_string
	call Println

	jmp MainMenu	;empezamos de nuevo

callProgram1:

	mov cl, 3
	mov bx, 0x8000
	int 0x13

	call 0x8000
	jmp MainMenu


callProgram2:
	mov cl, 4
	mov bx, 0x8200
	int 0x13

	call 0x8200
	jmp MainMenu


callProgram3:
	mov cl, 5
	mov bx, 0x8400
	int 0x13

	call 0x8400
	jmp MainMenu



times (512-($-$$)) db 0
