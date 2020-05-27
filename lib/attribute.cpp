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
