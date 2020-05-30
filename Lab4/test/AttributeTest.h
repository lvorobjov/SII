// AttributeTest.h
// Testing attribute_t, rule_t, goal_t, fact_t

#include <cxxtest/TestSuite.h>
#include <attribute.h>
#include <tchar.h>

#define N_CASES 4

typedef struct _test_case_t {
    const TCHAR* pStr;
    int len;
} test_case_t;

class AttributeTestSuite : public CxxTest::TestSuite {
private:
    attribute_t* attr;

    const test_case_t cases[N_CASES] = {
        { _T("Yes"), 4 },
        { _T("No"), 3 },
        { _T("Ignore"), 7 },
        { _T("Accept"), 7 },
    };
    int cases_len;

public:
    void setUp() {
        attr = (attribute_t*)malloc(sizeof(attribute_t));
        attr->name.c_str() = _tcsdup(_T("Attr1"));
        attr->title.c_str() = _tcsdup(_T("First attribute"));
        attr->query.c_str() = _tcsdup(_T("What is the first attribute"));
        int len = 0;
        for (int i=0; i<N_CASES; i++)
            len += cases[i].len;
        attr->lpszCases = (TCHAR*)calloc(len+1, sizeof(TCHAR));
        len = 0;
        for (int i=0; i<N_CASES; i++) {
            memcpy(attr->lpszCases + len, cases[i].pStr, cases[i].len * sizeof(TCHAR));
            len += cases[i].len;
        }
        attr->lpszCases[len] = _T('\0');
        attr->nCases = N_CASES;
        cases_len = len;
    }

    void tearDown() {
        free(attr -> name.c_str());
        free(attr -> title.c_str());
        free(attr -> query.c_str());
        free(attr -> lpszCases);
        free(attr);
    }

    void testAttributeSetCases() {
        TCHAR *pszBuf = (TCHAR*)calloc(cases_len, sizeof(TCHAR));
        memset(attr -> lpszCases, -1, cases_len * sizeof(TCHAR));
        memset(pszBuf, -1, cases_len * sizeof(TCHAR));
        attribute_set_cases(attr, _T("Yes"), _T("No"), _T("Accept"), NULL);
        memcpy(pszBuf, _T("Yes\0No\0Accept\0\0"), 15 * sizeof(TCHAR));
        TS_ASSERT_SAME_DATA(attr -> lpszCases, pszBuf, cases_len * sizeof(TCHAR));
        TS_ASSERT_EQUALS(attr -> nCases, 3);
        free(pszBuf);
    }

    void testAttributeGetCase() {
        TCHAR *pCase;
        for (int i=0; i<N_CASES; i++) {
            pCase = attribute_get_case(attr,i);
            TS_ASSERT_SAME_DATA(pCase, cases[i].pStr, cases[i].len * sizeof(TCHAR));
        }
        pCase = attribute_get_case(attr,N_CASES);
        TS_ASSERT_EQUALS(pCase, nullptr);
    }

    void testAttributeFirstNext() {
        TCHAR* pCase = attribute_first_case(attr);
        for (int i=0; i<N_CASES; i++) {
            TS_ASSERT_SAME_DATA(pCase, cases[i].pStr, cases[i].len * sizeof(TCHAR));
            pCase = attribute_next_case(attr, pCase);
        }
        TS_ASSERT_EQUALS(pCase, nullptr);
    }

    void testAttributeGetValue() {
        int value = attribute_get_value(attr, _T("Yes"));
        TS_ASSERT_EQUALS(value, 0);
        value = attribute_get_value(attr, _T("Accept"));
        TS_ASSERT_EQUALS(value, 3);
        value = attribute_get_value(attr, _T("Rollback"));
        TS_ASSERT_EQUALS(value, -1);
    }
};
