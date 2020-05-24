// LogicTest.h
// Testing rule_t, goal_t, fact_t

#include <cxxtest/TestSuite.h>
#include <logic.h>
#include <string.h>
#include <tchar.h>

#define N_RULES 5
#define CASES_LEN 32

class LogicTestSuite : public CxxTest::TestSuite {
private:
	static const int nAttrs = 3;
	attribute_t attrs[nAttrs];
	list_t* rules;
	stack_t* goals;
	list_t* facts;

public:
	rule_t* rule_init(int prem1, int prem2, int concl) {
		rule_t* rule = rule_new(2);
		rule->premises[0].attr = &attrs[1];
		rule->premises[0].dwValue = prem1;
		rule->premises[1].attr = &attrs[2];
		rule->premises[1].dwValue = prem2;
		rule->conclusion.attr = &attrs[0];
		rule->conclusion.dwValue = concl;
		return rule;
	}

	void setUp() {
		attrs[0].lpszName = _tcsdup(_T("A"));
		attrs[1].lpszName = _tcsdup(_T("B"));
		attrs[2].lpszName = _tcsdup(_T("C"));
		attrs[1].lpszQuery = _tcsdup(_T("B?"));
		attrs[2].lpszQuery = _tcsdup(_T("C?"));
		for (int i=0; i<nAttrs; i++) {
			attrs[i].lpszCases = (LPTSTR)calloc(CASES_LEN, sizeof(TCHAR));
			attribute_set_cases(&attrs[i], _T("No"), _T("Yes"), _T("Ignore"), NULL);
		}
		rules = NULL;
		goals = NULL;
		facts = NULL;
		// A = (B and not C) or (not B and C)
		// A = Ignore, if B = C = Ignore
		// A = undefined, if B = Ignore and C != Ignore, or vice verse
		list_t* l;
		l = list_add(rules, rule_init(1, 0, 1));
		l = list_add(l, rule_init(0, 1, 1));
		l = list_add(l, rule_init(0, 0, 0));
		l = list_add(l, rule_init(1, 1, 0));
		l = list_add(l, rule_init(2, 2, 2));
	}

	void tearDown() {
		for(int i=0; i<nAttrs; i++) {
			free(attrs[i].lpszName);
			free(attrs[i].lpszCases);
		}
		free(attrs[1].lpszQuery);
		free(attrs[2].lpszQuery);
		while (goals != NULL) {
			goal_free((goal_t*)stack_pop(goals));
		}
		rule_list_free(rules);
		fact_list_free(facts);
	}

	void testRuleListFind() {
		rule_t* founds[N_RULES];
		rule_t* rule;

		TS_TRACE("Find all rules with attrs[0]");
		rule = rule_list_find(rules, &attrs[0]);
		for (int i=0; i<N_RULES; i++) {
			TS_ASSERT_DIFFERS(rule, nullptr);
			for (int j=0; j<i; j++) {
				TS_ASSERT_DIFFERS(rule, founds[j]);
			}
			TS_ASSERT_EQUALS(rule->conclusion.attr, &attrs[0]);
			founds[i] = rule;
			rule = rule_list_find(NULL, &attrs[0]);
		}
		TS_ASSERT_EQUALS(rule, nullptr);

		TS_TRACE("Find all rules with attrs[1] (missing)");
		rule = rule_list_find(rules, &attrs[1]);
		TS_ASSERT_EQUALS(rule, nullptr);
	}

	void testFactListFind() {
		fact_t* fact;
		list_add(facts, fact_new(&attrs[1], 1, NULL));
		list_add(facts, fact_new(&attrs[2], 0, NULL));

		TS_TRACE("Find one fact with attr[0] (missing)");
		fact = fact_list_find(facts, &attrs[0]);
		TS_ASSERT_EQUALS(fact, nullptr);

		TS_TRACE("Find one fact with attr[1]");
		fact = fact_list_find(facts, &attrs[1]);
		TS_ASSERT_DIFFERS(fact, nullptr);
		TS_ASSERT_EQUALS(fact -> attr, &attrs[1]);
		TS_ASSERT_EQUALS(fact -> dwValue, 1);

		TS_TRACE("Find one fact with attr[2]");
		fact = fact_list_find(facts, &attrs[2]);
		TS_ASSERT_DIFFERS(fact, nullptr);
		TS_ASSERT_EQUALS(fact -> attr, &attrs[2]);
		TS_ASSERT_EQUALS(fact -> dwValue, 0);
	}

	void testAddAnswer() {
		stack_push(goals, goal_new(&attrs[1],NULL));
		stack_push(goals, goal_new(&attrs[2],NULL));
		fact_t* fact;

		TS_TRACE("Answer first question");
		add_answer(goals, facts, 1);
		fact = fact_list_find(facts, &attrs[1]);
		TS_ASSERT_EQUALS(fact, nullptr);
		fact = fact_list_find(facts, &attrs[2]);
		TS_ASSERT_DIFFERS(fact, nullptr);
		TS_ASSERT_EQUALS(fact -> dwValue, 1);
		TS_ASSERT_EQUALS(goals -> next, nullptr);

		TS_TRACE("Answer second question");
		add_answer(goals, facts, 2);
		fact = fact_list_find(facts, &attrs[1]);
		TS_ASSERT_DIFFERS(fact, nullptr);
		TS_ASSERT_EQUALS(fact -> dwValue, 2);
		TS_ASSERT_EQUALS(goals, nullptr);

		TS_TRACE("Answer null question");
		add_answer(goals, facts, 2);
		TS_ASSERT_EQUALS(goals, nullptr);
	}

#define GOAL_TOP(goals) \
	((goals == nullptr)? nullptr : (goal_t*)(goals -> data))

#define RULE_FIRST(rules) \
	((rules == nullptr)? nullptr : (rule_t*)(rules -> data))

	void checkFiveRues() {
		rule_t* exists[N_RULES];
		rule_t* rule;
		list_t* l = rules;
		int i;
		for (i=0; i<N_RULES && l != nullptr; i++) {
			rule = RULE_FIRST(l);
			TS_ASSERT_DIFFERS(rule, nullptr);
			for (int j=0; j<i; j++) {
				TS_ASSERT_DIFFERS(rule, exists[j]);
			}
			exists[i] = rule;
			l = l->next;
		}
		TS_ASSERT_EQUALS(i, N_RULES);
		if (i < N_RULES) {
			TS_FAIL("Less than five rules");
		}
		TS_ASSERT_EQUALS(l, nullptr);
	}

	void testProveOk() {
		const int nQuestions = 2;
		attribute_t* questions[nQuestions];
		prove_result_t res;
		stack_push(goals, goal_new(&attrs[0], NULL));
		attribute_t* question;

		for(int i=0; i<nQuestions; i++) {
			res = prove(rules, goals, facts);
			TS_ASSERT_EQUALS(res, PROVE_QUERY);
			TS_ASSERT_DIFFERS(GOAL_TOP(goals), nullptr);
			question = GOAL_TOP(goals) -> attr;
			TS_ASSERT_DIFFERS(question, &attrs[0]);
			for (int j=0; j<i; j++) {
				TS_ASSERT_DIFFERS(question, questions[j]);
			}
			questions[i] = question;
			add_answer(goals, facts, i);
		}
		res = prove(rules, goals, facts);
		TS_ASSERT_EQUALS(res, PROVE_OK);

		fact_t* fact = fact_list_find(facts, &attrs[0]);
		TS_ASSERT_DIFFERS(fact, nullptr);
		if (fact != nullptr)
			TS_ASSERT_EQUALS(fact -> dwValue, 1);

		TS_ASSERT_EQUALS(goals, nullptr);

		checkFiveRues();
	}

	void testProveFail() {
		const int nQuestions = 2;
		attribute_t* questions[nQuestions];
		prove_result_t res;
		stack_push(goals, goal_new(&attrs[0], NULL));
		attribute_t* question;

		for(int i=0; i<nQuestions; i++) {
			res = prove(rules, goals, facts);
			TS_ASSERT_EQUALS(res, PROVE_QUERY);
			TS_ASSERT_DIFFERS(GOAL_TOP(goals), nullptr);
			question = GOAL_TOP(goals) -> attr;
			TS_ASSERT_DIFFERS(question, &attrs[0]);
			for (int j=0; j<i; j++) {
				TS_ASSERT_DIFFERS(question, questions[j]);
			}
			questions[i] = question;
			add_answer(goals, facts, i+1);
		}
		res = prove(rules, goals, facts);
		TS_ASSERT_EQUALS(res, PROVE_FAIL);

		fact_t* fact = fact_list_find(facts, &attrs[0]);
		TS_ASSERT_EQUALS(fact, nullptr);

		TS_ASSERT_EQUALS(goals, nullptr);

		checkFiveRues();
	}
};
