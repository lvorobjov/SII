section .text

%include 'defs.inc'

global _parse_attr
_parse_attr:
	push R(bp)
	mov R(bp), R(sp)
	push R(BX)
	
	cld
    xor R(BX),R(BX)
.loop1:
    xchg R(DI),R(SI)
    mov ax, ' '
    repe SCAS_TCHAR
    xchg R(SI),R(DI)
    DEC_TCHAR(S)
	inc R(CX)
.loop2:
	LODS_TCHAR
	cmp AX_TCHAR, ';'
    je .end
	cmp AX_TCHAR, '.'
    je .end1
	cmp AX_TCHAR, 0
	je .end1
    STOS_TCHAR
    loop .loop2
	jmp .end1
.end:
    xor ax, ax
    STOS_TCHAR
    inc R(BX)
	loop .loop1
	jmp .end2
.end1:
	inc R(BX)
	xor ax, ax
	STOS_TCHAR
.end2:
    STOS_TCHAR
	
	mov R(AX), R(BX)
	pop R(BX)
	leave
	ret
	
section .data
	sep	db ';.',0
	len equ $-sep