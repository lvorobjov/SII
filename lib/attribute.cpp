/**
 * @Author: Lev Vorobjev
 * @Date:   09.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: attribute.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 12.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#include "attribute.h"

#include <stdarg.h>
#include <string.h>

#include <functional>
using std::ref;

struct pass {
    template <typename ... Ts>
    explicit pass(Ts...) { }
};

static wstring attribute_parse_field(const wstring &head, size_t &index) {
    auto pos = head.find(_T(":"), index);
    if (pos == wstring::npos)
        throw std::out_of_range(":");
    wstring res = head.substr(index,pos);
    index = pos + 1;
    return res;
}

template <typename ... Args>
size_t attribute_parse_head(const wstring &head, Args &... args) {
    size_t index = 0;
    try {
        pass{(args = attribute_parse_field(head, index), 1)...};
    } catch (std::out_of_range const&) {}
    return index;
}

static LPTSTR attribute_parse_body(const wstring &body, size_t pos, int* nCases) {
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

void attribute_parse(const wstring &record, attribute_t* attr) {
    // Заполнение первых трех полей
    auto pos = attribute_parse_head(record,
        attr->name, attr->title, attr->query);

    // Разбор списка значений
    attr->lpszCases = attribute_parse_body(record, pos, &attr->nCases);
}


int attribute_set_cases(attribute_t* attr, ...) {
    va_list args{};
    va_start(args, attr);

    int offset = 0, len, nCases = 0;
    LPTSTR pszCase;
    while ((pszCase = va_arg(args, LPTSTR)) != NULL) {
        len = _tcslen(pszCase) + 1;
        memcpy(attr -> lpszCases + offset, pszCase, len * sizeof(TCHAR));
        offset += len;
        nCases ++;
    }
    attr -> lpszCases[offset] = _T('\0');
    attr -> nCases = nCases;

    va_end(args);
    return offset;
}

LPTSTR attribute_get_case(attribute_t* attr, int nCase) {
    LPTSTR lpszCase = attr->lpszCases;
	if (nCase >= attr->nCases)
		return NULL;
	while (nCase > 0) {
		lpszCase += _tcslen(lpszCase) + 1;
		nCase --;
	}
	return lpszCase;
}

LPTSTR attribute_first_case(attribute_t* attr) {
    return attr -> lpszCases;
}

LPTSTR attribute_next_case(attribute_t* attr, LPTSTR lpszCase) {
    lpszCase += _tcslen(lpszCase) + 1;
    if (lpszCase[0] == _T('\0'))
        return NULL;
    return lpszCase;
}

int attribute_get_value(attribute_t* attr, LPCTSTR lpszCase) {
    LPTSTR ptr = attribute_first_case(attr);
    for (int i=0; i<attr->nCases; i++) {
        if (_tcscmp(lpszCase, ptr) == 0) {
            return i;
        }
        // next case
        ptr += _tcslen(ptr) + 1;
    }
    return -1;
}
