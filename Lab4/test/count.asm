section .text

global _count_attrs
_count_attrs:
	push ebp
	mov ebp, esp
	push ebx
	push ecx
	push edx
	push edi
	
	cld
	mov ecx, -1
	xor eax, eax
	mov edi, [ebp+8]
	repne scasb
	neg ecx
	dec ecx
	dec edi
	mov edx,ecx
	std
	xor ebx,ebx
	mov al, ';'
	jecxz .end
.loop1:
	repne scasb
	jne .end
	inc ebx
	test ecx,ecx
	jne .loop1
.end:
	cld
	test ebx, ebx
	jnz .skip
	inc edi
	mov ecx,edx
	jecxz .skip2
	mov al, ':'
	repne scasb
	jne .skip2
.skip:
	inc ebx
.skip2:
	mov eax,ebx
	
	pop edi
	pop edx
	pop ecx
	pop ebx
	leave
	ret
