// table.h

#ifndef TABLE_H
#define TABLE_H

#include <logic.h>

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

table_t *table_new(int nRows, int nCols);
table_t *table_load(LPCTSTR lpszFileData);
void table_free(table_t* t);

typedef enum {
    DECISION = 1,
    SOLVE = 2
} tree_node_type;

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

tree_node_t* table_to_tree(table_t* t);
list_t* tree_to_rules(tree_node_t* n);
list_t* table_to_rules(table_t* t);

void print_rules_db(table_t* t, list_t* r);

#endif
