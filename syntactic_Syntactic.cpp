//
// Created by JacquesdeH on 2020/10/2.
//

#include <iostream>

#include "syntactic_Syntactic.h"
#include "functional_strext.h"

syntactic::Syntactic::Syntactic(const string &fOut, PeekableQueue* _queue)
{
    this->printer = new Printer(fOut);
    this->queue = _queue;
}

syntactic::Syntactic::~Syntactic()
{
    delete this->printer;
}

Token syntactic::Syntactic::_cur()
{
    return queue->peek();
}

void syntactic::Syntactic::_next()
{
    queue->pop();
}

void syntactic::Syntactic::_printAndNext()
{
    printer->printToken(_cur());
    _next();
}

void syntactic::Syntactic::parseProgram()
{
    // [<常量说明>]
    if (_cur().isToken(config::CONSTTK))
        parseConstIllustration();
    // [＜变量说明＞] by '(' at token 3
    if (!queue->peek(3).isToken(config::LPARENT))
        parseVarIllustration();
    // {＜有返回值函数定义＞|＜无返回值函数定义＞} by MAINTK at 2
    while (!queue->peek(2).isToken(config::MAINTK))
    {
        // ＜有返回值函数定义＞ by token at 1 of char int void
        if (_cur().isValuedType())
            parseFunctionValuedDeclaration();
        // ＜无返回值函数定义＞
        else
            parseFunctionVoidDeclaration();
    }
    // ＜主函数＞
    parseMainFunction();

    printer->printComponent("＜主函数＞");
}

void syntactic::Syntactic::parseConstIllustration()
{
    do
    {
        // const
        if (!_cur().isToken(config::CONSTTK))
        {
            // TODO: ErrorManager
        }
        _printAndNext();
        // ＜常量定义＞
        parseConstDeclaration();
        // ;
        if (!_cur().isToken(config::SEMICN))
        {
            // TODO: ErrorManager
        }
        _printAndNext();
    } while (_cur().isToken(config::CONSTTK));

    printer->printComponent("＜常量说明＞");
}

void syntactic::Syntactic::parseConstDeclaration()
{
    config::TokenCode dataType;
    // char | int
    if (!_cur().isValuedType())
    {
        // TODO: ErrorManager
    }
    dataType = _cur().getTkcode();
    _printAndNext();
    // ＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} | ＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
    bool isFirst = true;
    do
    {
        Token idenfr;
        char charValue;
        int intValue;
        // ,
        if (!isFirst)
        {
            if (!_cur().isToken(config::COMMA))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
        }
        // ＜标识符＞
        if (!_cur().isToken(config::IDENFR))
        {
            // TODO: ErrorManager
        }
        idenfr = _cur();
        _printAndNext();
        // =
        if (!_cur().isToken(config::ASSIGN))
        {
            // TODO: ErrorManager
        }
        _printAndNext();
        // ＜字符＞
        if (config::CHARTK == dataType)
        {
            if (!_cur().isToken(config::CHARCON))
            {
                // TODO: ErrorManager
            }
            charValue = _cur().getTkvalue()[0];
            _printAndNext();
        }
        // ＜整数＞
        else
        {
            parseInteger(intValue);
        }
        isFirst = false;
    } while (_cur().isToken(config::COMMA));

    printer->printComponent("＜常量定义＞");
}

void syntactic::Syntactic::parseInteger(int &_integer)
{
    int flag = 1;
    // ［＋｜－］
    if (_cur().isPlusMinusOp())
    {
        if (_cur().isToken(config::MINU))
            flag = -1;
        _printAndNext();
    }
    // ＜无符号整数＞
    parseUnsigned(_integer);
    _integer *= flag;

    printer->printComponent("＜整数＞");
}

void syntactic::Syntactic::parseUnsigned(int &_unsigned)
{
    // ＜无符号整数＞
    if (!_cur().isToken(config::INTCON))
    {
        // TODO: ErrorManager
    }
    _unsigned = str2int(_cur().getTkvalue());
    _printAndNext();

    printer->printComponent("＜无符号整数＞");
}

void syntactic::Syntactic::parseVarIllustration()
{
    // ＜变量定义＞;{＜变量定义＞;} start with ＜类型标识符＞＜标识符＞
    // should differ from following {＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞ by '(' at token 3
    do
    {
        parseVarDeclaration();
        // ;
        if (!_cur().isToken(config::SEMICN))
        {
            // TODO: ErrorManager
        }
        _printAndNext();
    } while (queue->peek(3).isToken(config::LPARENT));

    printer->printComponent("＜变量说明＞");
}

void syntactic::Syntactic::parseVarDeclaration()
{
    // ＜变量定义无初始化＞|＜变量定义及初始化＞ by '=' at token 3 6 9
    // ＜变量定义及初始化＞
    if (queue->peek(3).isToken(config::ASSIGN) ||
        queue->peek(6).isToken(config::ASSIGN) ||
        queue->peek(9).isToken(config::ASSIGN))
    {
        parseVarDeclarationInitialized();
    }
    // ＜变量定义无初始化＞
    else
    {
        parseVarDeclarationUninitialized();
    }

    printer->printComponent("＜变量定义＞");
}

void syntactic::Syntactic::parseVarDeclarationUninitialized()
{
    config::DataType dataType;
    // ＜类型标识符＞
    if (!_cur().isValuedType())
    {
        // TODO: ErrorManager
    }
    dataType = (_cur().isToken(config::CHARTK)) ? config::DataType::CHAR : config::DataType::INT;
    _printAndNext();
    // (＜标识符＞|＜标识符＞'['＜无符号整数＞']'|＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']')
    // {,(＜标识符＞|＜标识符＞'['＜无符号整数＞']'|＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']')}
    bool isFirst = true;
    do
    {
        Token idenfr;
        int dim = 0;
        int dimLim[2] = {-1, -1};
        // ,
        if (!isFirst)
        {
            if (!_cur().isToken(config::COMMA))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
        }
        // ＜标识符＞
        if (!_cur().isToken(config::IDENFR))
        {
            // TODO: ErrorManager
        }
        idenfr = _cur();
        _printAndNext();
        // '['＜无符号整数＞']'
        while (_cur().isToken(config::LBRACK))
        {
            // dim limit up to 2-dim arrays
            if (dim >= 2)
            {
                // TODO: ErrorManager
                break;
            }
            // [
            _printAndNext();
            // ＜无符号整数＞
            parseUnsigned(dimLim[dim]);
            // ]
            if (!_cur().isToken(config::RBRACK))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
            // increase dim
            dim++;
        }
        // TODO: Update SymbolManager
        isFirst = false;
    } while (_cur().isToken(config::COMMA));

    printer->printComponent("＜变量定义无初始化＞");
}

void syntactic::Syntactic::parseVarDeclarationInitialized()
{
    config::DataType dataType;
    // ＜类型标识符＞
    if (!_cur().isValuedType())
    {
        // TODO: ErrorManager
    }
    dataType = (_cur().isToken(config::CHARTK)) ? config::DataType::CHAR : config::DataType::INT;
    _printAndNext();
    // ＜标识符＞
    Token idenfr;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr = _cur();
    _printAndNext();
    // '['＜无符号整数＞']'
    int dim = 0;
    int dimLim[2] = {-1, -1};
    while (_cur().isToken(config::LBRACK))
    {
        // dim limit up to 2-dim arrays
        if (dim >= 2)
        {
            // TODO: ErrorManager
            break;
        }
        // [
        _printAndNext();
        // ＜无符号整数＞
        parseUnsigned(dimLim[dim]);
        // ]
        if (!_cur().isToken(config::RBRACK))
        {
            // TODO: ErrorManager
        }
        _printAndNext();
        // increase dim
        dim++;
    }
    // =
    if (!_cur().isToken(config::ASSIGN))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜常量＞ | '{'＜常量＞{,＜常量＞}'}' | '{''{'＜常量＞{,＜常量＞}'}'{,'{'＜常量＞{,＜常量＞}'}'}'}'
    vector<int> params;
    switch (dim)
    {
        // ＜常量＞
        case 0:
        {
            // ＜常量＞
            bool isInt = false;
            int _value = 0;
            parseConstant(_value, isInt);
            if (isInt != (dataType == config::DataType::INT))
            {
                // TODO: ErrorManager
            }
            params.push_back(_value);
        }
            break;
        // '{'＜常量＞{,＜常量＞}'}'
        case 1:
        {
            // {
            if (!_cur().isToken(config::LBRACE))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
            // ＜常量＞{,＜常量＞}
            bool isFirst = true;
            bool isInt = false;
            int _value = 0;
            do
            {
                // ,
                if (!isFirst)
                {
                    if (!_cur().isToken(config::COMMA))
                    {
                        // TODO: ErrorManager
                    }
                    _printAndNext();
                }
                // ＜常量＞
                parseConstant(_value, isInt);
                if (isInt != (dataType == config::DataType::INT))
                {
                    // TODO: ErrorManager
                }
                params.push_back(_value);
                isFirst = false;
            } while (_cur().isToken(config::COMMA));
            // }
            if (!_cur().isToken(config::RBRACE))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
            // check param numbers with dimLim
            if (params.size() != dimLim[0])
            {
                // TODO: ErrorManager
            }
        }
            break;
        // '{''{'＜常量＞{,＜常量＞}'}'{,'{'＜常量＞{,＜常量＞}'}'}'}'
        case 2:
        {
            // {
            if (!_cur().isToken(config::LBRACE))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
            // ,'{'＜常量＞{,＜常量＞}'}'
            int lastSize = params.size();
            bool isFirst0 = true;
            do
            {
                // ,
                if (!isFirst0)
                {
                    if (!_cur().isToken(config::COMMA))
                    {
                        // TODO: ErrorManager
                    }
                    _printAndNext();
                }
                // {
                if (!_cur().isToken(config::LBRACE))
                {
                    // TODO: ErrorManager
                }
                _printAndNext();

                // ,＜常量＞
                bool isFirst1 = true;
                int _value = 0;
                bool isInt = false;
                do
                {
                    // ,
                    if (!isFirst1)
                    {
                        if (!_cur().isToken(config::COMMA))
                        {
                            // TODO: ErrorManager
                        }
                        _printAndNext();
                    }
                    isFirst1 = false;
                    // ＜常量＞
                    parseConstant(_value, isInt);
                    if (isInt != (dataType == config::DataType::INT))
                    {
                        // TODO: ErrorManager
                    }
                    params.push_back(_value);
                } while (_cur().isToken(config::COMMA));
                // }
                if (!_cur().isToken(config::RBRACE))
                {
                    // TODO: ErrorManager
                }
                _printAndNext();
                // check dimLim[1]
                if (dimLim[1] != (params.size() - lastSize))
                {
                    // TODO: ErrorManager
                }
                isFirst0 = false;
            } while (_cur().isToken(config::COMMA));
            // }
            if (!_cur().isToken(config::RBRACE))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
            // check dimLim[0]
            if (dimLim[0] * dimLim[1] != params.size())
            {
                // TODO: ErrorManager
            }
        }
            break;
        default:
            std::cerr << "Unexpected dim in parseVarDeclarationInitialized() with dim = " << dim << std::endl;
    }
    // TODO: Update SymbolManager

    printer->printComponent("＜变量定义及初始化＞");
}

void syntactic::Syntactic::parseString(string &_str)
{
    // STRCON
    if (!_cur().isToken(config::STRCON))
    {
        // TODO: ErrorManaer
    }
    _str = _cur().getTkvalue();
    _printAndNext();

    printer->printComponent("＜字符串＞");
}

void syntactic::Syntactic::parseConstant(int& _value, bool& _isInteger)
{
    // ＜字符＞
    if (_cur().isToken(config::CHARCON))
    {
        _isInteger = false;
        _value = char2int(_cur().getTkvalue()[0]);
        _printAndNext();
    }
    // ＜整数＞
    else
    {
        _isInteger = true;
        parseInteger(_value);
    }

    printer->printComponent("＜常量＞");
}

void syntactic::Syntactic::parseCaseList()
{
    // ＜情况子语句＞{＜情况子语句＞}
    do
    {
        // ＜情况子语句＞
        parseCaseSubStatement();
    } while (_cur().isToken(config::CASETK));

    printer->printComponent("＜情况表＞");
}

void syntactic::Syntactic::parseCaseSubStatement()
{
    // case
    if (!_cur().isToken(config::CASETK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜常量＞
    int _value;
    bool isInt;
    parseConstant(_value, isInt);
    // :
    if (!_cur().isToken(config::COLON))
    {
        // TODO: ErrorMAnager
    }
    _printAndNext();
    // ＜语句＞
    parseStatement();

    printer->printComponent("＜情况子语句＞");
}

void syntactic::Syntactic::parseDefault()
{
    // default
    if (!_cur().isToken(config::DEFAULTTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // :
    if (!_cur().isToken(config::COLON))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜语句＞
    parseStatement();

    printer->printComponent("＜缺省＞");
}

void syntactic::Syntactic::parseStatementList()
{
    // ｛＜语句＞｝by '}' following
    while (!_cur().isToken(config::RBRACE))
    {
        // ＜语句＞
        parseStatement();
    }

    printer->printComponent("＜语句列＞");
}

void syntactic::Syntactic::parseReadStatement()
{
    // scanf
    if (!_cur().isToken(config::SCANFTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜标识符＞
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    Token token = _cur();
    _printAndNext();
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();

    printer->printComponent("＜读语句＞");
}

void syntactic::Syntactic::parsePrintStatement()
{
    // printf
    if (!_cur().isToken(config::PRINTFTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜字符串＞,＜表达式＞ | ＜字符串＞ | ＜表达式＞ by token 1 with STRCON
    string _str;
    if (_cur().isToken(config::STRCON))
    {
        // ＜字符串＞
        parseString(_str);
        // ,＜表达式＞
        if (_cur().isToken(config::COMMA))
        {
            // ,
            _printAndNext();
            // ＜表达式＞
            parseExpression();
        }
    }
    else
    {
        // ＜表达式＞
        parseExpression();
    }
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();

    printer->printComponent("＜写语句＞");
}

void syntactic::Syntactic::parseSwitchStatement()
{
    // switch
    if (!_cur().isToken(config::SWITCHTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜表达式＞
    parseExpression();
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // {
    if (!_cur().isToken(config::LBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜情况表＞
    parseCaseList();
    // ＜缺省＞
    parseDefault();
    // }
    if (!_cur().isToken(config::RBRACE))
    {
        // TODO: ErrorManager
    }

    printer->printComponent("＜情况语句＞");
}

void syntactic::Syntactic::parseReturnStatement()
{
    // return
    if (!_cur().isToken(config::RETURNTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ['('＜表达式＞')'] by '[' existence
    if (_cur().isToken(config::LPARENT))
    {
        // (
        _printAndNext();
        // ＜表达式＞
        parseExpression();
        // )
        if (!_cur().isToken(config::RPARENT))
        {
            // TODO: ErrorManager
        }
        _printAndNext();
    }

    printer->printComponent("＜返回语句＞");
}

void syntactic::Syntactic::parseAssignStatement()
{
    // ＜标识符＞
    Token idenfr;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr = _cur();
    _printAndNext();
    // '['＜表达式＞']' | '['＜表达式＞']''['＜表达式＞']'
    int dim = 0;
    while (_cur().isToken(config::LBRACK))
    {
        if (dim >= 2)
        {
            // TODO: ErrorManager
            break;
        }
        // [
        _printAndNext();
        // ＜表达式＞
        parseExpression();
        // ]
        if (!_cur().isToken(config::RBRACK))
        {
            // TODO: ErrorManager
        }
        _printAndNext();
        // increase dim
        dim++;
    }
    // TODO: ErrorManager with SymbolManager
    // =
    if (!_cur().isToken(config::ASSIGN))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜表达式＞
    parseExpression();

    printer->printComponent("＜赋值语句＞");
}

void syntactic::Syntactic::parseConditionStatement()
{
    // if
    if (!_cur().isToken(config::IFTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜条件＞
    parseCondition();
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜语句＞
    parseStatement();
    // ［else＜语句＞］
    if (_cur().isToken(config::ELSETK))
    {
        // else
        _printAndNext();
        // ＜语句＞
        parseStatement();
    }

    printer->printComponent("＜条件语句＞");
}

void syntactic::Syntactic::parseLoopStatement()
{
    // for
    if (_cur().isToken(config::FORTK))
    {
        parseForStatement();
    }
    // while
    else
    {
        parseWhileStatement();
    }

    printer->printComponent("＜循环语句＞");
}

void syntactic::Syntactic::parseWhileStatement()
{
    // while
    if (!_cur().isToken(config::WHILETK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜条件＞
    parseCondition();
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜语句＞
    parseStatement();
}

void syntactic::Syntactic::parseForStatement()
{
    // for
    if (!_cur().isToken(config::FORTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜标识符＞
    Token idenfr1;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr1 = _cur();
    _printAndNext();
    // =
    if (!_cur().isToken(config::ASSIGN))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜表达式＞
    parseExpression();
    // ;
    if (!_cur().isToken(config::SEMICN))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜条件＞
    parseCondition();
    // ;
    if (!_cur().isToken(config::SEMICN))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜标识符＞
    Token idenfr2;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr2 = _cur();
    if (idenfr1.getTkvalue() != idenfr2.getTkvalue())
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // =
    if (!_cur().isToken(config::ASSIGN))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜标识符＞
    Token idenfr3;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr3 = _cur();
    if (idenfr1.getTkvalue() != idenfr3.getTkvalue())
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // +|-
    if (!_cur().isPlusMinusOp())
    {
        // TODO: ErrorManager
    }
    int flag = 1;
    if (_cur().isToken(config::MINU))
        flag = -1;
    _printAndNext();
    // ＜步长＞
    int _step = 0;
    parseStepLength(_step);
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜语句＞
    parseStatement();
}

void syntactic::Syntactic::parseCondition()
{
    // ＜表达式＞
    parseExpression();
    // ＜关系运算符＞
    if (!_cur().isCmpOp())
    {
        // TODO: ErrorMAnager
    }
    Token cmpOp = _cur();
    _printAndNext();
    // ＜表达式＞
    parseExpression();

    printer->printComponent("＜条件＞");
}

void syntactic::Syntactic::parseStepLength(int &_step)
{
    // ＜无符号整数＞
    parseUnsigned(_step);

    printer->printComponent("＜步长＞");
}

void syntactic::Syntactic::parseParameterDeclarationList()
{
    // ＜空＞
    if (!_cur().isValuedType())
    {
        // Do nothing
    }
    // ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}
    else
    {
        bool isFirst = true;
        config::DataType dataType;
        Token idenfr;
        do
        {
            // ,
            if (!isFirst)
            {
                if (!_cur().isToken(config::COMMA))
                {
                    // TODO: ErrorManager
                }
                _printAndNext();
            }
            // ＜类型标识符＞
            if (!_cur().isValuedType())
            {
                // TODO: ErrorManager
            }
            dataType = (_cur().isToken(config::CHARTK)) ? config::DataType::CHAR : config::DataType::INT;
            _printAndNext();
            //  ＜标识符＞
            if (!_cur().isToken(config::IDENFR))
            {
                // TODO: ErrorManager
            }
            idenfr = _cur();
            _printAndNext();
            // Update SymbolManager
            isFirst = false;
        } while (_cur().isToken(config::COMMA));
    }

    printer->printComponent("＜参数表＞");
}

void syntactic::Syntactic::parseParameterValueList()
{
    // ＜空＞ by next token after to be ')'
    if (_cur().isToken(config::RPARENT))
    {
        // do nothing
    }
    // ＜表达式＞{,＜表达式＞}
    else
    {
        bool isFirst = true;
        do
        {
            // ,
            if (!isFirst)
            {
                if (!_cur().isToken(config::COMMA))
                {
                    // RODO: ErrorManager
                }
                _printAndNext();
            }
            // ＜表达式＞
            parseExpression();
            // deal with new value
            isFirst = false;
        } while (_cur().isToken(config::COMMA));
    }

    printer->printComponent("＜值参数表＞");
}

void syntactic::Syntactic::parseCompoundStatement()
{
    // ［＜常量说明＞］
    if (_cur().isToken(config::CONSTTK))
    {
        parseConstIllustration();
    }
    // ［＜变量说明＞］
    if (_cur().isValuedType())
    {
        parseVarIllustration();
    }
    // ＜语句列＞
    parseStatementList();

    printer->printComponent("＜复合语句＞");
}


