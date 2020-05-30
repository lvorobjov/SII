/**
 * @Author: Lev Vorobjev
 * @Date:   06.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: parser.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 13.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#include "parser.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <asmdef.h>

#include <functional>
using std::ref;

size_t Parser::parseAttributeHead(const wstring &head, int nFields, ...) {
    va_list args{};
    va_start(args, nFields);
    size_t index = 0;

    for (int i=0; i<nFields; i++) {
        auto pos = head.find(_T(":"), index);
        if (pos == wstring::npos)
            break;
        auto &field = va_arg(args, wstring&);
        field = head.substr(index,pos);
        index = pos + 1;
    }

    va_end(args);
    return index;
}

LPTSTR Parser::parseAttributeBody(const wstring &body, size_t pos, int* nCases) {
    // Разбор списка значений
    size_t len = body.length() - pos;
    LPCTSTR ptr = body.c_str() + pos;
    LPTSTR dst = (LPTSTR)calloc(len+2, sizeof(TCHAR));
    int count = 0;
    size_t index = 0;
    while (ptr) {
        ptr += _tcsspn(ptr, _T(" \t"));
        if (! *ptr)
            break;
        len = _tcscspn(ptr, _T(";."));
        _tcsncpy(dst+index, ptr, len);
        ptr += len;
        index += len + 1;
        count ++;
    }
    *nCases = count;
    return dst;
}

void Parser::parseAttributeRecord(const wstring &record, attribute_t* attr) {
    LPTSTR ptr;

    // Заполнение первых трех полей
    auto pos = parseAttributeHead(record, 3,
        ref(attr->name), ref(attr->title), ref(attr->query));

    // Разбор списка значений
    attr->lpszCases = parseAttributeBody(record, pos, &attr->nCases);
}

#define PARSE_STATEMENT \
    ".intel_syntax noprefix\n\t" \
    "cld\n\t"	\
    JRCXZ " 1f\n\t" \
    "mov " AX_TCHAR ", '='\n\t" \
    "repne " SCAS_TCHAR \
    "jne 1f\n\t" \
    "mov " TCHAR_PTR(RDI "-" TCHAR_SIZE) ", 0\n\t" \
    "1:\n\t"

void Parser::parseStatement(LPTSTR lpszStmt, statement_t* stmt) {
    LPTSTR ptr = lpszStmt;
    int len = _tcslen(ptr);
    __asm__ __volatile__ (
        PARSE_STATEMENT
        : "+D" (ptr)
        : "c" (len)
        : RAX, "cc", "memory"
    );
    stmt->attr = getAttributeByName(lpszStmt);
    if (stmt->attr == NULL)
        return;
    if (ptr[0] == _T('\0')) {
        stmt->dwValue = 1;
    } else {
        stmt->dwValue = _tcstol(ptr, NULL, 10);
    }
}

#define COUNT_ATTRIBUTES \
    ".intel_syntax noprefix\n\t" \
    "cld\n\t" \
    "mov " RCX ", -1\n\t" \
	"xor eax,eax\n\t" \
    "repne " SCAS_TCHAR \
    "neg " RCX "\n\t" \
	"dec " RCX "\n\t" \
	DEC_TCHAR(RDI) \
	"mov " RDX "," RCX "\n\t" \
    "std\n\t" \
    "xor %0,%0\n\t" \
    "mov " AX_TCHAR ", ';'\n\t" \
	JRCXZ " 2f\n\t" \
    "1:\n\t" \
    "repne " SCAS_TCHAR \
	"jne 2f\n\t" \
    "inc %0\n\t" \
	"test " RCX "," RCX "\n\t" \
    "jnz 1b\n\t" \
	"2:\n\t" \
	/* Upd. 1 */	\
	"cld\n\t"	\
	"test %0,%0\n\t" \
	"jnz 3f\n\t" \
	INC_TCHAR(RDI) \
	"mov " RCX "," RDX "\n\t" \
	JRCXZ " 4f\n\t" \
	"mov " AX_TCHAR ", ':'\n\t" \
	"repne " SCAS_TCHAR \
	"jne 4f\n\t" \
	"3:\n\t" \
	"inc %0\n\t" \
	"4:\n\t"

int Parser::countAttributes(LPCTSTR lpszRecord) {
    int nAttrs;
    volatile LPCTSTR ptr = lpszRecord;
    __asm__ (
        COUNT_ATTRIBUTES
        : "=b" (nAttrs)
        : "D" (ptr)
        : RAX, RDX, RCX, "cc", "memory"
    );
    return nAttrs;
}

rule_t* Parser::parseRuleRecord(LPTSTR lpszRecord) {
    rule_t* rule;
    LPTSTR ptr;
    LPTSTR saveptr;
    int nAttrs = countAttributes(lpszRecord);
    rule = rule_new(nAttrs);
    ptr = _tcstok(lpszRecord, _T(":\t "), &saveptr);
    parseStatement(ptr, &rule->conclusion);
    for (int i=0; i<nAttrs; i++) {
        ptr = _tcstok(NULL, _T(":;.\t "), &saveptr);
        parseStatement(ptr, &rule->premises[i]);
    }
    return rule;
}

void Parser::load(LPTSTR lpszData) {
    LPTSTR lpszLine;
    int nAttrs;
    int nRules;
    LPTSTR saveptr;
    lpszLine = _tcstok(lpszData, _T("\r\n"), &saveptr);
    _stscanf(lpszLine, _T("%d %d"), &nAttrs, &nRules);
    init(nAttrs);
    for (int i=0; i<nAttrs; i++) {
        lpszLine = _tcstok(NULL, _T("\r\n"), &saveptr);
        parseAttributeRecord(lpszLine, getAttribute(i));
    }
    for (int i=0; i<nRules; i++) {
        lpszLine = _tcstok(NULL, _T("\r\n"), &saveptr);
        addRule(parseRuleRecord(lpszLine));
    }
}

void Parser::free() {
    int nAttrs = getAttributeCount();
    attribute_t* attr;
    for (int i=0; i<nAttrs; i++) {
        attr = getAttribute(i);
        if (attr != NULL) {
            if (attr->lpszCases != NULL)
                ::free(attr->lpszCases);
        }
    }
}
