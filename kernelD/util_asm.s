
	.global _times2
	; int times2(int n);
_times2:
	push bp
	mov bp,sp
	push di
	push si
	mov ax,[bp+4]
	shl ax,1
	pop si
	pop di
	pop bp
	ret
.global _pipo

_pipo:
	push bp
	mov bp,sp
	push di
	push si
	mov ah, #0x0e	; = *$0e
	mov bx, #0x07	; = *$07
	movb al, #0x41	; = *$41 ; = #64; = 64
	pop si
	pushf
	int 0x10
	popf
	pop si
	pop di
	pop bp
	ret

.global _interrupt
_interrupt:
	push bp
	mov bp,sp
	push si
	mov ax,[bp+4] ; get the interrupt number in AL (int number)
	push ds ; use self-modifying code to call the right interrupt
	mov bx,cs
	mov ds,bx
	mov si,#intr
	mov [si+1],al ; change the 00 below to the contents of AL
	pop ds
	mov ax,[bp+6] ; get the other parameters AX, BX, CX, DX and DI
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]
	mov di,[bp+14]
	pop si
	pushf
	intr: int #0x00 ; call interrupt (00 will be changed above)
	popf
	pop bp
	ret 


.global _init_syscalls
.global _syscall_handler
.extern _syscall_handler
	; interrupt 0x80 vector (80 * 4 = 320)
	; interrupts vectors start at address 0x0000:0x0000
	; each vector uses 4 bytes (32-bit).
	int80_addr equ (0x80 * 4)
	; Set up the interrupt 0x80 vector used for system calls.
	; When an interrupt 0x80 occurs, it calls _int80_stub below.
	; void init_syscalls()
_init_syscalls:
	push bp
	mov bp,sp
	mov dx,#_int80_stub ; get the address of the interruption service routine (ISR)
	push ds
	push si
	mov ax,#0 ; interrupts vectors start at address 0x0000:0x0000
	mov ds,ax
	mov si,#int80_addr
	mov ax,cs ; have interrupt go to the current segment
	mov [si+2],ax
	mov [si],dx ; set up our vector
	pop si
	pop ds
	pop bp
	ret
	; Stub routine called when an interrupt 0x80 occurs.
	; This stub calls the syscall_handler function defined in the C code.
	; void syscall_handler(uint syscall_nb, uint arg1, uint arg2, uintarg3, arg4)
_int80_stub:
	; arguments are pushed from right to left (i.e. ax = 1st argument)
	push di
	push dx
	push cx
	push bx
	push ax
	sti
	call _syscall_handler
	; The following pops are required but we don't care about their values.
	; We just avoid popping into ax since we need to keep
	; it unchanged as it stores the function's return value.
	pop bx
	pop bx
	pop bx
	pop bx
	pop bx
	iret ; pop ip, cs and flags registerrs (3 registers) 




