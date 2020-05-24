section .text

global _strlen
global _strcpy
global _strcmp
global _strtr

_strlen:
	mov ecx, -1
	xor ax,ax
	repne scasb
	neg ecx
	dec ecx
	ret
	
_strcpy:
	mov ecx, [esp+12]
	mov esi, [esp+8]
	mov edi, [esp+4]
	rep movsb
	ret 12

_strcmp:
	mov ecx, [esp+12]
	mov esi, [esp+8]
	mov edi, [esp+4]
	repe cmpsb
	ret 12

_strtr:
	mov ecx, [esp+8]
	mov edi, [esp+4]
	xor eax,eax
	jecxz .end
.loop1:
	repne scasb
	jne .end
	mov byte [edi-1],'_'
	test ecx,ecx
	jnz .loop1
.end:
	stosb
	ret
