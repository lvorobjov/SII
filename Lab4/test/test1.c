#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <asmdef.h>

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
	JRCXZ " .end3\n\t" \
    ".loop3:\n\t" \
    "repne " SCAS_TCHAR \
	"jne .end3\n\t" \
    "inc %0\n\t" \
	"test " RCX "," RCX "\n\t" \
    "jnz .loop3\n\t" \
	".end3:\n\t" \
	/* Upd. 1 */	\
	"cld\n\t"	\
	"test %0,%0\n\t" \
	"jnz .skip3\n\t" \
	INC_TCHAR(RDI) \
	"mov " RCX "," RDX "\n\t" \
	JRCXZ " .skip4\n\t" \
	"mov " AX_TCHAR ", ':'\n\t" \
	"repne " SCAS_TCHAR \
	"jne .skip4\n\t" \
	".skip3:\n\t" \
	"inc %0\n\t" \
	".skip4:\n\t"
	
int count_attrs(TCHAR* str) {
	int count;
    __asm__ (
        COUNT_ATTRIBUTES
        : "=b" (count)
        : "D" (str)
        : RAX, RDX, RCX, "cc", "memory"
    );
	return count;
}

typedef struct _test_t {
	const TCHAR* str;
	int count;
} test_t;

int main() {
	const int n_tests = 16;
	test_t tests[] = {
		{ _T("A=1:B=2;C=3;D=4."), 3 },
		{ _T(";;."), 3 },
		{ _T(";;;."), 4 },
		{ _T(":;;."), 3 },
		{ _T("A:B;C;D."), 3 },
		{ _T("A;B;C;D."), 4 },
		{ _T("A:B;C;D"), 3 },
		{ _T("A;B;C;D"), 4 },
		{ _T(":B;C;D."), 3 },
		{ _T(":B;C;D"), 3 },
		{ _T(":;;"), 3 },
		{ _T("."), 0 },
		{ _T(""), 0 },
		{ _T(";B;C;D."), 4 },
		{ _T(";B;C;D"), 4 },
		{ _T(";;;"), 4 },
		/* Upd. 1 */
		{ _T("A:B."), 1 },
		{ _T("C:D"), 1 },
		{ _T("E."), 0 },
		{ _T("F"), 0 },
		{ _T(":G."), 1 },
		{ _T(":."), 1 },
		{ _T(":H"), 1 },
		{ _T(":"), 1 }
	};

	TCHAR str[64];
	for (int i=0; i<n_tests; i++) {
		_tcscpy(str, tests[i].str);
		if (count_attrs(str) != tests[i].count) {
			_tprintf(_T("Fail: count_attrs(\"%s\") = %d != %d\n"), str, count_attrs(str), tests[i].count);
			return 1;
		}
	}
	_tprintf(_T("OK: count_attrs()\n"));
	return 0;
}