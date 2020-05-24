extern ExitProcess:near
extern MessageBoxA:near
extern wsprintfA:near
extern lstrcpyA:near

global _main
extern _count_attrs

%include 'consts.inc' 
%define BUFFER_SIZE 64

STRUC test_case
.str 	resd 1
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

_main:
	push DWORD handler
	push DWORD [fs:0]
	mov  DWORD [fs:0],esp

	mov ecx, len
	lea edi, [tests]
	jecxz .end
.loop1:
	push ecx
	push dword [edi+test_case.str]
	push buffer
	call lstrcpyA
	pop ecx
	
	push buffer
	call _count_attrs
	add esp, 4
	cmp eax, dword [edi+test_case.count]
	je .skip
	
	push dword [edi+test_case.count]
	push eax
	push dword [edi+test_case.str]
	push msg_fail
	push buffer
	call wsprintfA
	add esp, 20
	
	push MB_ICONEXCLAMATION
	push msg_title
	push buffer
	push dword 0
	call MessageBoxA
	jmp exit
	
.skip:
	add edi, test_case_size
	loop .loop1
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
	
section .data
	msg_title db 'Test2',0
	msg_fail db 'Fail: count_attrs("%s") = %d != %d',0
	msg_ok  db 'OK: count_attrs()',0
	msg_exception db 'Fail: exception',0
	
;
; Test strings
;
str1	db 'A=1:B=2;C=3;D=4.',0
str2	db ';;.',0
str3	db ';;;.',0
str4	db ':;;.',0
str5	db 'A:B;C;D.',0
str6	db 'A;B;C;D.',0
str7	db 'A:B;C;D',0
str8	db 'A;B;C;D',0
str9	db ':B;C;D.',0
str10	db ':B;C;D',0
str11	db ':;;',0
str12	db '.',0
str13	db 0
str14	db ';B;C;D.',0
str15	db ';B;C;D',0
str16	db ';;;',0
; Upd. 1
str17	db 'A:B.',0
str18	db 'C:D',0
str19	db 'E.',0
str20	db 'F',0
str21	db ':G.',0
str22	db ':.',0
str23 	db ':H',0
str24	db ':',0

;
; Test table
;
tests:
	dd str1, 3
	dd str2, 3
	dd str3, 4
	dd str4, 3
	dd str5, 3
	dd str6, 4
	dd str7, 3
	dd str8, 4
	dd str9, 3
	dd str10, 3
	dd str11, 3
	dd str12, 0
	dd str13, 0
	dd str14, 4
	dd str15, 4
	dd str16, 4
	; Upd. 1
	dd str17, 1
	dd str18, 1
	dd str19, 0
	dd str20, 0
	dd str21, 1
	dd str22, 1
	dd str23, 1
	dd str24, 1
len	equ ($-tests) >> 3
