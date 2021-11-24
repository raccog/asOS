/*
 * VGA print functions for second bootloader
 *
 * Included in the second bootloader.
 *
 * Modified from https://wiki.osdev.org/Babystep4
 */
hexstr:		.ascii "0123456789ABCDEF"
outstr16:	.asciz "0x0000"
reg16:		.word 0
xpos:		.byte 0
ypos:		.byte 0

cprint:
	push es	# preserve es

	mov dx, 0xb800
	mov es, dx		# es = start of VGA memory

	mov ah, 0x0f	# attribute = white on black
	mov cx, ax		# save word/attribute
	movzx ax, byte ptr [ypos]
	mov dx, 160		# 2 bytes (char/attribute)
	mul dx			# for 80 columns
	movzx bx, byte ptr [xpos]
	shl bx, 1		# times 2 to skip attribute

	mov di, 0	# start of VGA text memory
	add di, ax	# add y offset
	add di, bx	# add x offset

	mov ax, cx				# restore char/attribute
	stosw					# write char/attribute
	add byte ptr [xpos], 1	# move to next column

	pop es	# restore es

	ret

sprint_dochar:
	call cprint
sprint:
	lodsb
	or al, al
	jnz sprint_dochar
	add byte ptr [ypos], 1
	mov byte ptr [xpos], 0

	ret

printreg16:
	mov di, offset outstr16
	mov ax, [reg16]
	mov si, offset hexstr + 2
	mov cx, 4	# 4 hex digits
printreg16_loop:
	rol ax, 4		# rotate hex values
	mov bx, ax
	and bx, 0x0f	# extract last hex value
	mov bl, [si + bx] # get hex ascii character
	mov [di], bl	# output hex ascii character
	inc di
	loop printreg16_loop

	mov si, outstr16
	call sprint

	ret


