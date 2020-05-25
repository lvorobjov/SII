/**
 * @Author: Lev Vorobjev
 * @Date:   09.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: attribute.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 12.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#ifdef _WIN32
#include <windef.h>
#include <tchar.h>
#else
#ifdef _UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif
typedef const TCHAR *LPCTSTR;
typedef TCHAR *LPTSTR;
#endif

typedef struct _attribute_t {
    LPTSTR lpszName;
    LPTSTR lpszTitle;
    LPTSTR lpszQuery;
	int nCases;
	LPTSTR lpszCases;
} attribute_t;

int attribute_set_cases(attribute_t* attr, ...);
LPTSTR attribute_get_case(attribute_t* attr, int nCase);
LPTSTR attribute_first_case(attribute_t* attr);
LPTSTR attribute_next_case(attribute_t* attr, LPTSTR lpszCase);
int attribute_get_value(attribute_t* attr, LPCTSTR lpszCase);

#endif
