//
// Created by JacquesdeH on 2020/9/25.
//

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

typedef unsigned int uint;

namespace config
{
    const bool PRINT_LEXIC = true;

    const int BUFFER_SIZE = 0x10000;

    const int ROW_INIT = 0; // start with 1
    const int COLUMN_INIT = 0; // start with 1

    enum TokenCode
    {
        IDENFR, INTCON, CHARCON, STRCON, CONSTTK, INTTK, CHARTK, VOIDTK, MAINTK, IFTK,
        ELSETK, SWITCHTK, CASETK, DEFAULTTK, WHILETK, FORTK, SCANFTK, PRINTFTK, RETURNTK, PLUS,
        MINU, MULT, DIV, LSS, LEQ, GRE, GEQ, EQL, NEQ, COLON,
        ASSIGN, SEMICN, COMMA, LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE,
        EMPTY
    };

    const std::string RESERVED[] = {"const", "int", "char", "void", "main", "if", "else", "switch", "case", "default",
                                    "while", "for", "scanf", "printf", "return"};

    enum SymbolType
    {
        CONST, VAR, FUNCTION
    };

    enum DataType
    {
        INT, CHAR, VOID
    };
}

#endif //PROJECT_CONFIG_H
