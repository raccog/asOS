;
; Print procedures using VGA text mode.
;
; procedures:
;	 sprint
;	 clear
;	 cprint
;	 printreg16
;	 movecursor
;

_dochar:
	call cprint
; prints a string to VGA text memory
; inputs: si = start of string
; modifies: ypos += 1, xpos = 0
; does not preserve registers: see cprint
sprint:
	lodsb
	cmp al, 0
	jne _dochar
	add byte [ypos], 1; down one row
	mov byte [xpos], 0; to front of the row
	ret

; clears the VGA text buffer by filling it with spaces
; does not preserve registers: see cprint
; does not preserve memory: xpos, ypos
clear:
	mov al, ' '		    ; character = ' '
	mov byte [ypos], 0  ; start at top row
.yloop:
	mov byte [xpos], 0  ; reset column to beginning
.xloop:
	call cprint		    ; print a space character
	cmp byte [xpos], 79 ; if (xpos == 79)
	jne .xloop		    ; continue
	add byte [ypos], 1  ; move to next row
	cmp byte [ypos], 20 ; if (ypos == 20)
	jne .yloop		    ; continue

	ret

; prints a character to VGA text memory
; inputs: al = character
; modifies: xpos += 1
; does not preserve registers: ah, bx, cx, dx, di
cprint:
	mov ah, 0x1f	; attribute: white on black text
	mov cx, ax	    ; save character and attribute
	movzx ax, byte [ypos]
	mov dx, 160	 ; 2 bytes for 80 columns
	mul dx
	movzx bx, byte [xpos]
	shl bx, 1	 ; skip attribute

	mov di, 0   ; start of video memory
	add di, ax  ; add y offset
	add di, bx  ; add x offset

	mov ax, cx		    ; restore character and attribute
	stosw			    ; write character and attribute
	add byte [xpos], 1  ; move 1 column to the right

	ret

; prints the value of reg16 to VGA text memory
; inputs: reg16 = value, xpos = x, ypos = y
; modifies: ypos, xpos (see sprint)
; does not preserve registers: see cprint
printreg16:
	mov di, outstr16 + 2
	mov ax, [reg16]
	mov si, hexstr
	mov cx, 4 ; four places
.hexloop:
	rol ax, 4
	mov bx, ax
	and bx, 0x0f
	mov bl, [si + bx]
	mov [di], bl
	inc di
	dec cx
	jnz .hexloop

	mov si, outstr16
	call sprint

	ret

; moves the VGA text cursor to a new position
; inputs: ax = y, bx = x
; does not preserve registers: ax, bx, dx
; more info at https://wiki.osdev.org/Text_Mode_Cursor
movecursor:
	mov dl, 80
	mul dl      ; multiply y by the number of columns
	add bx, ax  ; load position index into bx

	mov dx, 0x3d4   ; CRT controller address register https://web.stanford.edu/class/cs140/projects/pintos/specs/freevga/vga/crtcreg.htm
	mov al, 0x0f    ; cursor location high register
	out dx, al

	inc dl          ; CRT data register
	mov al, bl
	out dx, al	    ; set high register

	dec dl		    ; CRT address register
	mov al, 0x0e	; cursor location low register
	out dx, al

	inc dl		    ; CRT data register
	mov al, bh
	out dx, al	    ; set low register

	ret

xpos db 0
ypos db 0
hexstr db '0123456789ABCDEF'
outstr16 db '0x0000',0
reg16 dw 0
