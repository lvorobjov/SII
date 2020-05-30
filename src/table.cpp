// table.cpp

#include "table.h"
#include <math.h>
#include <string.h>

#include "trace.h"
#include "parser.h"

#include <sstream>
using namespace std;

static double table_entropy(table_t* t, int i0, int n);
static double table_entropy_separate(table_t* t, int i0, int n, int xCol);
static int table_select_criteria(table_t* t, int i0, int n);
static void table_recursive_c45(table_t* t, int i0, int n, tree_node_t* node);

#ifdef _VERBOSE
  #define TRACE_TABLE(t,i0,n,xCol) \
 	print_table(t,i0,n,xCol);
  #define TRACE_TREE(n) \
 	print_tree(n);
  #define TRACE_RULES(r) \
 	print_rules(r);		\
    _tprintf(_T("\n\nПродукционные правила на языке Prolog:"));	\
	print_rules_prolog(r);
  #define BEGIN_TRACE_GAINS(nAttrs)  \
    double gains[nAttrs];
  #define TRACE_GAIN(i,g) \
    gains[i-1] = g;
  #define END_TRACE_GAINS(nAttrs,xCol) \
    print_gains(gains,nAttrs,xCol);
  #define TRACE_BEGIN(i0,n) \
    _tprintf(_T("\nАнализируется таблица от %d до %d.\n"), i0+1, i0+n);
  #define TRACE_END(i0,n) \
    _tprintf(_T("\nЗавершен анализ таблицы от %d до %d.\n"), i0+1, i0+n);
  #define TRACE_SOLVE(i,c,g) \
    _tprintf(_T("\nНайдено правило: $C_{%d} = %d \\Longrightarrow G = %d$.\n"),i,c,g);
#else
  #define TRACE_TABLE(t,i0,n,xCol)
  #define TRACE_TREE(n)
  #define TRACE_RULES(r)
  #define BEGIN_TRACE_GAINS(nAttrs)
  #define TRACE_GAIN(i,g)
  #define END_TRACE_GAINS(nAttrs,xCol)
  #define TRACE_BEGIN(i0,n)
  #define TRACE_END(i0,n)
  #define TRACE_SOLVE(i,c,g)
#endif

table_t* table_new(int nRows, int nCols) {
	table_t* t = (table_t*)malloc(sizeof(table_t));
	t->nRows = nRows;
	t->nCols = nCols;
	t->rows = (roadsign_t*)malloc(sizeof(roadsign_t)*nRows);
	t->cols = (column_t*)malloc(sizeof(column_t)*nCols);
    memset(t->rows, 0, nRows * sizeof(roadsign_t));
    memset(t->cols, 0, nCols * sizeof(column_t));
	return t;
}

void table_free(table_t* t) {
    if (t == NULL)
        return;
    for (int i=0; i < t->nCols; i++) {
        free(t->cols[i].name.c_str());
        free(t->cols[i].title.c_str());
        free(t->cols[i].query.c_str());
        free(t->cols[i].lpszCases);
    }
	free(t->rows);
	free(t->cols);
	free(t);
}

tree_node_t* node_decision_init(tree_node_t* n, column_t* col) {
    n->dwType = tree_node_type::DECISION;
	n->col = col;
	n->nodes = (tree_node_t*)malloc(col->nCases * sizeof(tree_node_t));
    memset(n->nodes, 0, col->nCases * sizeof(tree_node_t));
    return n;
}

tree_node_t* node_solve_init(tree_node_t* n, column_t* col, DWORD dwClass) {
    n->dwType = tree_node_type::SOLVE;
	n->col = col;
	n->dwClass = dwClass;
    return n;
}

static void node_free(tree_node_t* n) {
    if (n->dwType == tree_node_type::DECISION) {
		for (int i=0; i < n->col->nCases; i++) {
			node_free(&n->nodes[i]);
		}
		free(n->nodes);
	}
}

void tree_free(tree_node_t* root) {
    node_free(root);
    free (root);
}

double table_entropy(table_t* t, int i0, int n) {
	int nCases = t->cols[0].nCases;
	int *counts = new int [nCases];
    memset(counts, 0, nCases*sizeof(int));
	for (int i=i0; i<i0+n; i++) {
		counts[t->rows[i].dwClass]++;
	}
	double entropy = 0.0;
	for (int i=0; i<nCases; i++) {
        if (counts[i] > 0) {
            double p = (double)counts[i]/(double)n;
    		entropy -= p * log2(p);
        }
	}
	delete [] counts;
	return entropy;
}

/**
 * @param xCol номер столбца критерия разбиения, обычно не равен 0
 */
double table_entropy_separate(table_t* t, int i0, int n, int xCol) {
	int nCases = t->cols[0].nCases;
	int nXCases = t->cols[xCol].nCases;
	int *counts = new int[nCases*nXCases];
	int *sizes = new int[nXCases];
    memset(counts, 0, nCases*nXCases*sizeof(int));
    memset(sizes, 0, nXCases*sizeof(int));
	for (int i=i0; i<i0+n; i++) {
		int xColValue = t->rows[i].bAttrs[xCol-1];
		counts[xColValue*nCases + t->rows[i].dwClass]++;
		sizes[xColValue]++;
	}
	double sumEntropy = 0.0;
	for (int i=0; i<nXCases; i++) {
		double entropy = 0.0;
		for (int j=0; j<nCases; j++) {
			if (counts[i*nCases + j] > 0) {
				double p = (double)counts[i*nCases + j]/(double)n;
				entropy -= p * log2(p);
			}
		}
		sumEntropy += entropy * (double)sizes[i]/(double)n;
	}
	delete [] counts;
	delete [] sizes;
	return sumEntropy;
}

/**
 * @return номер столбца найденного критерия, если критерий не найден, то -1
 */
int table_select_criteria(table_t* t, int i0, int n) {
	double entropy = table_entropy(t, i0, n);
	double minEntropy = entropy;
	double xEntropy;
    BEGIN_TRACE_GAINS(N_ATTRS);
	int xCol = -1;
	for (int i=1; i < t->nCols; i++) {
		xEntropy = table_entropy_separate(t, i0, n, i);
		TRACE_GAIN(i,entropy-xEntropy);
		if (xEntropy < minEntropy) {
			minEntropy = xEntropy;
			xCol = i;
		}
	}
    END_TRACE_GAINS(N_ATTRS,xCol);
	return xCol;
}


static int rows_compare(roadsign_t* row1, roadsign_t* row2, int xCol) {
	return row1->bAttrs[xCol-1] - row2->bAttrs[xCol-1];
}

void table_qsort_criteria(table_t* t, int i_0, int i_n, int xCol) {
	// сортировка
	int i,j;
    roadsign_t row;
    bool left = true;
    i = i_0;
    j = i_n;
    while(i<j)
    {
        if(rows_compare(&t->rows[i], &t->rows[j], xCol) > 0)
        {
            row = t->rows[j];
            t->rows[j] = t->rows[i];
            t->rows[i] = row;
            left = !left;
        }
        if(left) j--;
        else i++;
    }
    if(i > i_0) table_qsort_criteria(t,i_0,i,xCol);
    if(i+1 < i_n) table_qsort_criteria(t,i+1,i_n,xCol);
}

void table_foreach_subtable(table_t* t, int i0, int n, int xCol, tree_node_t* node,
		void (*subtable_callback)(table_t* t, int i0, int n,
		bool isMono, int xCol, tree_node_t* node)) {
	int subtable_i0 = i0;
	int subtable_n = 0;
	DWORD dwLastClass = t->rows[i0].dwClass;
	BYTE bLastAttr = t->rows[i0].bAttrs[xCol-1];
	DWORD dwClass;
	BYTE bAttr;
	bool isMono = true;
	for (int i=i0; i<i0+n; i++) {
		dwClass = t->rows[i].dwClass;
		bAttr = t->rows[i].bAttrs[xCol-1];
		if (bAttr != bLastAttr) {
			// нашли подтаблицу
			(*subtable_callback)(t, subtable_i0, subtable_n, isMono, xCol, node);
			subtable_i0 = i;
			subtable_n = 0;
			dwLastClass = dwClass;
			bLastAttr = bAttr;
			isMono = true;
		} else {
            if (isMono && dwClass != dwLastClass)
    			isMono = false;
        }
		subtable_n ++;
	}
	// последняя подтаблица
	(*subtable_callback)(t, subtable_i0, subtable_n, isMono, xCol, node);
}

void table_callback(table_t* t, int i0, int n, bool isMono,
		int xCol, tree_node_t* node) {
	BYTE bAttr = t->rows[i0].bAttrs[xCol-1];
    DWORD dwClass = t->rows[i0].dwClass;
	tree_node_t* subnode = &node->nodes[bAttr];
	if (isMono) {
		node_solve_init(subnode, &t->cols[0], dwClass);
        TRACE_SOLVE(xCol-1,(int)bAttr,dwClass);
	} else {
		table_recursive_c45(t, i0, n, subnode);
	}
}

void table_recursive_c45(table_t* t, int i0, int n, tree_node_t* node) {
    TRACE_BEGIN(i0,n);
	int xCol = table_select_criteria(t, i0, n);
	if (xCol < 0)
		return; // TODO: ситуация не имеет решения
	// сортировка
	table_qsort_criteria(t, i0, n+i0-1, xCol);
	TRACE_TABLE(t,i0,n,xCol);
	// передор подтаблиц
	node_decision_init(node, &t->cols[xCol]);
	table_foreach_subtable(t, i0, n, xCol, node, table_callback);
    TRACE_END(i0,n);
}

tree_node_t* table_to_tree(table_t* t) {
	tree_node_t* n = (tree_node_t*)malloc(sizeof(tree_node_t));
	table_recursive_c45(t, 0, t->nRows, n);
    TRACE_TREE(n);
	return n;
}

list_t* tree_to_rules(tree_node_t* n) {
	static int nested = 0;
	list_t* list = NULL;
    if (n->dwType == tree_node_type::DECISION) {
		nested++;
		list_t* pList = list;
		list_t* othList;
		for (int i=0; i < n->col->nCases; i++) {
            pList = list_append(pList,
				othList = tree_to_rules(&n->nodes[i]));
			if (list == NULL)
				list = pList;
			while (othList != NULL) {
                statement_t* stmt = &(reinterpret_cast<rule_t*>(othList -> data) -> premises[nested-1]);
                stmt->attr = n->col;
				stmt->dwValue = i;
				othList = othList -> next;
			}
		}
		nested--;
    } else if (n->dwType == tree_node_type::SOLVE) {
		rule_t* rule = rule_new(nested);
        rule->conclusion.attr = n->col;
		rule->conclusion.dwValue = n->dwClass;
        list_add(list, rule);
	}
	return list;
}

list_t* table_to_rules(table_t* t) {
    tree_node_t* root_node = table_to_tree(t);
    list_t* rule_list = tree_to_rules(root_node);
    TRACE_RULES(rule_list);
    tree_free(root_node);
    return rule_list;
}

static
int count_rules(list_t* rules) {
    if (! rules)
        return 0;
    if (rules -> next == NULL) {
        return 1;
    } else {
        return 1 + count_rules(rules -> next);
    }
}

void print_rules_db(table_t* t, list_t* r) {
    int nAttrs = t->nCols;
    int nRules = count_rules(r);
    int nCases;
    LPTSTR pszCase;
    _tprintf(_T("%d %d\n"), nAttrs, nRules);

    // Printing attribute list
    for (int i=0; i<nAttrs; i++) {
        _tprintf(_T("%ls:%ls:%ls:"),
            t->cols[i].name.c_str(),
            t->cols[i].title.c_str(),
            t->cols[i].query.c_str());
        nCases = t->cols[i].nCases;
        pszCase = attribute_first_case(&t->cols[i]);
        for (int j=0; pszCase != NULL; j++) {
            _tprintf(_T(" %ls%ls"), pszCase, (j < nCases-1)? _T(";") : _T(".\n"));
            pszCase = attribute_next_case(&t->cols[i], pszCase);
        }
    }

    rule_t* rule;
    for (int i=0; i<nRules && r != NULL; i++) {
        rule = reinterpret_cast<rule_t*>(r->data);
        _tprintf(_T("%ls=%d:"),
            rule->conclusion.attr->name.c_str(),
            rule->conclusion.dwValue);
        for (int j=0; j<rule->nPremises; j++) {
            _tprintf(_T("%ls=%d%ls"),
                rule->premises[j].attr->name.c_str(),
                rule->premises[j].dwValue,
                (j < rule->nPremises-1)? _T(";") : _T(".\n"));
        }
        r=r->next;
    }
}

table_t *table_load(const wstring &fileData) {
    wistringstream ss(fileData);
    wstring line;
    int nCols;
    int nRows;
    ss >> nCols >> nRows;
    getline(ss, line);
    table_t* t = table_new(nRows, nCols);
    for (int i=0; i<nCols; i++) {
        getline(ss, line);
        LPTSTR lineDup = _tcsdup(line.c_str());
        Parser::parseAttributeRecord(lineDup, &t->cols[i]);
        free(lineDup);
    }
    LPTSTR saveptr;
    for (int i=0; i<nRows; i++) {
        getline(ss, line);
        auto pos = line.find(_T('\t'));
        t->rows[i].title = line.substr(0, pos);
        t->rows[i].dwClass = _tcstol(line.c_str()+pos+1, &saveptr, 10);
        for (int j=0; j<nCols-1; j++) {
            auto ptr = ++saveptr;
            t->rows[i].bAttrs[j] = _tcstol(ptr, &saveptr, 10);
        }
    }
    return t;
}
