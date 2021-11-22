	/* check if a20 line is enabled */
	call a20_check
	cmp ax, 0
	jnz a20_enabled

	/* enable a20 line through BIOS */
	mov ax, 0x2403	# check for BIOS a20 support
	int 0x15
	jb a20_keycontroller
	cmp ah, 0
	jnz a20_keycontroller

	mov ax, 0x2402	# check a20 gate status
	int 0x15
	jb a20_keycontroller
	cmp ah, 0
	jnz a20_keycontroller
	cmp al, 1
	je a20_enabled

	mov ax, 0x2401	# enable a20 gate
	int 0x15
	jb a20_keycontroller
	cmp ah, 0
	jnz a20_keycontroller

	call a20_check	# check if a20 gate is enabled without BIOS
	cmp ax, 0
	jnz a20_enabled

	/* enable a20 line through keyboard controller */
a20_keycontroller:
	call a20_wait
	mov al, 0xad	# ad = disable first PS/2 port
	out 0x64, al

	call a20_wait
	mov al, 0xd0	# d0 = read controller output port
	out 0x64, al

	call a20_wait2
	in al, 0x60		# read a20 status to ax
	push ax			# push status to stack

	call a20_wait
	mov al, 0xd1	# d1 = write to controller output port
	out 0x64, al
	
	call a20_wait
	pop ax			# pop a20 status
	or al, 2		# set bit to enable a20 line
	out 0x60, al

	call a20_wait
	mov al, 0xae	# ae = enable first PS/2 port
	out 0x64, al

	call a20_wait

	mov cx, 0xff
a20_check_loop1:
	call a20_check	# check a20 line 256 times
	cmp ax, 0
	jnz a20_enabled
	loop a20_check_loop1

a20_fast:
	in al, 0x92		# read fast a20 gate register
	cmp al, 2		# check if a20 is enabled
	jne a20_enabled 
	or al, 2		# set a20 enable bit
	and al, 0xfe	# ensure bit 0 is not set (used for fast reset)
	out 0x92, al

a20_fast_done:
	mov cx, 0xff
a20_check_loop2:
	call a20_check	# check a20 line 256 times
	cmp ax, 0
	jnz a20_enabled
	loop a20_check_loop2

	/* display error and halt, a20 line could not be enabled */
	mov si, offset a20_err
	call sprint

	hlt

	/* a20 line is enabled */
a20_enabled:
	mov si, offset a20_msg
	call sprint

/* runs a loop until the input buffer is clear */
a20_wait:
	in al, 0x64		# read status register
	test al, 2		# if input buffer is full
	jnz a20_wait	# check again
	ret

/* runs a loop until the output buffer is full */
a20_wait2:
	in al, 0x64		# read status register
	test al, 1		# if output buffer is empty
	jz a20_wait2	# check again
	ret

/* checks if the a20 line is enabled by checking if two different
 * memory addresses point to the same data
 *
 * outputs: ax = 0 if disabled, 1 if enabled
 *
 * modified from https://wiki.osdev.org/A20_Line
 */
a20_check:
	push ds

	xor ax, ax
	not ax
	mov ds, ax	# ds = 0xffff

	mov di, 0x500
	mov si, 0x510

	es mov byte ptr [di], 0x00
	ds mov byte ptr [si], 0xff

	es cmp byte ptr [di], 0xff

	mov ax, 0
	je a20_check_exit

	mov ax, 1
a20_check_exit:
	pop ds
	ret

a20_msg:	.asciz "a20 line is enabled"
a20_err:	.asciz "a20 line coult not be enabled. Halting."
