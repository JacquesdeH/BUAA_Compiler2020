//
// Created by JacquesdeH on 2020/9/26.
//

#include <iostream>
#include <algorithm>

#include "functional_convert.h"
#include "functional_strext.h"

string tkcode2output(const config::TokenCode& tkcode)
{
    string tkclass;
    switch (tkcode)
    {
        case config::IDENFR:
            tkclass = "IDENFR";
            break;
        case config::INTCON:
            tkclass = "INTCON";
            break;
        case config::CHARCON:
            tkclass = "CHARCON";
            break;
        case config::STRCON:
            tkclass = "STRCON";
            break;
        case config::CONSTTK:
            tkclass = "CONSTTK";
            break;
        case config::INTTK:
            tkclass = "INTTK";
            break;
        case config::CHARTK:
            tkclass = "CHARTK";
            break;
        case config::VOIDTK:
            tkclass = "VOIDTK";
            break;
        case config::MAINTK:
            tkclass = "MAINTK";
            break;
        case config::IFTK:
            tkclass = "IFTK";
            break;
        case config::ELSETK:
            tkclass = "ELSETK";
            break;
        case config::SWITCHTK:
            tkclass = "SWITCHTK";
            break;
        case config::CASETK:
            tkclass = "CASETK";
            break;
        case config::DEFAULTTK:
            tkclass = "DEFAULTTK";
            break;
        case config::WHILETK:
            tkclass = "WHILETK";
            break;
        case config::FORTK:
            tkclass = "FORTK";
            break;
        case config::SCANFTK:
            tkclass = "SCANFTK";
            break;
        case config::PRINTFTK:
            tkclass = "PRINTFTK";
            break;
        case config::RETURNTK:
            tkclass = "RETURNTK";
            break;
        case config::PLUS:
            tkclass = "PLUS";
            break;
        case config::MINU:
            tkclass = "MINU";
            break;
        case config::MULT:
            tkclass = "MULT";
            break;
        case config::DIV:
            tkclass = "DIV";
            break;
        case config::LSS:
            tkclass = "LSS";
            break;
        case config::LEQ:
            tkclass = "LEQ";
            break;
        case config::GRE:
            tkclass = "GRE";
            break;
        case config::GEQ:
            tkclass = "GEQ";
            break;
        case config::EQL:
            tkclass = "EQL";
            break;
        case config::NEQ:
            tkclass = "NEQ";
            break;
        case config::COLON:
            tkclass = "COLON";
            break;
        case config::ASSIGN:
            tkclass = "ASSIGN";
            break;
        case config::SEMICN:
            tkclass = "SEMICN";
            break;
        case config::COMMA:
            tkclass = "COMMA";
            break;
        case config::LPARENT:
            tkclass = "LPARENT";
            break;
        case config::RPARENT:
            tkclass = "RPARENT";
            break;
        case config::LBRACK:
            tkclass = "LBRACK";
            break;
        case config::RBRACK:
            tkclass = "RBRACK";
            break;
        case config::LBRACE:
            tkclass = "LBRACE";
            break;
        case config::RBRACE:
            tkclass = "RBRACE";
            break;
        default:
            tkclass = "ERROR";
            std::cerr << "Error branch in convert::tkcode2output!" << std::endl;
    }
    return tkclass;
}

config::TokenCode text2token(const string& original)
{
    string token = toLower(original);
    if (token == "const")
        return config::CONSTTK;
    else if (token == "int")
        return config::INTTK;
    else if (token == "char")
        return config::CHARTK;
    else if (token == "void")
        return config::VOIDTK;
    else if (token == "main")
        return config::MAINTK;
    else if (token == "if")
        return config::IFTK;
    else if (token == "else")
        return config::ELSETK;
    else if (token == "switch")
        return config::SWITCHTK;
    else if (token == "case")
        return config::CASETK;
    else if (token == "default")
        return config::DEFAULTTK;
    else if (token == "while")
        return config::WHILETK;
    else if (token == "for")
        return config::FORTK;
    else if (token == "scanf")
        return config::SCANFTK;
    else if (token == "printf")
        return config::PRINTFTK;
    else if (token == "return")
        return config::RETURNTK;
    else if (token == "+")
        return config::PLUS;
    else if (token == "-")
        return config::MINU;
    else if (token == "*")
        return config::MULT;
    else if (token == "/")
        return config::DIV;
    else if (token == "<")
        return config::LSS;
    else if (token == "<=")
        return config::LEQ;
    else if (token == ">")
        return config::GRE;
    else if (token == ">=")
        return config::GEQ;
    else if (token == "==")
        return config::EQL;
    else if (token == "!=")
        return config::NEQ;
    else if (token == ":")
        return config::COLON;
    else if (token == "=")
        return config::ASSIGN;
    else if (token == ";")
        return config::SEMICN;
    else if (token == ",")
        return config::COMMA;
    else if (token == "(")
        return config::LPARENT;
    else if (token == ")")
        return config::RPARENT;
    else if (token == "[")
        return config::LBRACK;
    else if (token == "]")
        return config::RBRACK;
    else if (token == "{")
        return config::LBRACE;
    else if (token == "}")
        return config::RBRACE;
    else
    {
        std::cerr << "Error branch in convert::text2token!" << std::endl;
        return config::ERROR; // ERROR
    }
}
