/**
 * @Author: Lev Vorobjev
 * @Date:   09.03.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: parser.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 06.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2017 Lev Vorobjev
 */

#ifndef PARSER_H
#define PARSER_H

#include "table.h"

table_t* LoadTable(LPCTSTR lpszFilename);
void FreeTable(table_t* t);

#endif
