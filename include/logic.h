/**
 * @Author: Lev Vorobjev
 * @Date:   05.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: logic.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 14.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 * This is free software that provided "as-is" and without 
 * any warranty to the extent permitted by applicable law.
 */

#ifndef LOGIC_H
#define LOGIC_H

#include "utils.h"
#include "attribute.h"

typedef struct _statement_t {
	attribute_t* attr;
	DWORD dwValue;
} statement_t;

typedef struct _rule_t {
	int nPremises;
	statement_t* premises;
	statement_t conclusion;
} rule_t;

rule_t* rule_new(int nPremises);
void rule_free(rule_t* r);
void rule_list_free(list_t* &rules);

typedef struct _goal_t {
    attribute_t* attr;
    rule_t* rule;
} goal_t;

goal_t* goal_new(attribute_t* attr, rule_t* rule);
void goal_free(goal_t* goal);

typedef struct _fact_t {
    attribute_t* attr;
    DWORD dwValue;
    rule_t* rule;
} fact_t;

fact_t* fact_new(attribute_t* attr, DWORD dwValue, rule_t* rule);
void fact_free(fact_t* fact);
void fact_list_free(list_t* &facts);

/**
 * @deprecated
 */
typedef struct _logic_context_t {
    int nAttrs;
    attribute_t* attrs;
    list_t* rules;
} logic_context_t;

logic_context_t* logic_ctx_new(int nAttrs);
void logic_ctx_free(logic_context_t* ctx);

typedef enum {
    PROVE_OK, PROVE_FAIL, PROVE_QUERY
} prove_result_t;

/**
 * Обратный логический вывод
 * @param rules список правил
 * @param goals стек целей, изначално включает глобальную цель
 * @param facts список фактов
 * @return ответ на глобальную цель или запрос ввода параметра (-1)
 */
prove_result_t prove(list_t* &rules, stack_t* &goals, list_t* &facts);

/**
 * Добаление новго факта в базу фактов
 * @papam goals стек целей, в вершине стека добавляемый в БФ аттрибут
 * @param facts база чактоы
 * @param dwValue значение добавляемого аттрибута
 */
void add_answer(stack_t* &goals, list_t* &facts, DWORD dwValue);

void rule_list_qsort(list_t* &rules);
rule_t* rule_list_find(list_t* rules, attribute_t* attr);
fact_t* fact_list_find(list_t* facts, attribute_t* attr);

#endif
