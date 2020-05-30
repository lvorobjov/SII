/**
 * @Author: Lev Vorobjev
 * @Date:   05.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: logic.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 14.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#include "logic.h"

#include <stdlib.h>

rule_t* rule_new(int nPremises) {
	rule_t* r = (rule_t*)malloc(sizeof(rule_t));
	r->nPremises = nPremises;
	r->premises = (statement_t*)calloc(nPremises, sizeof(statement_t));
	return r;
}

void rule_free(rule_t* r) {
	if (r == NULL)
		return;
	free(r->premises);
	free(r);
}

static
void _rule_list_free(list_t* rules) {
    if (rules -> next != NULL)
		_rule_list_free(rules -> next);
    rule_free ((rule_t*)rules->data);
	free (rules);
}

void rule_list_free(list_t* &rules) {
	if (rules == NULL)
        return;
	_rule_list_free(rules);
	rules = NULL;
}

goal_t* goal_new(attribute_t* attr, rule_t* rule) {
    goal_t* goal = (goal_t*)malloc(sizeof(goal_t));
    goal -> attr = attr;
    goal -> rule = rule;
    return goal;
}

void goal_free(goal_t* goal) {
	if (goal == NULL)
		return;
    free(goal);
}

fact_t* fact_new(attribute_t* attr, DWORD dwValue, rule_t* rule) {
	fact_t* fact = (fact_t*)malloc(sizeof(fact_t));
    fact -> attr = attr;
    fact -> rule = rule;
    fact -> dwValue = dwValue;
    return fact;
}

void fact_free(fact_t* fact) {
	if (fact == NULL)
		return;
    free(fact);
}

static
void _fact_list_free(list_t* facts) {
    if (facts -> next != NULL)
		_fact_list_free(facts -> next);
    fact_free ((fact_t*)facts->data);
	free (facts);
}

void fact_list_free(list_t* &facts) {
	if (facts == NULL)
        return;
    _fact_list_free(facts);
	facts = NULL;
}

/// @deprecated
logic_context_t* logic_ctx_new(int nAttrs) {
    logic_context_t* ctx = (logic_context_t*)malloc(sizeof(logic_context_t));
    ctx -> nAttrs = nAttrs;
    ctx -> attrs = (attribute_t*)calloc(nAttrs, sizeof(attribute_t));
    ctx -> rules = NULL;
    return ctx;
}

/// @deprecated
void logic_ctx_free(logic_context_t* ctx) {
	if (ctx == NULL)
		return;
    free (ctx -> attrs);
    free (ctx);
}

/**
 * Поиск в списке правила с указанным атрибутом в заключении
 * @param rules список правил, или NULL для продолжения последнего поиска
 * @param attr аттрибут, находящийся в заключении искомого правила
 * @return искомое правило, или NULL, если правила нет
 */
rule_t* rule_list_find(list_t* rules, attribute_t* attr) {
    rule_t* rule;
    static list_t* saveList = NULL;
    if (rules == NULL)
        rules = saveList;
    while (rules != NULL) {
        rule = (rule_t*)rules->data;
        if (rule -> conclusion.attr == attr) {
            saveList = rules->next;
            return rule;
        }
        rules = rules->next;
    }
    saveList = NULL;
    return NULL;
}

/**
 * Поиск в списке факта с указанным атрибутом
 * @param facts список фактов
 * @param attr атрибут искомого факта
 * @return искомый факт, или NULL, если факта нет
 */
fact_t* fact_list_find(list_t* facts, attribute_t* attr) {
    fact_t* fact;
    while (facts != NULL) {
        fact = (fact_t*)facts->data;
        if (fact -> attr == attr)
            return fact;
        facts = facts->next;
    }
    return NULL;
}

// Состояние правила: необзаченное, ложное, истинное
typedef enum {
    NO_VALUE, RULE_TRUE, RULE_FALSE
} rule_state_t;

/**
 * Проверка правила
 * @param rule проверяемое правило
 * @param facts список известных фактов
 * @param attrs стек для неозначенных аттрибутов
 * @return одно из трех значений:
 *  NO_VALUE   значение правила неизвестно, стек attrs не пуст
 *  RULE_TRUE  правило истинно, стек attrs без изменений
 *  RULE_FALSE правило ложно, стек attrs пуст
 */
static
rule_state_t rule_check(rule_t* rule, list_t* facts, stack_t* &attrs) {
    rule_state_t state = RULE_TRUE;
    fact_t* fact;
    for (int i=0; i<rule->nPremises && state != RULE_FALSE; i++) {
        fact = fact_list_find(facts, rule->premises[i].attr);
        if (fact == NULL) {
            // Правило неозначенное
            stack_push(attrs, rule->premises[i].attr);
            state = NO_VALUE;
        } else if (fact->dwValue != rule->premises[i].dwValue) {
            // Правило ложно
            while (attrs != NULL)
                stack_pop(attrs);
            state = RULE_FALSE;
        }
    }
    return state;
}

prove_result_t prove(list_t* &rules, stack_t* &goals, list_t* &facts) {
    attribute_t* attr;
    rule_t* rule;
    rule_state_t state;
    static list_t* resetRules = NULL;
	prove_result_t result = PROVE_OK;
    while (goals != NULL) {
        // Находим все правила, приводящие к цели
        attr = ((goal_t*)goals->data)->attr;
        rule = rule_list_find(rules, attr);
        if (rule != NULL) {
            do {
                // Проверяем правило
                stack_t* attrs = NULL;
                state = rule_check(rule, facts, attrs);
                switch (state) {
                    case NO_VALUE:
                        // Поместить все неозначенный переменные в стек целей
                        while (attrs != NULL)
                            stack_push(goals, goal_new((attribute_t*)stack_pop(attrs), rule));
						return prove(rules, goals, facts);
                    case RULE_TRUE:
						goal_free((goal_t*)stack_pop(goals));
                        list_add(facts, fact_new(
                            rule->conclusion.attr,
                            rule->conclusion.dwValue, rule));
                    case RULE_FALSE:
                        list_append(resetRules, list_remove(rules, rule));
                        break;
                }
                if (state == RULE_TRUE)
                    break;
                rule = rule_list_find(NULL, attr);
            } while (rule != NULL);
        } else {
            if (attr -> query.c_str() != NULL) {
                // Запросить у пользователя ответ на текущую цель
                return PROVE_QUERY;
            } else {
				rule = ((goal_t*)goals->data) -> rule;
                goal_free((goal_t*)stack_pop(goals));
                if (rule == NULL) {
					result = PROVE_FAIL;
					break;
				} else {
					list_append(resetRules, list_remove(rules, rule));
				}
            }
        }
    }
    list_append(rules, resetRules);
	resetRules = NULL;
    return result;
}

void add_answer(stack_t* &goals, list_t* &facts, DWORD dwValue) {
	goal_t* goal = (goal_t*)stack_pop(goals);
	if (goal == NULL)
		return;
    list_add(facts, fact_new(goal->attr, dwValue, NULL));
	goal_free(goal);
}

static
list_t** list_to_array(list_t* list, int index, int *nItems) {
	if (list == NULL) {
		*nItems = index;
		return (list_t**)calloc(index, sizeof(list_t*));
	} else {
		list_t** pList = list_to_array(list -> next, index+1, nItems);
		pList[index] = list;
		return pList;
	}
}

static
int rule_list_comparator(const void* val1, const void* val2) {
	list_t* l1 = *((list_t**)val1);
	list_t* l2 = *((list_t**)val2);
	rule_t* r1 = (rule_t*)l1->data;
	rule_t* r2 = (rule_t*)l2->data;
	return (r1->nPremises - r2->nPremises);
}

static
list_t* _rule_list_qsort(list_t* rules) {
	int nRules;
	list_t** pListArray = list_to_array(rules, 0, &nRules);
	qsort(pListArray, nRules, sizeof(list_t*), rule_list_comparator);
	rules = pListArray[0];
	list_t* l = rules;
	for (int i=1; i<nRules; i++) {
		l->next = pListArray[i];
		l = l->next;
	}
	l->next = NULL;
	return rules;
}

void rule_list_qsort(list_t* &rules) {
	rules = _rule_list_qsort(rules);
}
