//
// Created by JacquesdeH on 2020/9/25.
//

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include <unordered_map>
#include <initializer_list>
#include <unordered_set>
#include <vector>
#include <set>

typedef unsigned int uint;

namespace config
{
    const bool USE_STDERR = false;

    const bool PRINT_LEXIC = false;
    const bool PRINT_SYNTACTIC = false;
    const bool PRINT_ERROR_TUPLE = true;
    const bool PRINT_DETAILED_ERROR = false & PRINT_ERROR_TUPLE;

    const bool FORCE_UNCASED = true;

    const bool OPTIM_BACKEND_MATHOP_BOTH_CONSTANT = true;
    const bool OPTIM_BACKEND_MULT_SHIFT = true;
    const bool OPTIM_BACKEND_DIV_SHIFT = true;

    const int INF = 0x3f3f3f3f;

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

    const std::set<TokenCode> uncasedTokenCodes = {
            IDENFR,
            CONSTTK, INTTK, CHARTK, VOIDTK, MAINTK, IFTK, ELSETK, SWITCHTK, CASETK, DEFAULTTK,
            WHILETK, FORTK, SCANFTK, PRINTFTK, RETURNTK
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
        CONST, VAR, FUNCTION,
        SYMBOL_DEFAULT
    };

    enum DataType
    {
        INT, CHAR, VOID,
        DATA_DEFAULT
    };

    bool isValuedDataType(const DataType & dataType);

    bool isVoidDataType(const DataType & dataType);

    enum ErrorType
    {
        // Code A
        IllegalLetterChar, IllegalLetterString, EmptyCharOrString, CharLengthError,
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

    const std::unordered_map<std::string, std::unordered_set<ErrorType>> errorType2Code = {
            {"a", {IllegalLetterChar, IllegalLetterString, EmptyCharOrString, CharLengthError}},
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

    const std::unordered_set<char> stopwordsChar = {EOF, '\r', '\n'};
    const std::unordered_set<TokenCode> stopwordsToken = {SEMICN};

    enum IRCode
    {
        ADD_IR, MINUS_IR, MULT_IR, DIV_IR,
        LOAD_IR, STORE_IR,
        MOVE_IR,
        BEQ_IR, BNE_IR, BLE_IR, BLT_IR, JUMP_IR,
        PARA_IR, PUSH_IR, DEPUSH_IR, CALL_IR, RET_IR, MOVERET_IR,
        READ_IR, WRITE_IR, STRING_IR,
        SETLABEL_IR,
        EXIT_IR
    };

    const std::set<IRCode> blockOutIRCodes = {BEQ_IR, BNE_IR, BLE_IR, BLT_IR, JUMP_IR, CALL_IR, RET_IR};
    const std::set<IRCode> labeledBranchJump = {BEQ_IR, BNE_IR, BLE_IR, BLT_IR, JUMP_IR};

    bool isBlockOutIRCode(const IRCode &_irCode);
    bool isLabeledBranchJump(const IRCode &_ircode);

    const int atomSizeChar = 1;
    const int atomSizeInt = 4;
    const int atomSizeTemp = 4;
    const int atomSizePush = 4;
    const int atomSizeReg = 4;
    const int bitsSLLInt = 2;

    const std::string tempHead = "___Temp__";
    const std::string labelHead = "___Label__";
    const std::string stringHead = "___String__";
    const std::string procHead = "___Proc__";
    const std::string globalHead = "___Global__";
    const std::string localHead = "___Local__";

    const std::string mainLabel = procHead + "main";

    bool isTemp(const std::string & _str);
    bool isLabel(const std::string & _str);
    bool isString(const std::string & _str);
    bool isProc(const std::string & _str);
    bool isGlobal(const std::string & _str);
    bool isLocal(const std::string & _str);
    bool isNumeric(const std::string & _str);
    int toNumeric(const std::string & _str);
    bool isAbsPower2(const int & _num);
    int toAbsLog2(const int & _num);

    const std::string sep = "\t";

    const std::vector<std::string> InitLocalRegs = {
            "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
//            "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    };
    const std::initializer_list<std::string> ParamRegs = {"$a0", "$a1", "$a2", "$a3"};
    const std::string zeroReg = "$zero";
    const std::string stackReg = "$sp";
    const std::string globalReg = "$gp";
    const std::string frameReg = "$fp";
    const std::string returnAddrReg = "$ra";

    const std::string gpInit = "0x10010000";
    const int paramRegCntLimit = 4;

    const int _readInt = 5;
    const int _readChar = 12;
    const int _printInt = 1;
    const int _printChar = 11;
    const int _printString = 4;

    const int _printQuadWidth = 25;
}

#endif //PROJECT_CONFIG_H
