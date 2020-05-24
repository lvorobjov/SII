extern ExitProcess:near
extern MessageBoxA:near
extern wsprintfA:near

global _main
extern _parse_attr

extern _strlen
extern _strcpy
extern _strcmp
extern _strtr

%include 'consts.inc' 
%define BUFFER_SIZE 128

STRUC test_case
.str 	resd 1
.res	resd 1
.len	resd 1
.count	resd 1
ENDSTRUC

section .text

safeseh handler
handler:
	push MB_ICONHAND
	push msg_title
	push msg_exception
	push DWORD 0
	call MessageBoxA
	dec eax
	jmp exit

_strneg:
	mov eax, -1
	mov ecx, BUFFER_SIZE>>2
	rep stosd
	ret

_main:
	push DWORD handler
	push DWORD [fs:0]
	mov  DWORD [fs:0],esp

	mov edx, len
	lea ebx, [tests]
	test edx,edx
	jz .end
.loop1:
	; strneg
	mov edi, buffer2
	call _strneg

	; parse_attrs
	mov edi, dword [ebx+test_case.str]
	call _strlen
	mov esi, dword [ebx+test_case.str]
	mov edi, buffer2
	call _parse_attr
	
	cmp eax, dword [ebx+test_case.count]
	jne .case1
	
	; strneg
	mov edi, buffer3
	call _strneg
	
	; strcpy
	push dword [ebx+test_case.len]
	push dword [ebx+test_case.res]
	push buffer3
	call _strcpy
	
	; strcmp
	;push BUFFER_SIZE
	;push buffer3
	;push buffer2
	;call _strcmp
	mov ecx, BUFFER_SIZE>>4
	mov edi, buffer3
	mov esi, buffer2
	repe cmpsd
	je .skip
	
	; strtr
	push dword [ebx+test_case.len]
	push buffer2
	call _strtr
	add esp,4
	push buffer3
	call _strtr
	add esp,8
	
	push buffer3
	push buffer2
	push dword [ebx+test_case.str]
	push msg_fail2
	push buffer
	call wsprintfA
	add esp, 20
	jmp .show
	
.case1:
	push dword [ebx+test_case.count]
	push eax
	push dword [ebx+test_case.str]
	push msg_fail1
	push buffer
	call wsprintfA
	add esp, 20
	
.show:
	push MB_ICONEXCLAMATION
	push msg_title
	push buffer
	push dword 0
	call MessageBoxA
	jmp exit
	
.skip:
	add ebx, test_case_size
	dec edx
	test edx,edx
	jnz .loop1
.end
	
	push MB_OK
	push msg_title
	push msg_ok
	push dword 0
	call MessageBoxA
	
exit:
	push dword 0
	call ExitProcess
	
section .bss
	buffer resb BUFFER_SIZE
	buffer2 resb BUFFER_SIZE
	buffer3 resb BUFFER_SIZE
	
section .data
	msg_title db 'Test3',0
	msg_fail1 db 'Fail: parse_attrs("%s") = %d',13,10,'Expected "%d"',0
	msg_fail2 db 'Fail: parse_attrs("%s").',13,10,'Buffer: "%s"',13,10,'Expected "%s"',0
	msg_ok  db 'OK: parse_attrs()',0
	msg_exception db 'Fail: exception',0

;
; Test strings
;

str1	db 'a;b;c;d.',0
res1	db 'a',0,'b',0,'c',0,'d',0,0
len1	equ $-res1

str2	db 'a; b;  c; d.',0
res2	db 'a',0,'b',0,'c',0,'d',0,0
len2	equ $-res2

str3	db ' a; b;  c; d.',0
res3	db 'a',0,'b',0,'c',0,'d',0,0
len3	equ $-res3

str4	db 'ab;cd;ef;ghi.',0
res4	db 'ab',0,'cd',0,'ef',0,'ghi',0,0
len4	equ $-res4

str5	db 'ab; cd;  ef; ghi.',0
res5	db 'ab',0,'cd',0,'ef',0,'ghi',0,0
len5	equ $-res5

str6	db ' ab; cd;  ef; ghi.',0
res6	db 'ab',0,'cd',0,'ef',0,'ghi',0,0
len6	equ $-res6

str7	db ' ab; cd;  ef; ghi',0
res7	db 'ab',0,'cd',0,'ef',0,'ghi',0,0
len7	equ $-res7

str8	db ';;;',0
res8	db 0,0,0,0,0
len8	equ $-res8

str9	db ';;;.',0
res9	db 0,0,0,0,0
len9	equ $-res9

str10	db ';a;b;c.',0
res10	db 0,'a',0,'b',0,'c',0,0
len10	equ $-res10

str11	db '.',0
res11	db 0,0
len11	equ $-res11

str12	db 0
res12	db 0,0
len12	equ $-res12

;
; Test table
;
tests:
	dd str1, res1, len1, 4
	dd str2, res2, len2, 4
	dd str3, res3, len3, 4
	dd str4, res4, len4, 4
	dd str5, res5, len5, 4
	dd str6, res6, len6, 4
	dd str7, res7, len7, 4
	dd str8, res8, len8, 4
	dd str9, res9, len9, 4
	dd str10, res10, len10, 4
	dd str11, res11, len11, 1
	dd str12, res12, len12, 1
len equ (($-tests)>>4)