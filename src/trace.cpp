/**
 * @Author: Lev Vorobjev
 * @Date:   06.03.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: trace.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 08.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2017 Lev Vorobjev
 * This is free software that provided "as-is" and without 
 * any warranty to the extent permitted by applicable law.
 */

#include "trace.h"

#include <cstdlib>
#include <cstdio>

void print_table(table_t* t, int i0, int n, int xCol) {
    _tprintf(_T("$$\nT = \\left( \\begin{array}{l"));
    for (int i=0; i<t->nCols; i++) {
        _tprintf(_T("%ls"), (i==xCol) ? _T("|c|") : _T("c"));
    }
    _tprintf(_T("}\n\t & G"));
    for (int i=1; i < t->nCols; i++) {
        _tprintf(_T(" & C_{%d}"), i-1);
    }
    _tprintf(_T("\\\\\n"));
    for (int i=i0; i < i0+n; i++) {
        roadsign_t* row = &t->rows[i];
        _tprintf(_T("\t%d & %d"), i+1, row->dwClass);
        for (int j=0; j < t->nCols-1; j++) {
            _tprintf(_T(" & %d"), row->bAttrs[j]);
        }
        _tprintf(_T("\\\\\n"));
    }
    _tprintf(_T("\\end{array} \\right)\n$$\n"));
}

static int count_solves(tree_node_t* n) {
    if (n->dwType == tree_node_type::SOLVE) {
        return 1;
    } else if (n->dwType == tree_node_type::DECISION) {
        int count = 0;
        for (int i=0; i<n->col->nCases; i++) {
            count += count_solves(&n->nodes[i]);
        }
        return count;
    }
    return 0;
}

typedef struct _tree_context_t {
    LPCTSTR lpszRoot;
    tree_node_t* node;
} tree_context_t;

static tree_context_t* tree_context_new(
        LPCTSTR lpszRoot, tree_node_t* node) {
    tree_context_t* tree;
    tree = (tree_context_t*)malloc(sizeof(tree_context_t));
    tree -> lpszRoot = lpszRoot;
    tree -> node = node;
    return tree;
}

static stack_t *stack = NULL;

static void print_recursive_tree(tree_node_t* n) {
    if (n->dwType == tree_node_type::SOLVE) {
        _tprintf(_T("{\\hspace{0pt}%.12ls...} "),
            attribute_get_case(n->col, n->dwClass));
    } else if (n->dwType == tree_node_type::DECISION) {
        _tprintf(_T("[.{%ls} "), n->col->title.c_str());
        for (int i=0; i<n->col->nCases; i++) {
            if (n->nodes[i].dwType != 0) {
                _tprintf(_T("\\edge node[auto]{%ls}; "),
                    attribute_get_case(n->col, i));
                if (count_solves(&n->nodes[i]) <= 3) {
                    print_recursive_tree(&n->nodes[i]);
                } else {
                    _tprintf(_T("{см. далее} "));
                    stack_push(stack, tree_context_new(
                        attribute_get_case(n->col, i),
                        &n->nodes[i]
                    ));
                }
            }
        }
        _tprintf(_T("] "));
    }
}

void print_tree(tree_node_t* n) {
    stack_push(stack, tree_context_new(_T("Название паттерна?"), n));
    tree_context_t* tree;
    int i = 1;
    while (stack != NULL) {
        tree = reinterpret_cast<tree_context_t*>(stack_pop(stack));
        _tprintf(_T("%% require tikz-qtree\n"
            "\\begin{figure}[H]\n"
            "\t\\sffamily\n"
            "\t\\small\n"
        	"\t\\centering\n"
            "\t\\begin{tikzpicture}\n"
            "\t\\tikzset{level distance=60pt}\n"
            "\t\\tikzset{every leaf node/.style={text width=2cm,align=center,anchor=north}}\n"
            "\t\\tikzset{every internal node/.style={text width=4cm,align=center,anchor=north}}\n"
            "\t\\Tree [.{%ls} "), tree->lpszRoot);
        print_recursive_tree(tree->node);
        _tprintf(_T(" ]\n"
            "\t\\end{tikzpicture}\n"
            "\t\\caption{Дерево утверждений и фактов (часть~%d)}\n"
            "\\end{figure}\n"), i++
        );
    }
}

void print_rules(list_t* r) {
    _tprintf(_T("\n\\section{Продукционные правила}\n"
		"\n\\begin{enumerate}[itemindent=0pt]\n"));
    rule_t *rule;
    statement_t *stmt;
    while (r != NULL) {
        rule = reinterpret_cast<rule_t*>(r->data);
        _tprintf(_T("\t\\item \\begin{tabbing}\n"
            "\t\\hspace{4em}\\=\\kill\n"));
        for (int i=0; i<rule->nPremises; i++) {
            stmt = &rule -> premises[i];
            _tprintf(_T("\t\\bf %ls \\> \\tabfill{%ls = %ls} \\\\\n"),
                (i == 0) ? _T("ЕСЛИ") : _T("И"),
                stmt->attr->title.c_str(),
                attribute_get_case(stmt->attr, stmt->dwValue));
        }
        _tprintf(_T("\t\\bf %ls \\> \\tabfill{%ls = %ls}\n"),
            _T("ТО"), rule->conclusion.attr->title.c_str(),
            attribute_get_case(rule->conclusion.attr, rule->conclusion.dwValue));
        _tprintf(_T("\t\\end{tabbing}\n"));
        r = r -> next;
    }
    _tprintf(_T("\\end{enumerate}\n"));
}

void print_gains(double *gains, int nAttrs, int xCol) {
    _tprintf(_T("$$ \n\\begin{array}{l"));
    for (int i=0; i<nAttrs; i++) {
        _tprintf(_T("%ls"), (i==xCol-1) ? _T("|c|") : _T("c"));
    }
    _tprintf(_T("}\n\t "));
    for (int i=0; i < nAttrs; i++) {
        _tprintf(_T(" & C_{%d}"), i);
    }
    _tprintf(_T("\\\\\n \\textrm{GAIN}"));
    for (int i=0; i < nAttrs; i++) {
        _tprintf(_T(" & %.2f"), gains[i]);
    }
    _tprintf(_T("\n\\end{array}\n $$\n"));
}

void print_rules_prolog(list_t* r) {
    _tprintf(_T("\n\\begin{minted}{prolog}\n"));
    rule_t *rule;
    statement_t *stmt;
    while (r != NULL) {
        rule = reinterpret_cast<rule_t*>(r->data);
        _tprintf(_T("\troadsign(\"%ls\") :-"),
            attribute_get_case(rule->conclusion.attr, rule->conclusion.dwValue));
        for (int i=0; i<rule->nPremises; i++) {
            stmt = &rule -> premises[i];
            _tprintf(_T("\n\t\tpriznak(\"%ls = %ls\"),"),
                stmt->attr->title.c_str(),
                attribute_get_case(stmt->attr, stmt->dwValue));
        }
        _tprintf(_T("!.\n"));
        r = r -> next;
    }
    _tprintf(_T("\\end{minted}\n"));
}
