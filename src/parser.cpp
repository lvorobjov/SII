/**
 * @Author: Lev Vorobjev
 * @Date:   06.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: parser.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 08.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#include "parser.h"

#include <stdlib.h>
#include <string.h>

#include "asmdef.h"
#include "filemap.h"

#define PARSE_COLUMN \
    ".intel_syntax noprefix\n\t" \
    "cld\n\t"	\
    "xor %0,%0\n\t"	\
    ".loop1:\n\t"	\
    "xchg " RDI "," RSI "\n\t"	\
    "mov ax, ' '\n\t"	\
    "repe " SCAS_TCHAR	\
    "xchg " RSI "," RDI "\n\t"	\
    DEC_TCHAR(RSI)	\
    "inc " RCX "\n\t"	\
    ".loop2:\n\t"	\
    LODS_TCHAR	\
    "cmp " AX_TCHAR ", ';'\n\t"	\
    "je .end\n\t"	\
    "cmp " AX_TCHAR ", '.'\n\t"	\
    "je .end1\n\t"	\
    "cmp " AX_TCHAR ", 0\n\t"	\
    "je .end1\n\t"	\
    STOS_TCHAR	\
    "loop .loop2\n\t"	\
    "jmp .end1\n\t"	\
    ".end:\n\t"	\
    "xor ax, ax\n\t"	\
    STOS_TCHAR	\
    "inc %0\n\t"	\
    "loop .loop1\n\t"	\
    "jmp .end2\n\t"	\
    ".end1:\n\t"	\
    "inc %0\n\t"	\
    "xor ax, ax\n\t"	\
    STOS_TCHAR	\
    ".end2:\n\t"	\
    STOS_TCHAR

static
void ParseColumn(LPTSTR lpszRecord, column_t* col) {
    LPTSTR ptr;
    LPTSTR dst;
    int len;
    int count;

    LPTSTR saveptr;
    // Заполнение первых трех полей
    ptr = _tcstok(lpszRecord, _T(":"), &saveptr);
    col->lpszName = _tcsdup(ptr);
    ptr = _tcstok(NULL, _T(":"), &saveptr);
    col->lpszTitle = _tcsdup(ptr);
    ptr = _tcstok(NULL, _T(":"), &saveptr);
    col->lpszQuery = _tcsdup(ptr);
    ptr = _tcstok(NULL, _T(":"), &saveptr);

    // Разбор списка значений
    len = _tcslen(ptr);
    dst = (LPTSTR)calloc(len+2, sizeof(TCHAR));
    col->lpszCases = dst;
    __asm__ __volatile__ (
        PARSE_COLUMN
        : "=b" (count)
        : "S" (ptr), "D" (dst), "c" (len)
        : RAX, "cc", "memory"
    );
    col->nCases = count;
}

#if 0
void collapse(LPTSTR lpszBuffer) {
    __asm__ (
        ".intel_syntax noprefix\n\t"
        "cld\n\t"
        "mov " RCX ", 0x200\n\t"
        ".loop1:\n\t"
        "lodsw\n\t"
        "stosb\n\t"
        "loop .loop1\n\t"
        :
        : "S" (lpszBuffer), "D" (lpszBuffer)
        : RCX, "memory"
    );
}
#endif

table_t* LoadTable(LPCTSTR lpszFileData) {
    LPTSTR lpszFileDup = _tcsdup(lpszFileData);
    LPTSTR lpszLine;
    int nCols;
    int nRows;
    LPTSTR lineptr;
    lpszLine = _tcstok(lpszFileDup, _T("\r\n"), &lineptr);
    _stscanf(lpszLine, _T("%d %d"), &nCols, &nRows);
    table_t* t = table_new(nRows, nCols);
    for (int i=0; i<nCols; i++) {
        lpszLine = _tcstok(NULL, _T("\r\n"), &lineptr);
        ParseColumn(lpszLine, &t->cols[i]);
    }
    LPTSTR ptr;
    LPTSTR saveptr;
    for (int i=0; i<nRows; i++) {
        lpszLine = _tcstok(NULL, _T("\r\n"), &lineptr);
        ptr = _tcschr(lpszLine, (int)_T('\t'));
        *ptr++ = _T('\0');
        t->rows[i].lpszTitle = _tcsdup(lpszLine);
        t->rows[i].dwClass = _tcstol(ptr, &saveptr, 10);
        for (int j=0; j<nCols-1; j++) {
            ptr = ++saveptr;
            t->rows[i].bAttrs[j] = _tcstol(ptr, &saveptr, 10);
        }
    }
    free(lpszFileDup);
    return t;
}

void FreeTable(table_t* t) {
    if (t == NULL)
        return;
    for (int i=0; i < t->nCols; i++) {
        free(t->cols[i].lpszName);
        free(t->cols[i].lpszTitle);
        free(t->cols[i].lpszQuery);
        free(t->cols[i].lpszCases);
    }
    for (int i=0; i < t->nRows; i++) {
        free(t->rows[i].lpszTitle);
    }
    table_free(t);
}
