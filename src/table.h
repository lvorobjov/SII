// table.h

#ifndef TABLE_H
#define TABLE_H

#include <attribute.h>

#define N_ATTRS 4

typedef struct _roadsign_t {
	LPTSTR lpszTitle;
	DWORD dwClass;
	BYTE bAttrs[N_ATTRS];
} roadsign_t;

typedef attribute_t column_t;

typedef struct _table_t {
	int nRows;
	int nCols;
	roadsign_t* rows;
	column_t* cols;
} table_t;

table_t* table_new(int nRows, int nCols);
void table_free(table_t* t);

#define TREE_NODE_TYPE_DECIDION 1
#define TREE_NODE_TYPE_SOLVE 	2

typedef struct _tree_node_t {
	DWORD dwType;
	column_t* col;
	union {
		DWORD dwClass;
		struct _tree_node_t* nodes;
	};
} tree_node_t;

tree_node_t* node_decision_init(tree_node_t* n, column_t* col);
tree_node_t* node_solve_init(tree_node_t* n, column_t* col, DWORD dwClass);
void tree_free(tree_node_t* n);

typedef struct _statement_t {
	column_t* col;
	DWORD dwValue;
} statement_t;

typedef struct _rule_t {
	int nPremises;
	statement_t* premises;
	statement_t conclusion;
} rule_t;

typedef struct _rule_list_t {
	rule_t* rule;
	struct _rule_list_t* next;
} rule_list_t;

rule_t* rule_new(int nPremises);
void rule_free(rule_t* r);

rule_list_t* rule_list_add(rule_list_t* list, rule_t* rule);
rule_list_t* rule_list_append(rule_list_t* list, rule_list_t* list2);
void rule_list_free(rule_list_t* list);

tree_node_t* table_to_tree(table_t* t);
rule_list_t* tree_to_rules(tree_node_t* n);
rule_list_t* table_to_rules(table_t* t);

void print_rules_db(table_t* t, rule_list_t* r);

#endif
