/**
 * @Author: Lev Vorobjev
 * @Date:   06.03.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: trace.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 08.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2017 Lev Vorobjev
 */

#ifndef TRACE_H
#define TRACE_H

#include "table.h"

void print_table(table_t* t, int i0, int n, int xCol);
void print_tree(tree_node_t* n);
void print_rules(list_t* r);
void print_rules_prolog(list_t* r);
void print_gains(double *gains, int nAttrs, int xCol);

#endif
