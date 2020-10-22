//
// Created by JacquesdeH on 2020/9/25.
//

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include <unordered_map>
#include <initializer_list>

typedef unsigned int uint;

namespace config
{
    const bool PRINT_LEXIC = false;
    const bool PRINT_SYNTACTIC = false;
    const bool PRINT_ERROR_TUPLE = true;

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

    const std::unordered_map<TokenCode, std::string> TOKEN2STR = {
            {IDENFR, "IDENFR"}, {INTCON, "INTCON"}, {CHARCON, "CHARCON"},
            {STRCON, "STRCON"}, {CONSTTK, "CONSTTK"}, {INTTK, "INTTK"},
            {CHARTK, "CHARTK"}, {VOIDTK, "VOIDTK"}, {MAINTK, "MAINTK"},
            {IFTK, "IFTK"}, {ELSETK, "ELSETK"}, {SWITCHTK, "SWITCHTK"},
            {CASETK, "CASETK"}, {DEFAULTTK, "DEFAULTTK"}, {WHILETK, "WHILETK"},
            {FORTK, "FORTK"}, {SCANFTK, "SCANFTK"}, {PRINTFTK, "PRINTFTK"},
            {RETURNTK, "RETURNTK"}, {PLUS, "PLUS"}, {MINU, "MINU"},
            {MULT, "MULT"}, {DIV, "DIV"}, {LSS, "LSS"},
            {LEQ, "LEQ"}, {GRE, "GRE"}, {GEQ, "GEQ"},
            {EQL, "EQL"}, {NEQ, "NEQ"}, {COLON, "COLON"},
            {ASSIGN, "ASSIGN"}, {SEMICN, "SEMICN"}, {COMMA, "COMMA"},
            {LPARENT, "LPARENT"}, {RPARENT, "RPARENT"}, {LBRACK, "LBRACK"},
            {RBRACK, "RBRACK"}, {LBRACE, "LBRACE"}, {RBRACE, "RBRACE"},
            {EMPTY, "EMPTY"}
    };

    const std::unordered_map<std::string, TokenCode> STR2TOKEN = {
            {"const", CONSTTK}, {"int", INTTK}, {"char", CHARTK},
            {"void", VOIDTK}, {"main", MAINTK}, {"if", IFTK},
            {"else", ELSETK}, {"switch", SWITCHTK}, {"case", CASETK},
            {"default", DEFAULTTK}, {"while", WHILETK}, {"for", FORTK},
            {"scanf", SCANFTK}, {"printf", PRINTFTK}, {"return", RETURNTK},
            {"+", PLUS}, {"-", MINU}, {"*", MULT},
            {"/", DIV}, {"<", LSS}, {"<=", LEQ},
            {">", GRE}, {">=", GEQ}, {"==", EQL},
            {"!=", NEQ}, {":", COLON}, {"=", ASSIGN},
            {";", SEMICN}, {",", COMMA}, {"(", LPARENT},
            {")", RPARENT}, {"[", LBRACK}, {"]", RBRACK},
            {"{", LBRACE}, {"}", RBRACE}
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

    enum ErrorType
    {
        // Code A
        IllegalChar, EmptyCharOrString, CharLengthError,
        // Code B
        DuplicatedName,
        // Code C
        UndefinedName,
        // Code D
        FunctionParamCountMismatch,
        // Code E
        FunctionParamTypeMismatch,
        // Code F
        IllegalTypeInCondition,
        // Code G
        VoidFunctionWithParents, VoidFunctionWithValue,
        // Code H
        ValuedFunctionWithoutReturn, ValuedFunctionWithVoid, ValuedFunctionWithParents, ValuedFunctionReturnTypeMismatch,
        // Code I
        ArraySubIndexTypeNotInt,
        // Code J
        ModifyConstWithAssign, ModifyConstWithScanf,
        // Code K
        ExpectSemicnInStatementEnd, ExpectSemicnInFor, ExpectSemicnAtConstVarDeclarationEnd,
        // Code L
        ExpectRParentAtFunctionCall, ExpectRParentAtFunctionDeclaration, ExpectRParentAtMain, ExpectRParentAtExpression,
        ExpectRParentAtIf, ExpectRParentAtWhile, ExpectRParentAtFor, ExpectRParentAtSwitch,
        ExpectRParentAtScanf, ExpectRParentAtPrintf, ExpectRParentAtReturn,
        // Code M
        ExpectRBrackAtArrayDeclaration, ExpectRBrackAtArrayUseInFactor, ExpectRBrackAtArrayUseInAssignLeft,
        // Code N
        ArrayInitMismatchWithTemplate,
        // Code O
        ConstantTypeMismatchInVarDeclarationAndInit, ConstantTypeMismatchInSwitchCase,
        // Code P
        ExpectDefaultStatement,
        // Default
        GeneralError
    };

    const std::unordered_map<std::string, std::initializer_list<ErrorType>> errorType2Code = {
            {"a", {IllegalChar, EmptyCharOrString, CharLengthError}},
            {"b", {DuplicatedName}},
            {"c", {UndefinedName}},
            {"d", {FunctionParamCountMismatch}},
            {"e", {FunctionParamTypeMismatch}},
            {"f", {IllegalTypeInCondition}},
            {"g", {VoidFunctionWithParents, VoidFunctionWithValue}},
            {"h", {ValuedFunctionWithoutReturn, ValuedFunctionWithVoid, ValuedFunctionWithParents, ValuedFunctionReturnTypeMismatch}},
            {"i", {ArraySubIndexTypeNotInt}},
            {"j", {ModifyConstWithAssign, ModifyConstWithScanf}},
            {"k", {ExpectSemicnInStatementEnd, ExpectSemicnInFor, ExpectSemicnAtConstVarDeclarationEnd}},
            {"l", {ExpectRParentAtFunctionCall, ExpectRParentAtFunctionDeclaration, ExpectRParentAtMain, ExpectRParentAtExpression,
                          ExpectRParentAtIf, ExpectRParentAtWhile, ExpectRParentAtFor, ExpectRParentAtSwitch,
                          ExpectRParentAtScanf, ExpectRParentAtPrintf, ExpectRParentAtReturn}},
            {"m", {ExpectRBrackAtArrayDeclaration, ExpectRBrackAtArrayUseInFactor, ExpectRBrackAtArrayUseInAssignLeft}},
            {"n", {ArrayInitMismatchWithTemplate}},
            {"o", {ConstantTypeMismatchInVarDeclarationAndInit, ConstantTypeMismatchInSwitchCase}},
            {"p", {ExpectDefaultStatement}},
    };
}

#endif //PROJECT_CONFIG_H
