%define R(x)	e %+ x

%ifdef _UNICODE
  %define SCAS_TCHAR    \
    scasw
  %define MOVS_TCHAR    \
    movsw
  %define STOS_TCHAR    \
    stosw
  %define LODS_TCHAR    \
    lodsw
  %define CMPS_TCHAR    \
    cmpsw
  %define AX_TCHAR		\
	ax
  %define CMP_AX_TCHAR(r)  \
    cmp ax, word [R(r %+ I)]
  %define MOV_AX_TCHAR(r)  \
    mov ax, word [R(r %+ I)]
  %define CMP_TCHAR(r)  \
    cmp word [R(r %+ I)]
  %define INC_TCHAR(r) \
    add R(r %+ I), 2
  %define DEC_TCHAR(r) \
    sub R(r %+ I), 2
%else
  %define SCAS_TCHAR    \
    scasb
  %define MOVS_TCHAR    \
    movsb
  %define STOS_TCHAR    \
    stosb
  %define LODS_TCHAR    \
    lodsb
  %define CMPS_TCHAR    \
    cmpsb
  %define AX_TCHAR		\
    al
  %define CMP_AX_TCHAR(r)  \
    cmp AX_TCHAR, byte [R(r %+ I)]
  %define MOV_AX_TCHAR(r)  \
    mov AX_TCHAR, byte [R(r %+ I)]
  %define CMP_TCHAR(r)  \
    cmp byte [R(r %+ I)]
  %define INC_TCHAR(r) \
    inc R(r %+ I)
  %define DEC_TCHAR(r) \
    dec R(r %+ I)
%endif