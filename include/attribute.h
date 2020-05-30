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

#include <string>
using std::wstring;

#ifdef _WIN32
#include <windef.h>
#include <tchar.h>
#define wcstok wcstok_s
#define strtok strtok_s
#else
#ifdef _UNICODE
#include <wchar.h>
typedef wchar_t TCHAR;
#define _tcslen wcslen
#define _tcscmp wcscmp
#define _T(str) L##str
#define _tcstok wcstok
#define _stscanf swscanf
#define _tprintf wprintf
#define _tcschr wcschr
#define _tcstol wcstol
#define _tcsdup wcsdup
#define _tcsspn wcsspn
#define _tcscspn wcscspn
#define _tcscpy wcscpy
#define _tcsncpy wcsncpy
#define _tcspbrk wcspbrk
#else
typedef char TCHAR;
#define _tcslen strlen
#define _tcscmp strcmp
#define _T(str) str
#define _tcstok strtok
#define _stscanf sscanf
#define _tprintf printf
#define _tcschr strchr
#define _tcstol strtol
#define _tcsdup strdup
#define _tcsspn strspn
#define _tcscspn strcspn
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcspbrk strpbrk
#endif
#include <cstdint>
typedef const TCHAR *LPCTSTR;
typedef TCHAR *LPTSTR;
typedef uint32_t DWORD;
typedef uint8_t BYTE;
#endif

typedef struct _attribute_t {
    wstring name;
    wstring title;
    wstring query;
	int nCases;
	LPTSTR lpszCases;
} attribute_t;

void attribute_parse(const wstring &record, attribute_t* attr);
int attribute_set_cases(attribute_t* attr, ...);
LPTSTR attribute_get_case(attribute_t* attr, int nCase);
LPTSTR attribute_first_case(attribute_t* attr);
LPTSTR attribute_next_case(attribute_t* attr, LPTSTR lpszCase);
int attribute_get_value(attribute_t* attr, LPCTSTR lpszCase);

#endif
