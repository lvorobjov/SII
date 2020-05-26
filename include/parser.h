/**
 * @Author: Lev Vorobjev
 * @Date:   09.03.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: parser.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 13.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2017 Lev Vorobjev
 */

#ifndef PARSER_H
#define PARSER_H

#include "logic.h"

class Parser {
protected:
    virtual int getAttributeCount() const = 0;
    virtual attribute_t* getAttribute(int index) const = 0;
    virtual attribute_t* getAttributeByName(LPCTSTR lpszName) const = 0;
    virtual void addRule(rule_t* rule) = 0;

#ifdef _TEST_MODULE
  protected:
#else
  private:
#endif
    static void parseAttributeRecord(LPTSTR lpszRecord, attribute_t* attr);
    static LPTSTR parseAttributeHead(LPTSTR lpszHead, int nFields, ...);
    static LPTSTR parseAttributeBody(LPCTSTR lpszBody, int* nCases);
    rule_t* parseRuleRecord(LPTSTR lpszRecord);
    int countAttributes(LPCTSTR lpszRecord);
    void parseStatement(LPTSTR lpszStmt, statement_t* stmt);

public:
    virtual void init(int nAttrs) = 0;
    virtual void load(LPTSTR lpszData);
    virtual void free();
};

#endif
