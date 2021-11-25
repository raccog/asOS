/*
 * VGA print functions for second bootloader
 *
 * Included in the second bootloader.
 *
 * Modified from https://wiki.osdev.org/Babystep4
 */

# data
outstr16:	.asciz "0x0000"
reg16:		.word 0
xpos:		.byte 0
ypos:		.byte 0

# string constant
hexstr:		.ascii "0123456789ABCDEF"

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
	lodsb					# load byte from ds:si
	or al, al
	jnz sprint_dochar		# if al!=0 print next character
	add byte ptr [ypos], 1	# move to next row
	mov byte ptr [xpos], 0	# reset column to 0

	ret

printreg16:
	mov di, offset outstr16 + 2	# get offset of output buffer
	mov ax, [reg16]				# move value into ax
	mov si, offset hexstr		# get offset of character string
	mov cx, 4					# 4 hex characters
printreg16_loop:
	rol ax, 4		# rotate left-most 4-bits to the right-most 4-bits
	mov bx, ax		# copy ax to bx
	and bx, 0x0f	# extract right-most 4-bits
	mov bl, [si + bx]
	mov [di], bl	# copy hex character to output buffer
	inc di			# move to next output character
	loop printreg16_loop

	mov si, outstr16
	call sprint		# print output buffer

	ret


