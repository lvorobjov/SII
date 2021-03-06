/**
 * @Author: Lev Vorobjev
 * @Date:   09.03.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: parser.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 13.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2017 Lev Vorobjev
 * This is free software that provided "as-is" and without 
 * any warranty to the extent permitted by applicable law.
 */

#ifndef PARSER_H
#define PARSER_H

#include "logic.h"

class Parser {
protected:
    virtual int getAttributeCount() const = 0;
    virtual attribute_t* getAttribute(int index) const = 0;
    virtual attribute_t* getAttributeByName(const wstring &name) const = 0;
    virtual void addRule(rule_t* rule) = 0;

#ifdef _TEST_MODULE
  protected:
#else
  private:
#endif
    rule_t* parseRuleRecord(LPTSTR lpszRecord);
    int countAttributes(LPCTSTR lpszRecord);
    void parseStatement(LPTSTR lpszStmt, statement_t* stmt);

public:
    virtual void init(int nAttrs) = 0;
    virtual void load(LPTSTR lpszData);
    virtual void free();
};

#endif
