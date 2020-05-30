// ParserTest.h
// Testing parser for logic.lib

#include <cxxtest/TestSuite.h>
#include "../src/parser.h"
#include <stdio.h>
#include <string.h>
#include <tchar.h>

#define N_FIELDS 3
#define N_MAX_PREMISES 5
#define BUFFER_SIZE 64

#define N_ATTR_TEST_CASES 11
#define N_RULE_TEST_CASES 7
#define N_STMT_TEST_CASES 4

typedef struct _test_stmt {
	int index;
	int value;
} test_stmt_t;

typedef struct _test_case_attr {
	LPCTSTR szRecord;
	// testParseAttrHead
	LPCTSTR szFields[N_FIELDS];
	LPCTSTR szResult;
	// testParseAttrBody
	int nCases;
	LPCTSTR szCases;
	int nCasesLen;
} test_case_attr;

typedef struct _test_case_rule {
	LPCTSTR szRecord;
	// testCountPremises
	int nPremises;
	// testParceRule
	test_stmt_t conclusion;
	test_stmt_t premises[N_MAX_PREMISES];
} test_case_rule;

typedef struct _test_case_stmt {
	LPCTSTR szRecord;
	// testParceStmt
	test_stmt_t stmt;
} test_case_stmt;

class ParserTestSuite : public CxxTest::TestSuite, Parser  {
private:
	static const int nAttrs = 8;
	attribute_t *attrs;
	rule_t* rule;
	static const int nRules = 7;
	list_t* rules;
	LPTSTR lpszBuffer;

	static const test_case_attr attrCases[];
	static const test_case_rule ruleCases[];
	static const test_case_stmt stmtCases[];
	static const LPCTSTR szData;

protected:
	void init(int nAttrsNew) {
		TS_TRACE("Call init()");
		TS_ASSERT_EQUALS(nAttrsNew, nAttrs);
		Parser::free();
		memset(attrs, 0, nAttrs * sizeof(attribute_t));
	}
	int getAttributeCount() const {
        return nAttrs;
    }
	attribute_t* getAttribute(int index) const {
		return (index < nAttrs)? &attrs[index] : nullptr;
	}
	void addRule(rule_t* rule) {
        list_add(rules, rule);
    }
	attribute_t* getAttributeByName(LPCTSTR lpszName) const {
		for (int i=0; i<nAttrs; i++) {
			if (_tcscmp(attrs[i].lpszName, lpszName) == 0) {
				return &attrs[i];
			}
		}
		return NULL;
	}

public:
	void setUp() {
		attrs = (attribute_t*)calloc(nAttrs, sizeof(attribute_t));
		for (int i=0; i<nAttrs; i++) {
			attrs[i].lpszName = _tcsdup(attrCases[i].szFields[0]);
			if (attrCases[i].szFields[1] != nullptr)
				attrs[i].title.c_str() = _tcsdup(attrCases[i].szFields[1]);
			if (attrCases[i].szFields[2] != nullptr)
				attrs[i].lpszQuery = _tcsdup(attrCases[i].szFields[2]);
			attrs[i].lpszCases = (LPTSTR)calloc(attrCases[i].nCasesLen,sizeof(TCHAR));
			memcpy(attrs[i].lpszCases, attrCases[i].szCases, attrCases[i].nCasesLen * sizeof(TCHAR));
			attrs[i].nCases = attrCases[i].nCases;
		}
		rule = NULL;
		rules = NULL;
		lpszBuffer = (LPTSTR)calloc(BUFFER_SIZE, sizeof(TCHAR));
	}

	void tearDown() {
		Parser::free();
		::free(attrs);
		rule_free(rule);
		rule_list_free(rules);
		::free(lpszBuffer);
	}

#define N_BYTES(tcs) (_tcslen(tcs)+1)*sizeof(TCHAR)

#define TS_ASSERT_SAME_DATA_OR_NULL_AND_FREE(orig,expect) \
	if (orig == nullptr || expect == nullptr) {		\
		TS_ASSERT_EQUALS(orig, expect);				\
	} else {										\
		TS_ASSERT_SAME_DATA(orig, expect, N_BYTES(orig));	\
		::free(orig);								\
	}

#define TS_ASSERT_SAME_DATA_NOT_NULL_AND_FREE(orig,expect) \
	TS_ASSERT_DIFFERS(orig, nullptr);					\
	if (orig != nullptr) {								\
		TS_ASSERT_SAME_DATA(orig, expect, N_BYTES(orig));	\
		::free(orig);									\
	}

	void testParseAttrHead() {
		int nTestCases = N_ATTR_TEST_CASES;
		LPTSTR szFields[N_FIELDS];
		LPTSTR szResult;
		TS_TRACE(parseAttributeHead);
		for (int i=0; i<nTestCases; i++) {
			_tprintf(_T("Parsing head: %s\n"), attrCases[i].szRecord);
			_tcscpy(lpszBuffer, attrCases[i].szRecord);
			szResult = parseAttributeHead(lpszBuffer, N_FIELDS,
				&szFields[0], &szFields[1], &szFields[2]);
			TS_ASSERT_SAME_DATA(szResult, attrCases[i].szResult, N_BYTES(szResult));
			for (int j=0; j<N_FIELDS; j++) {
				TS_ASSERT_SAME_DATA_OR_NULL_AND_FREE(szFields[j], attrCases[i].szFields[j]);
			}
		}
	}

	void testParseAttrBody() {
		int nTestCases = N_ATTR_TEST_CASES;
		LPTSTR szCases;
		int nCases;
		TS_TRACE(parseAttributeBody);
		for (int i=0; i<nTestCases; i++) {
			_tprintf(_T("Parsing body: %s\n"), attrCases[i].szResult);
			szCases = parseAttributeBody(attrCases[i].szResult, &nCases);
			TS_ASSERT_EQUALS(nCases, attrCases[i].nCases);
			TS_ASSERT_SAME_DATA(szCases, attrCases[i].szCases, attrCases[i].nCasesLen * sizeof(TCHAR));
			::free(szCases);
		}
	}

	void testParseAttr() {
		int nTestCases = N_ATTR_TEST_CASES;
		attribute_t attr = {0};
		TS_TRACE(parseAttributeRecord);
		for (int i=0; i<nTestCases; i++) {
			_tprintf(_T("Parsing: %s\n"), attrCases[i].szRecord);
			_tcscpy(lpszBuffer, attrCases[i].szRecord);
			parseAttributeRecord(lpszBuffer, &attr);
			TS_ASSERT_SAME_DATA_NOT_NULL_AND_FREE(attr.lpszName, attrCases[i].szFields[0]);
			TS_ASSERT_SAME_DATA_OR_NULL_AND_FREE(attr.title.c_str(), attrCases[i].szFields[1]);
			TS_ASSERT_SAME_DATA_OR_NULL_AND_FREE(attr.lpszQuery, attrCases[i].szFields[2]);
			TS_ASSERT_DIFFERS(attr.lpszCases, nullptr);
			if (attr.lpszCases != nullptr) {
				TS_ASSERT_SAME_DATA(attr.lpszCases, attrCases[i].szCases, attrCases[i].nCasesLen * sizeof(TCHAR));
				::free(attr.lpszCases);
			}
			TS_ASSERT_EQUALS(attr.nCases, attrCases[i].nCases);
			memset(&attr, 0, sizeof(attribute_t));
		}
	}

	void testParceStmt() {
		int nTestCases = N_STMT_TEST_CASES;
		statement_t stmt = {0};
		TS_TRACE(parseStatement);
		for (int i=0; i<nTestCases; i++) {
			_tprintf(_T("Parsing statement: %s\n"), stmtCases[i].szRecord);
			_tcscpy(lpszBuffer, stmtCases[i].szRecord);
			parseStatement(lpszBuffer, &stmt);
			if (stmtCases[i].stmt.index == -1) {
				TS_ASSERT_EQUALS(stmt.attr, nullptr);
			} else {
				TS_ASSERT_EQUALS(stmt.attr, &attrs[stmtCases[i].stmt.index]);
			}
			TS_ASSERT_EQUALS(stmt.dwValue, stmtCases[i].stmt.value);
			memset(&stmt, 0, sizeof(statement_t));
		}
	}

	void testCountAttributes() {
		int nTestCases = N_RULE_TEST_CASES;
		int nPremises;
		TS_TRACE(countAttributes);
		for (int i=0; i<nTestCases; i++) {
			_tprintf(_T("Count premises: %s\n"), ruleCases[i].szRecord);
			nPremises = countAttributes(ruleCases[i].szRecord);
			TS_ASSERT_EQUALS(nPremises, ruleCases[i].nPremises);
		}
	}

	void testParceRule() {
		int nTestCases = N_RULE_TEST_CASES;
		TS_TRACE(parseRuleRecord);
		for (int i=0; i<nTestCases; i++) {
			_tprintf(_T("Parse: %s\n"), ruleCases[i].szRecord);
			_tcscpy(lpszBuffer, ruleCases[i].szRecord);
			rule = parseRuleRecord(lpszBuffer);
			TS_ASSERT_DIFFERS(rule, nullptr);
			if (rule != nullptr) {
				TS_ASSERT_EQUALS(rule->conclusion.attr, &attrs[ruleCases[i].conclusion.index]);
				TS_ASSERT_EQUALS(rule->conclusion.dwValue, ruleCases[i].conclusion.value);
				TS_ASSERT_EQUALS(rule->nPremises, ruleCases[i].nPremises);
				if (rule->nPremises <= ruleCases[i].nPremises) {
					for (int j=0; j<rule->nPremises; j++) {
						TS_ASSERT_EQUALS(rule->premises[j].attr, &attrs[ruleCases[i].premises[j].index]);
						TS_ASSERT_EQUALS(rule->premises[j].dwValue, ruleCases[i].premises[j].value);
					}
				}
				rule_free(rule);
			}
		}
		rule = NULL;
	}

#define TS_ASSERT_SAME_DATA_OR_NULL(orig,expect) \
	if (orig == nullptr || expect == nullptr) {		\
		TS_ASSERT_EQUALS(orig, expect);				\
	} else {										\
		TS_ASSERT_SAME_DATA(orig, expect, N_BYTES(orig));	\
	}

#define TS_ASSERT_SAME_DATA_NOT_NULL(orig,expect) \
	TS_ASSERT_DIFFERS(orig, nullptr);					\
	if (orig != nullptr) {								\
		TS_ASSERT_SAME_DATA(orig, expect, N_BYTES(orig));	\
	}


	void testLoad() {
		TS_TRACE("Load data...");
		LPTSTR lpszData = _tcsdup(szData);
		load(lpszData);
		::free(lpszData);

		for (int i=0; i<nAttrs; i++) {
			TS_ASSERT_SAME_DATA_NOT_NULL(attrs[i].lpszName, attrCases[i].szFields[0]);
			TS_ASSERT_SAME_DATA_OR_NULL(attrs[i].title.c_str(), attrCases[i].szFields[1]);
			TS_ASSERT_SAME_DATA_OR_NULL(attrs[i].lpszQuery, attrCases[i].szFields[2]);
			TS_ASSERT_DIFFERS(attrs[i].lpszCases, nullptr);
			if (attrs[i].lpszCases != nullptr) {
				TS_ASSERT_SAME_DATA(attrs[i].lpszCases, attrCases[i].szCases, attrCases[i].nCasesLen * sizeof(TCHAR));
			}
			TS_ASSERT_EQUALS(attrs[i].nCases, attrCases[i].nCases);
		}

		list_t* list = rules;
		rule_t* r;
		int count = 0;
		while (list != nullptr) {
			r = (rule_t*)list->data;
			TS_ASSERT_DIFFERS(r, nullptr);
			count ++;
			list = list->next;
		}
		TS_ASSERT_EQUALS(count, nRules);
	}
};

#define ATTR(i) (i)
#define OFFSET(i) (szRecord + i)

const test_case_attr ParserTestSuite::attrCases[] = {
	{ _T("A:American:Why is american?:None;West."),
 		{ _T("A"), _T("American"), _T("Why is american?") },
		_T("None;West."), 2, _T("None\0West\0\0"), 11 },
	{ _T("W:Weapon:Are there weapon?: No; Yes."),
		{ _T("W"), _T("Weapon"), _T("Are there weapon?") },
	 	_T(" No; Yes."), 2, _T("No\0Yes\0\0"), 8 },
	{ _T("S:Sells:Did West sells this missile?: No; Yes."),
 		{ _T("S"), _T("Sells"), _T("Did West sells this missile?") },
	 	_T(" No; Yes."), 2, _T("No\0Yes\0\0"), 8 },
	{ _T("H:Hostile:Nono is hostile?: No; Yes."),
 		{ _T("H"), _T("Hostile"), _T("Nono is hostile?") },
	 	_T(" No; Yes."), 2, _T("No\0Yes\0\0"), 8 },
	{ _T("M:Missile:It is missile?: No; Irrelevant; Yes."),
 		{ _T("M"), _T("Missile"), _T("It is missile?") },
		_T(" No; Irrelevant; Yes."), 3, _T("No\0Irrelevant\0Yes\0\0"), 19 },
	{ _T("O:Owns:Are Nono owns missile?: No;Yes."),
 		{ _T("O"), _T("Owns"), _T("Are Nono owns missile?") },
	 	_T(" No;Yes."), 2, _T("No\0Yes\0\0"), 8 },
	{ _T("E:Enemy:Are Nono is enemy for America?: No;Unknown;Yes."),
 		{ _T("E"), _T("Enemy"), _T("Are Nono is enemy for America?") },
		_T(" No;Unknown;Yes."), 3, _T("No\0Unknown\0Yes\0\0"), 16 },
	{ _T("C:Crimea:: No;Yes."), { _T("C"), _T("Crimea"), nullptr },
 		_T(" No;Yes."), 2, _T("No\0Yes\0\0"), 8 },
	{ _T("Z::."), { _T("Z"), nullptr, nullptr }, _T("."), 1, _T("\0\0"), 2 },
	{ _T("X:::"), { _T("X"), nullptr, nullptr }, _T(""), 1, _T("\0\0"), 2 },
	{ _T("Y::"), { _T("Y"), nullptr, nullptr }, _T(""), 1, _T("\0\0"), 2 }
};

const test_case_stmt ParserTestSuite::stmtCases[] = {
	{ _T("A=1"), { ATTR(0), 1 } },
	{ _T("W"), { ATTR(1), 1 } },
	{ _T("B"), { ATTR(-1), 0 } },
	{ _T("O=2"), { ATTR(5), 2 } }
};

const test_case_rule ParserTestSuite::ruleCases[] = {
	{ _T("C=1:A=1;W=1;S=1;H=1."), 4, { ATTR(7), 1 },
		{ { ATTR(0), 1 }, { ATTR(1), 1 }, { ATTR(2), 1 }, { ATTR(3), 1 } } },
	{ _T("S=1: M; O=1."), 2, { ATTR(2), 1 }, { { ATTR(4), 1 }, { ATTR(5), 1 } } },
	{ _T("M=2."), 0, { ATTR(4), 2 } },
	{ _T(" O=1."), 0, { ATTR(5), 1 } },
	{ _T("W=1 : M=2"), 1, { ATTR(1), 1 }, { { ATTR(4), 2 } } },
	{ _T("H=1:E=2"), 1, { ATTR(3), 1 }, { { ATTR(6), 2 } } },
	{ _T("E=2"), 0, { ATTR(6), 2 } }
};

const LPCTSTR ParserTestSuite::szData = _T("8 7\r\n"
	"A:American:Why is american?:None;West.\r\n"
	"W:Weapon:Are there weapon?: No; Yes.\r\n"
	"S:Sells:Did West sells this missile?: No; Yes.\r\n"
	"H:Hostile:Nono is hostile?: No; Yes.\r\n"
	"M:Missile:It is missile?: No; Irrelevant; Yes.\r\n"
	"O:Owns:Are Nono owns missile?: No;Yes.\r\n"
	"E:Enemy:Are Nono is enemy for America?: No;Unknown;Yes.\r\n"
	"C:Crimea:: No;Yes.\r\n"
	"C=1:A=1;W=1;S=1;H=1.\r\n"
	"S=1: M; O=1.\r\n"
	"M=2.\r\n"
	" O=1.\r\n"
	"W=1 : M=2\r\n"
	"H=1:E=2\r\n"
	"E=2");
