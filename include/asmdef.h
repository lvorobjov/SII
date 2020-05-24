/**
 * @Author: Lev Vorobjev
 * @Date:   06.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: asmdef.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 12.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#ifndef ASMDEF_H
#define ASMDEF_H

#ifdef _WIN64
  #define RAX   "rax"
  #define RBX   "rbx"
  #define RCX   "rcx"
  #define RDX   "rdx"
  #define RSI   "rsi"
  #define RDI   "rdi"
  #define JRCXZ "jrcxz"
#else
  #define RAX   "eax"
  #define RBX   "ebx"
  #define RCX   "ecx"
  #define RDX   "edx"
  #define RSI   "esi"
  #define RDI   "edi"
  #define JRCXZ "jecxz"
#endif

#ifdef _UNICODE
  #define SCAS_TCHAR    \
    "scasw\n\t"
  #define MOVS_TCHAR    \
    "movsw\n\t"
  #define STOS_TCHAR    \
    "stosw\n\t"
  #define LODS_TCHAR    \
    "lodsw\n\t"
  #define CMPS_TCHAR    \
    "cmpsw\n\t"
  #define AX_TCHAR		\
	"ax"
  #define TCHAR_PTR(reg)    \
    "word ptr [" reg "]"
  #define TCHAR_SIZE "2"
  #define CMP_AX_TCHAR(reg) \
    "cmp " AX_TCHAR ", " TCHAR_PTR(reg) "\n\t"
  #define MOV_AX_TCHAR(reg) \
    "mov " AX_TCHAR ", " TCHAR_PTR(reg) "\n\t"
  #define INC_TCHAR(reg) \
    "add " reg ", 2\n\t"
  #define DEC_TCHAR(reg) \
    "sub " reg ", 2\n\t"
#else
  #define SCAS_TCHAR    \
    "scasb\n\t"
  #define MOVS_TCHAR    \
    "movsb\n\t"
  #define STOS_TCHAR    \
    "stosb\n\t"
  #define LODS_TCHAR    \
    "lodsb\n\t"
  #define CMPS_TCHAR    \
    "cmpsb\n\t"
  #define AX_TCHAR		\
    "al"
  #define TCHAR_PTR(reg)    \
    "byte ptr [" reg "]"
  #define TCHAR_SIZE "1"
  #define CMP_AX_TCHAR(reg) \
    "cmp " AX_TCHAR ", " TCHAR_PTR(reg) "\n\t"
  #define MOV_AX_TCHAR(reg) \
    "mov " AX_TCHAR ", " TCHAR_PTR(reg) "\n\t"
  #define INC_TCHAR(reg) \
    "inc " reg "\n\t"
  #define DEC_TCHAR(reg) \
    "dec " reg "\n\t"
#endif

#endif
