//
// Created by JacquesdeH on 2020/10/2.
//

#include <iostream>

#include "core_syntactic_Syntactic.h"
#include "functional_strext.h"

syntactic::Syntactic::Syntactic(const string &fOut, PeekableQueue* _queue, symbol::SymbolManager* _symbolManager,
                                error::ErrorManager* _errorManager, semantic::Semantic* _semanticGenerator)
{
    this->cacheLast = Token(config::EMPTY);
    this->printer = new Printer(fOut);
    this->queue = _queue;
    this->symbolManager = _symbolManager;
    this->errorManager = _errorManager;
    this->semanticGenerator = _semanticGenerator;
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
    cacheLast = queue->peek();
    queue->pop();
}

void syntactic::Syntactic::_printAndNext()
{
    printer->printToken(_cur());
    _next();
}

bool syntactic::Syntactic::_isComeFirstThan(const config::TokenCode &tkcode1, const config::TokenCode &tkcode2) const
{
    // tkcode1 exists before tkcode2 occurred
    int pos = 1;
    while (!queue->peek(pos).isToken(config::EMPTY))
    {
        if (queue->peek(pos).isToken(tkcode1))
            return true;
        if (queue->peek(pos).isToken(tkcode2))
            return false;
        pos++;
    }
    return false;
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

    printer->printComponent("程序");
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
            // ErrorManager ;
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectSemicnAtConstVarDeclarationEnd,
                                      "Expect ; at the end of const declaration");
            // no skip
        }
        else
            _printAndNext();
    } while (_cur().isToken(config::CONSTTK));

    printer->printComponent("常量说明");
}

void syntactic::Syntactic::parseConstDeclaration()
{
    config::DataType dataType;
    // char | int
    if (!_cur().isValuedType())
    {
        // TODO: ErrorManager
    }
    dataType = (_cur().getTkcode() == config::INTTK) ? config::DataType::INT : config::DataType::CHAR;
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
        if (config::DataType::CHAR == dataType)
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
        int unifiedValue = (dataType == config::DataType::CHAR) ? (int) charValue : intValue;
        // SymbolManager
        if (symbolManager->hasSymbolInScope(idenfr.getTkvalue()))
        {
            // ErrorManager with dup name with const
            errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::DuplicatedName,
                                      "Duplicated const idenfr");
            // no skip
        }
        else
        {
            symbolManager->declareSymbol(idenfr.getTkvalue(), symbol::Info(
                    config::SymbolType::CONST, dataType, idenfr.getRow()));
            semanticGenerator->addMIR(config::CONST_IR, idenfr.getTkvalue(), toString(dataType), toString(1));
            semanticGenerator->addMIR(config::MOVE_IR, idenfr.getTkvalue(), toString(unifiedValue));
        }
        isFirst = false;
    } while (_cur().isToken(config::COMMA));

    printer->printComponent("常量定义");
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

    printer->printComponent("整数");
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

    printer->printComponent("无符号整数");
}

void syntactic::Syntactic::parseVarIllustration()
{
    // ＜变量定义＞;{＜变量定义＞;} start with ＜类型标识符＞＜标识符＞
    // should differ from following {＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞ by '(' at token 3
    // BUT ＜复合语句＞::=［＜常量说明＞］［＜变量说明＞］＜语句列＞ troubles to need more specification
    do
    {
        parseVarDeclaration();
        // ;
        if (!_cur().isToken(config::SEMICN))
        {
            // ErrorManager ;
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectSemicnAtConstVarDeclarationEnd,
                                      "Expect ; at the end of var declaration");
            // no skip
        }
        else
            _printAndNext();
    } while (_cur().isValuedType() && queue->peek(2).isToken(config::IDENFR) &&
                !queue->peek(3).isToken(config::LPARENT));

    printer->printComponent("变量说明");
}

void syntactic::Syntactic::parseVarDeclaration()
{
    // ＜变量定义无初始化＞|＜变量定义及初始化＞ by '=' should occur before ';'
    // ＜变量定义及初始化＞
    if (_isComeFirstThan(config::ASSIGN, config::SEMICN))
    {
        parseVarDeclarationInitialized();
    }
    // ＜变量定义无初始化＞
    else
    {
        parseVarDeclarationUninitialized();
    }

    printer->printComponent("变量定义");
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
        int totElements = 1;
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
            if (!_cur().isToken(config::INTCON))
            {
                // ErrorManager
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArraySubIndexTypeNotInt,
                                          "In declare array with init Sub is not int");
                _skipUntil({config::RBRACK}, config::stopwordsToken);
                dimLim[dim] = 0;
            }
            else
                parseUnsigned(dimLim[dim]);
            totElements *= dimLim[dim];
            // ]
            if (!_cur().isToken(config::RBRACK))
            {
                // ErrorManager
                errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRBrackAtArrayDeclaration,
                                          "Expect ] at array declaration for var without init");
                // no skip
            }
            else
                _printAndNext();
            // increase dim
            dim++;
        }
        // Update SymbolManager
        if (symbolManager->hasSymbolInScope(idenfr.getTkvalue()))
        {
            // ErrorManager with dup idenfr in var declaration
            errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::DuplicatedName,
                                      "Duplicated name in var declaration without init");
            // no skip
        }
        else
        {
            symbolManager->declareSymbol(idenfr.getTkvalue(), symbol::Info(
                    config::SymbolType::VAR, dataType, idenfr.getRow(), dim, dimLim[0], dimLim[1]));
            semanticGenerator->addMIR(config::VAR_IR, idenfr.getTkvalue(), toString(dataType), toString(totElements));
        }
        isFirst = false;
    } while (_cur().isToken(config::COMMA));

    printer->printComponent("变量定义无初始化");
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
    int totElements = 1;
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
        if (!_cur().isToken(config::INTCON))
        {
            // ErrorManager
            errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArraySubIndexTypeNotInt,
                                      "In declare array with init Sub is not int");
            _skipUntil({config::RBRACK}, config::stopwordsToken);
            dimLim[dim] = 0;
        }
        else
            parseUnsigned(dimLim[dim]);
        totElements *= dimLim[dim];
        // ]
        if (!_cur().isToken(config::RBRACK))
        {
            // ErrorManager
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRBrackAtArrayDeclaration,
                                      "Expect ] at array declaration for var with init");
            // no skip
        }
        else
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
            if (_cur().isToken(config::LBRACE))
            {
                // ErrorManager with dim mismatch
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                          "Array init count mismatch for dim=0");
                _skipUntil({config::SEMICN}, config::stopwordsToken);
                goto ArrayInitDimSwitchEnd;
            }
            // ＜常量＞
            bool isInt = false;
            int _value = 0;
            parseConstant(_value, isInt);
            if (isInt != (dataType == config::DataType::INT))
            {
                // ErrorManager
                errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ConstantTypeMismatchInVarDeclarationAndInit,
                                          "Init type mismatch with array template at dim=0");
                // no skip
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
                // ErrorManager with dim mismatch
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                          "Array init count mismatch for dim=1 with less {");
                _skipUntil({config::SEMICN}, config::stopwordsToken);
                goto ArrayInitDimSwitchEnd;
            }
            _printAndNext();
            if (_cur().isToken(config::LBRACE))
            {
                // ErrorManager with dim mismatch
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                          "Array init count mismatch for dim=1 with more {");
                _skipUntil({config::SEMICN}, config::stopwordsToken);
                goto ArrayInitDimSwitchEnd;
            }
            // ＜常量＞{,＜常量＞}
            bool isFirst = true;
            bool isInt = false;
            int _value = 0;
            do
            {
                Token enterToken = _cur();
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
                    // ErrorManager
                    errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ConstantTypeMismatchInVarDeclarationAndInit,
                                              "Init type mismatch with array template at dim=1");
                    // no skip
                }
                params.push_back(_value);
                // check size more than expected
                if (params.size() > dimLim[0])
                {
                    // ErrorManager
                    errorManager->insertError(enterToken.getRow(), enterToken.getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                              "Array init count more than expected at dim=1");
                    _skipUntil({config::SEMICN}, config::stopwordsToken, true);
                    goto ArrayInitDimSwitchEnd;
                }
                isFirst = false;
            } while (_cur().isToken(config::COMMA));
            // check  size less than expected
            if (params.size() < dimLim[0])
            {
                // ErrorManager
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                          "Array init count less than expected at dim=1");
                _skipUntil({config::SEMICN}, config::stopwordsToken, true);
                goto ArrayInitDimSwitchEnd;
            }
            // }
            if (!_cur().isToken(config::RBRACE))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
        }
            break;
        // '{''{'＜常量＞{,＜常量＞}'}'{,'{'＜常量＞{,＜常量＞}'}'}'}'
        case 2:
        {
            // {
            if (!_cur().isToken(config::LBRACE))
            {
                // ErrorManager with dim mismatch
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                          "Array init count mismatch for dim=2 with less {");
                _skipUntil({config::SEMICN}, config::stopwordsToken);
                goto ArrayInitDimSwitchEnd;
            }
            _printAndNext();
            // ,'{'＜常量＞{,＜常量＞}'}'
            int lastSize;
            bool isFirst0 = true;
            do
            {
                lastSize = params.size();
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
                    // ErrorManager with dim mismatch
                    errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                              "Array init count mismatch for dim=2 with less {");
                    _skipUntil({config::SEMICN}, config::stopwordsToken);
                    goto ArrayInitDimSwitchEnd;
                }
                _printAndNext();
                if (_cur().isToken(config::LBRACE))
                {
                    // ErrorManager with dim mismatch
                    errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                              "Array init count mismatch for dim=2 with more {");
                    _skipUntil({config::SEMICN}, config::stopwordsToken);
                    goto ArrayInitDimSwitchEnd;
                }

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
                        // ErrorManager
                        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ConstantTypeMismatchInVarDeclarationAndInit,
                                                  "Init type mismatch with array template at dim=2");
                        // no skip// TODO: ErrorManager
                    }
                    params.push_back(_value);
                } while (_cur().isToken(config::COMMA));
                // check dimLim[1]
                if (dimLim[1] != (params.size() - lastSize))
                {
                    // ErrorManager
                    errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                              "dimLim[1] mismatch in array init");
                    _skipUntil({config::SEMICN}, config::stopwordsToken, true);
                    goto ArrayInitDimSwitchEnd;
                }
                // }
                if (!_cur().isToken(config::RBRACE))
                {
                    // TODO: ErrorManager
                }
                _printAndNext();

                isFirst0 = false;
            } while (_cur().isToken(config::COMMA));
            // check dimLim[0]
            if (dimLim[0] * dimLim[1] != params.size())
            {
                // ErrorManager
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArrayInitMismatchWithTemplate,
                                          "dimLim[0] mismatch in array init");
                _skipUntil({config::SEMICN}, config::stopwordsToken, true);
                goto ArrayInitDimSwitchEnd;
            }
            // }
            if (!_cur().isToken(config::RBRACE))
            {
                // TODO: ErrorManager
            }
            _printAndNext();
        }
            break;
        default:
            std::cerr << "Unexpected dim in parseVarDeclarationInitialized() with dim = " << dim << std::endl;
    }
    ArrayInitDimSwitchEnd:
    // Update SymbolManager
    if (symbolManager->hasSymbolInScope(idenfr.getTkvalue()))
    {
        // ErrorManager with dup idenfr in var declaration
        errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::DuplicatedName,
                                  "Duplicated name in var declaration");
        // no skip
    }
    else
    {
        symbolManager->declareSymbol(idenfr.getTkvalue(), symbol::Info(
                config::SymbolType::VAR, dataType, idenfr.getRow(), dim, dimLim[0], dimLim[1]));
        semanticGenerator->addMIR(config::IRCode::VAR_IR, idenfr.getTkvalue(), toString(dataType), toString(totElements));
        for (int idx = 0; idx < params.size(); idx++)
        {
            const int _value = params[idx];
            semanticGenerator->addMIR(config::IRCode::STORE_IR, toString(_value), idenfr.getTkvalue(), toString(idx));
        }
    }
    printer->printComponent("变量定义及初始化");
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

    printer->printComponent("字符串");
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

    printer->printComponent("常量");
}

void syntactic::Syntactic::parseCaseList(bool & hasReturned, config::DataType insideFuncAndType,
                                         config::DataType declaredDataType)
{
    // ＜情况子语句＞{＜情况子语句＞}
    do
    {
        // ＜情况子语句＞
        parseCaseSubStatement(hasReturned, insideFuncAndType, declaredDataType);
    } while (_cur().isToken(config::CASETK));

    printer->printComponent("情况表");
}

void syntactic::Syntactic::parseCaseSubStatement(bool & hasReturned, config::DataType insideFuncAndType,
                                                 config::DataType declaredDataType)
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
    if (isInt != (declaredDataType == config::DataType::INT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ConstantTypeMismatchInSwitchCase,
                                  "Case constant type mismatch with declaration");
        // no skip
    }
    // :
    if (!_cur().isToken(config::COLON))
    {
        // TODO: ErrorMAnager
    }
    _printAndNext();
    // ＜语句＞
    parseStatement(hasReturned, insideFuncAndType);

    printer->printComponent("情况子语句");
}

void syntactic::Syntactic::parseDefault(bool & hasReturned, config::DataType insideFuncAndType)
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
    parseStatement(hasReturned, insideFuncAndType);

    printer->printComponent("缺省");
}

void syntactic::Syntactic::parseStatementList(bool & hasReturned, config::DataType insideFuncAndType)
{
    // ｛＜语句＞｝by '}' following
    while (!_cur().isToken(config::RBRACE))
    {
        // ＜语句＞
        parseStatement(hasReturned, insideFuncAndType);
    }

    printer->printComponent("语句列");
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
    Token idenfr = _cur();
    if (!symbolManager->hasSymbolInAll(idenfr.getTkvalue()))
    {
        // ErrorManager
        errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::UndefinedName,
                                  "Undefined idenfr in Read Statement");
        // no skip
    }
    else
    {
        if (symbolManager->getInfoInAll(idenfr.getTkvalue()).isSymbolTypeOf(config::SymbolType::CONST))
        {
            // ErrorManager
            errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::ModifyConstWithScanf,
                                      "Change CONST idenfr in scanf statement");
            // no skip
        }
        if (!(symbolManager->getInfoInAll(idenfr.getTkvalue()).isDimOf(0)))
        {
            // TODO: ErrorManager with dim in scanf not 0
        }
    }
    _printAndNext();
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtScanf,
                                  "Expect ) at scanf");
        // no skip
    }
    else
        _printAndNext();
    // semantic
    if (semanticGenerator->noError())
    {
        semanticGenerator->addMIR(config::READ_IR, idenfr.getTkvalue(),
                                  toString(symbolManager->getInfoInAll(idenfr.getTkvalue()).queryDataType()));
    }

    printer->printComponent("读语句");
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
    bool hasString = false;
    bool hasExpr = false;
    string strName;
    string exprTemp;
    config::DataType dataType;
    if (_cur().isToken(config::STRCON))
    {
        // ＜字符串＞
        parseString(_str);
        hasString = true;
        strName = semanticGenerator->genString(_str);
        // ,＜表达式＞
        if (_cur().isToken(config::COMMA))
        {
            // ,
            _printAndNext();
            // ＜表达式＞
            dataType = parseExpression(exprTemp);
            hasExpr = true;
        }
    }
    else
    {
        // ＜表达式＞
        dataType = parseExpression(exprTemp);
        hasExpr = true;
    }
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtPrintf,
                                  "Expect ) at printf");
        // no skip
    }
    else
        _printAndNext();
    // semantic
    if (semanticGenerator->noError())
    {
        if (hasString)
        {
            semanticGenerator->addMIR(config::STRING_IR, strName);
        }
        if (hasExpr)
        {
            semanticGenerator->addMIR(config::WRITE_IR, exprTemp, toString(dataType));
        }
        semanticGenerator->addMIR(config::WRITE_IR, toString((int) '\n'), toString(config::DataType::CHAR));
    }

    printer->printComponent("写语句");
}

void syntactic::Syntactic::parseSwitchStatement(bool & hasReturned, config::DataType insideFuncAndType)
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
    string exprTemp;
    config::DataType exprDataType = parseExpression(exprTemp);
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtSwitch,
                                  "Expect ) at switch()");
        // no skip
    }
    else
        _printAndNext();
    // {
    if (!_cur().isToken(config::LBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜情况表＞
    // TODO: semantic
    parseCaseList(hasReturned, insideFuncAndType, exprDataType);
    // ＜缺省＞
    if (!_cur().isToken(config::DEFAULTTK))
    {
        // ErrorManager
        errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ExpectDefaultStatement,
                                  "Expect <default> in switch statement");
        // no skip
    }
    else
        parseDefault(hasReturned, insideFuncAndType);
    // }
    if (!_cur().isToken(config::RBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();

    printer->printComponent("情况语句");
}

void syntactic::Syntactic::parseReturnStatement(bool & hasReturned, config::DataType insideFuncAndType)
{
    // return
    Token returnToken = _cur();
    if (!_cur().isToken(config::RETURNTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ['('＜表达式＞')'] by '[' existence
    // has a ( at least [ return ( ]
    if (_cur().isToken(config::LPARENT))
    {
        // (
        _printAndNext();
        // return(); type with empty expr
        if (_cur().isToken(config::RPARENT))
        {
            if (config::isValuedDataType(insideFuncAndType))
            {
                // ErrorManager
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ValuedFunctionWithParents,
                                          "Valued func with return();");
                // no skip
            }
            else if (config::isVoidDataType(insideFuncAndType))
            {
                // ErrorManager
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::VoidFunctionWithParents,
                                          "Void func with return();");
                // no skip
            }
        }
        // ＜表达式＞
        else
        {
            string exprTemp;
            config::DataType exprDataType = parseExpression(exprTemp);
            if (config::isValuedDataType(insideFuncAndType))
            {
                if (exprDataType != insideFuncAndType)
                {
                    // ErrorManager with mismatch return value type
                    errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ValuedFunctionReturnTypeMismatch,
                                              "Valued func with mismatch return value type");
                    // no skip
                }
            }
            else if (config::isVoidDataType(insideFuncAndType))
            {
                // ErrorManager
                errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::VoidFunctionWithValue,
                                          "Void func with valued return");
                // no skip
            }
        }
        // )
        if (!_cur().isToken(config::RPARENT))
        {
            // ErrorManager
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtReturn,
                                      "Expect ) at return() or return(<Expr>)");
            // no skip
        }
        else
            _printAndNext();
    }
    // return;
    else if (_cur().isToken(config::SEMICN))
    {
        if (config::isValuedDataType(insideFuncAndType))
        {
            // ErrorManager valued has return;
            errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ValuedFunctionWithVoid,
                                      "Valued func with return;");
            // no skip
        }
    }
    // TODO: semantic

    printer->printComponent("返回语句");
    hasReturned = true;
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
    if (!symbolManager->hasSymbolInAll(idenfr.getTkvalue()))
    {
        // ErrorManager without defined idenfr
        errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::UndefinedName,
                                  "Undefined idenfr in assignment statement");
        // no skip
    }
    else
    {
        if (symbolManager->getInfoInAll(idenfr.getTkvalue()).isSymbolTypeOf(config::SymbolType::CONST))
        {
            // ErrorManager
            errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::ModifyConstWithAssign,
                                      "Change CONST idenfr in assignment statement");
            // no skip
        }
    }
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
        string exprTemp;
        config::DataType exprDataType = parseExpression(exprTemp);
        if (exprDataType != config::DataType::INT)
        {
            // ErrorManager
            errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArraySubIndexTypeNotInt,
                                      "Assign use array sub is not int");
            _skipUntil({config::RBRACK}, config::stopwordsToken);
        }
        // ]
        if (!_cur().isToken(config::RBRACK))
        {
            // ErrorManager
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRBrackAtArrayUseInAssignLeft,
                                      "Expect ] at array use in assign statement left");
            // no skip
        }
        else
            _printAndNext();
        // TODO: semantic array assign
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
    string exprR;
    parseExpression(exprR);
    // semantic
    if (dim == 0)
    {
        // single var
        semanticGenerator->addMIR(config::MOVE_IR, idenfr.getTkvalue(), exprR);
    }
    else
    {
        // TODO: semantic array use
    }

    printer->printComponent("赋值语句");
}

void syntactic::Syntactic::parseConditionStatement(bool & hasReturned, config::DataType insideFuncAndType)
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
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtIf,
                                  "Expect ) at if()");
        // no skip
    }
    else
        _printAndNext();
    // ＜语句＞
    parseStatement(hasReturned, insideFuncAndType);
    // ［else＜语句＞］
    if (_cur().isToken(config::ELSETK))
    {
        // else
        _printAndNext();
        // ＜语句＞
        parseStatement(hasReturned, insideFuncAndType);
    }

    printer->printComponent("条件语句");
}

void syntactic::Syntactic::parseLoopStatement(bool & hasReturned, config::DataType insideFuncAndType)
{
    // for
    if (_cur().isToken(config::FORTK))
    {
        parseForStatement(hasReturned, insideFuncAndType);
    }
    // while
    else
    {
        parseWhileStatement(hasReturned, insideFuncAndType);
    }

    printer->printComponent("循环语句");
}

void syntactic::Syntactic::parseWhileStatement(bool & hasReturned, config::DataType insideFuncAndType)
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
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtWhile,
                                  "Expect ) at while()");
        // no skip
    }
    else
        _printAndNext();
    // ＜语句＞
    parseStatement(hasReturned, insideFuncAndType);
}

void syntactic::Syntactic::parseForStatement(bool & hasReturned, config::DataType insideFuncAndType)
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
    if (!symbolManager->hasSymbolInAll(idenfr1.getTkvalue()))
    {
        // ErrorManager
        errorManager->insertError(idenfr1.getRow(), idenfr1.getColumn(), config::ErrorType::UndefinedName,
                                  "Undefined idenfr in for of 1");
        // no skip
    }
    _printAndNext();
    // =
    if (!_cur().isToken(config::ASSIGN))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜表达式＞
    string exprTemp;
    parseExpression(exprTemp);
    // ;
    if (!_cur().isToken(config::SEMICN))
    {
        // ErrorManager ;
        errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ExpectSemicnInFor,
                                  "In <For> statement expect a ; at first position");
        // no skip
    }
    else
        _printAndNext();
    // ＜条件＞
    parseCondition();
    // ;
    if (!_cur().isToken(config::SEMICN))
    {
        // ErrorManager ;
        errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ExpectSemicnInFor,
                                  "In <For> statement expect a ; at second position");
        // no skip
    }
    else
        _printAndNext();
    // ＜标识符＞
    Token idenfr2;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr2 = _cur();
    if (!symbolManager->hasSymbolInAll(idenfr2.getTkvalue()))
    {
        // ErrorManager
        errorManager->insertError(idenfr2.getRow(), idenfr2.getColumn(), config::ErrorType::UndefinedName,
                                  "Undefined idenfr in for 2");
        // no skip
    }
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
    if (!symbolManager->hasSymbolInAll(idenfr3.getTkvalue()))
    {
        // ErrorManager
        errorManager->insertError(idenfr3.getRow(), idenfr3.getColumn(), config::ErrorType::UndefinedName,
                                  "Undefined idenfr in for 3");
        // no skip
    }
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
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtFor,
                                  "Expect ) at for()");
        // no skip
    }
    else
        _printAndNext();
    // ＜语句＞
    parseStatement(hasReturned, insideFuncAndType);
}

void syntactic::Syntactic::parseCondition()
{
    config::DataType tmpDataType;
    Token tmpToken;
    // ＜表达式＞
    tmpToken = _cur();
    string exprTempL;
    tmpDataType = parseExpression(exprTempL);
    if (tmpDataType != config::DataType::INT)
    {
        // ErrorManager with not INT of condition
        errorManager->insertError(tmpToken.getRow(), tmpToken.getColumn(), config::ErrorType::IllegalTypeInCondition,
                                  "Not int type in condition cmp");
        // no skip
    }
    // ＜关系运算符＞
    if (!_cur().isCmpOp())
    {
        // TODO: ErrorManager
    }
    Token cmpOp = _cur();
    _printAndNext();
    // ＜表达式＞
    tmpToken = _cur();
    string exprTempR;
    tmpDataType = parseExpression(exprTempR);
    if (tmpDataType != config::DataType::INT)
    {
        // ErrorManager with not INT of condition
        errorManager->insertError(tmpToken.getRow(), tmpToken.getColumn(), config::ErrorType::IllegalTypeInCondition,
                                  "Not int type in condition cmp");
        // no skip
    }

    printer->printComponent("条件");
}

void syntactic::Syntactic::parseStepLength(int &_step)
{
    // ＜无符号整数＞
    parseUnsigned(_step);

    printer->printComponent("步长");
}

vector<config::DataType> syntactic::Syntactic::parseParameterDeclarationList()
{
    vector<config::DataType> retParamDataTypeList;
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
            // update ret list of dataType
            retParamDataTypeList.push_back(dataType);
            // Update SymbolManager
            if (symbolManager->hasSymbolInScope(idenfr.getTkvalue()))
            {
                // ErrorManager with duplicated name
                errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::DuplicatedName,
                                          "Duplicated parameter");
                // no skip
            }
            else
                symbolManager->declareSymbol(idenfr.getTkvalue(), symbol::Info(
                        config::SymbolType::VAR, dataType, idenfr.getRow()));
            isFirst = false;
        } while (_cur().isToken(config::COMMA));
    }

    printer->printComponent("参数表");
    return retParamDataTypeList;
}

void syntactic::Syntactic::parseParameterValueList(const vector<config::DataType> & _paramDataTypeList)
{
    int curIndexOfParam = 0;
    // ＜空＞ by next token after to be ')'
    if (!_isExprFirst())
    {
        // do nothing
    }
    // ＜表达式＞{,＜表达式＞}
    else
    {
        bool isFirst = true;
        do
        {
            Token curToken = _cur();
            // ,
            if (!isFirst)
            {
                if (!_cur().isToken(config::COMMA))
                {
                    // TODO: ErrorManager
                }
                _printAndNext();
            }
            // ＜表达式＞
            string exprTemp;
            config::DataType curDataType = parseExpression(exprTemp);
            // deal with new value, >= coz ++ is guaranteed
            if (curIndexOfParam >= _paramDataTypeList.size())
            {
                // ErrorManager
                errorManager->insertError(curToken.getRow(), curToken.getColumn(), config::ErrorType::FunctionParamCountMismatch,
                                          "Param Value count more than expected");
                _skipUntil({config::COMMA, config::RPARENT}, config::stopwordsToken);
            }
            else if (curDataType != config::DataType::DATA_DEFAULT && curDataType != _paramDataTypeList[curIndexOfParam])
            {
                // ErrorManager
                errorManager->insertError(curToken.getRow(), curToken.getColumn(), config::ErrorType::FunctionParamTypeMismatch,
                                          "Param Value type mismatch");
                _skipUntil({config::COMMA, config::RPARENT}, config::stopwordsToken);
            }
            isFirst = false;
            curIndexOfParam++;
        } while (_cur().isToken(config::COMMA));
    }
    // possibly param count is less, with < is better to avoid multiple error
    if (curIndexOfParam < _paramDataTypeList.size())
    {
        // ErrorManager
        errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::FunctionParamCountMismatch,
                                  "Param Value count less than expected");
        // no skip
    }

    printer->printComponent("值参数表");
}

void syntactic::Syntactic::parseCompoundStatement(bool & hasReturned, config::DataType insideFuncAndType)
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
    parseStatementList(hasReturned, insideFuncAndType);

    printer->printComponent("复合语句");
}

void syntactic::Syntactic::parseMainFunction()
{
    Token idenfr;
    // void
    if (!_cur().isToken(config::VOIDTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // main
    if (!_cur().isToken(config::MAINTK))
    {
        // TODO: ErrorManager
    }
    idenfr = _cur();
    _printAndNext();
    // SymbolManager consider main function as well
    if (symbolManager->hasSymbolInScope(idenfr.getTkvalue()))
    {
        // ErrorManager
        // main name duplicated
        errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::DuplicatedName,
                                  "Main function name duplicate error");
        // no skip
    }
    symbolManager->declareSymbol(idenfr.getTkvalue(), symbol::Info(
            config::SymbolType::FUNCTION, config::DataType::VOID, idenfr.getRow()));
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // SymbolManager into a new scope
    symbolManager->pushNewScope();
    // semantic
    semanticGenerator->newProc();
    semanticGenerator->addMIR(config::FUNC_IR, idenfr.getTkvalue(), toString(config::DataType::VOID));
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtMain,
                                  "Expect ) at Main");
        // no skip
    }
    else
        _printAndNext();
    // {
    if (!_cur().isToken(config::LBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜复合语句＞
    bool hasReturned = false;
    parseCompoundStatement(hasReturned, config::VOID);
    // }
    if (!_cur().isToken(config::RBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // SymbolManager recover to original scope
    symbolManager->popCurScope();
    // semantic
    semanticGenerator->addMIR(config::FUNCEND_IR);

    printer->printComponent("主函数");
}

config::DataType syntactic::Syntactic::parseDeclarationHead(string &_idenfr)
{
    // must be inside the Valued Function Declaration
    // int | char
    config::DataType dataType;
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
    _idenfr = idenfr.getTkvalue();
    _printAndNext();
    // update SymbolManager with Valued function declaration
    if (symbolManager->hasSymbolInScope(idenfr.getTkvalue()))
    {
        // ErrorManager duplicated idenfr
        errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::DuplicatedName,
                                  "Duplicated name of valued function");
        // no skip
    }
    else
        symbolManager->declareSymbol(idenfr.getTkvalue(),
                                     symbol::Info(config::SymbolType::FUNCTION,
                                                  dataType, idenfr.getRow()));

    printer->printComponent("声明头部");
    return dataType;
}

void syntactic::Syntactic::parseFunctionValuedDeclaration()
{
    // ＜声明头部＞
    string funcIdenfr;
    config::DataType funcDataType = parseDeclarationHead(funcIdenfr);
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // SymbolManager new scope
    symbolManager->pushNewScope();
    // ＜参数表＞
    const vector<config::DataType> paramDataTypeList = parseParameterDeclarationList();
    symbolManager->getInfoFromLastScope(funcIdenfr).logFuncParam(paramDataTypeList);
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtFunctionDeclaration,
                                  "Expect ) at valued func declaration");
        // no skip
    }
    else
        _printAndNext();
    // {
    if (!_cur().isToken(config::LBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜复合语句＞
    bool hasReturned = false;
    parseCompoundStatement(hasReturned, funcDataType);
    if (!hasReturned)
    {
        // ErrorManager
        errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ValuedFunctionWithoutReturn,
                                  "Valued func has not returned");
        // no skip
    }
    // }
    if (!_cur().isToken(config::RBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // SymbolManager recover original scope
    symbolManager->popCurScope();

    printer->printComponent("有返回值函数定义");
}

void syntactic::Syntactic::parseFunctionVoidDeclaration()
{
    // void
    if (!_cur().isToken(config::VOIDTK))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜标识符＞
    Token idenfr;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr = _cur();
    _printAndNext();
    // update SymbolManager with Valued function declaration
    if (symbolManager->hasSymbolInScope(idenfr.getTkvalue()))
    {
        // ErrorManager with duplicated name
        errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::DuplicatedName,
                                  "Duplicated name in void function");
        // no skip
    }
    else
        symbolManager->declareSymbol(idenfr.getTkvalue(),
                                     symbol::Info(config::SymbolType::FUNCTION,
                                                  config::DataType::VOID, idenfr.getRow()));
    // (
    if (!_cur().isToken(config::LPARENT))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // SymbolManager new scope
    symbolManager->pushNewScope();
    // ＜参数表＞
    const vector<config::DataType> paramDataTypeList = parseParameterDeclarationList();
    symbolManager->getInfoFromLastScope(idenfr.getTkvalue()).logFuncParam(paramDataTypeList);
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtFunctionDeclaration,
                                  "Expect ) at void func declaration");
        // no skip
    }
    else
        _printAndNext();
    // {
    if (!_cur().isToken(config::LBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // ＜复合语句＞
    bool hasReturned = false;
    parseCompoundStatement(hasReturned, config::DataType::VOID);
    // }
    if (!_cur().isToken(config::RBRACE))
    {
        // TODO: ErrorManager
    }
    _printAndNext();
    // SymbolManager recover original scope
    symbolManager->popCurScope();

    printer->printComponent("无返回值函数定义");
}

void syntactic::Syntactic::parseFunctionValuedCallStatement()
{
    // ＜标识符＞
    Token idenfr;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr = _cur();
    if (!symbolManager->hasSymbolInAll(idenfr.getTkvalue()))
    {
        // ErrorManager
        errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::UndefinedName,
                                  "Undefined Valued function call");
        // no skip
    }
    else if (!symbolManager->getInfoInAll(idenfr.getTkvalue()).isValuedFunction())
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
    // ＜值参数表＞
    const vector<config::DataType> & _paramDataTypeList = (symbolManager->hasSymbolInAll(idenfr.getTkvalue())) ?
                                                          symbolManager->getInfoInAll(idenfr.getTkvalue()).queryParamDataTypeListOfFunction() :
                                                          vector<config::DataType>();
    parseParameterValueList(_paramDataTypeList);
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtFunctionCall,
                                  "Expect ) at valued func call");
        // no skip
    }
    else
        _printAndNext();

    printer->printComponent("有返回值函数调用语句");
}

void syntactic::Syntactic::parseFunctionVoidCallStatement()
{
    // ＜标识符＞
    Token idenfr;
    if (!_cur().isToken(config::IDENFR))
    {
        // TODO: ErrorManager
    }
    idenfr = _cur();
    if (!symbolManager->hasSymbolInAll(idenfr.getTkvalue()))
    {
        // ErrorManager
        errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::UndefinedName,
                                  "Undefined Void function call");
        // no skip
    }
    else if (!symbolManager->getInfoInAll(idenfr.getTkvalue()).isVoidFunction())
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
    // ＜值参数表＞
    const vector<config::DataType> & _paramDataTypeList = (symbolManager->hasSymbolInAll(idenfr.getTkvalue())) ?
                                                          symbolManager->getInfoInAll(idenfr.getTkvalue()).queryParamDataTypeListOfFunction() :
                                                          vector<config::DataType>();
    parseParameterValueList(_paramDataTypeList);
    // )
    if (!_cur().isToken(config::RPARENT))
    {
        // ErrorManager
        errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtFunctionCall,
                                  "Expect ) at void func call");
        // no skip
    }
    else
        _printAndNext();

    printer->printComponent("无返回值函数调用语句");
}

void syntactic::Syntactic::parseStatement(bool & hasReturned, config::DataType insideFuncAndType)
{
    // ＜空＞;
    if (_cur().isToken(config::SEMICN))
    {
        // ;
        if (!_cur().isToken(config::SEMICN))
        {
            // ErrorManager ;
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectSemicnInStatementEnd,
                                      "In <Null>; statement expect a ;");
            // no skip
        }
        else
            _printAndNext();
    }
    // '{'＜语句列＞'}'
    else if (_cur().isToken(config::LBRACE))
    {
        // {
        _printAndNext();
        // ＜语句列＞
        parseStatementList(hasReturned, insideFuncAndType);
        // }
        if (!_cur().isToken(config::RBRACE))
        {
            // TODO: ErrorManager
        }
        _printAndNext();
    }
    // ＜循环语句＞
    else if (_cur().isLoopKeyword())
    {
        // ＜循环语句＞
        parseLoopStatement(hasReturned, insideFuncAndType);
    }
    // ＜条件语句＞
    else if (_cur().isTokens({config::IFTK}))
    {
        // ＜条件语句＞
        parseConditionStatement(hasReturned, insideFuncAndType);
    }
    // ＜读语句＞;
    else if (_cur().isTokens({config::SCANFTK}))
    {
        // ＜读语句＞
        parseReadStatement();
        // ;
        if (!_cur().isToken(config::SEMICN))
        {
            // ErrorManager ;
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectSemicnInStatementEnd,
                                      "In <Read>; statement expect a ;");
            // no skip
        }
        else
            _printAndNext();
    }
    // ＜写语句＞;
    else if (_cur().isTokens({config::PRINTFTK}))
    {
        // ＜写语句＞
        parsePrintStatement();
        // ;
        if (!_cur().isToken(config::SEMICN))
        {
            // ErrorManager ;
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectSemicnInStatementEnd,
                                      "In <Write>; statement expect a ;");
            // no skip
        }
        else
            _printAndNext();
    }
    // ＜返回语句＞;
    else if (_cur().isTokens({config::RETURNTK}))
    {
        // ＜返回语句＞
        parseReturnStatement(hasReturned, insideFuncAndType);
        // ;
        if (!_cur().isToken(config::SEMICN))
        {
            // ErrorManager ;
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectSemicnInStatementEnd,
                                      "In <Return>; statement expect a ;");
            // no skip
        }
        else
            _printAndNext();
    }
    // ＜情况语句＞
    else if (_cur().isTokens({config::SWITCHTK}))
    {
        // ＜情况语句＞
        parseSwitchStatement(hasReturned, insideFuncAndType);
    }
    // ＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞; | ＜赋值语句＞;
    else if (_cur().isTokens({config::IDENFR}))
    {
        // ＜有返回值函数调用语句＞ | ＜无返回值函数调用语句＞ by token 2 with '(' to be function call
        if (queue->peek(2).isToken(config::LPARENT))
        {
            Token idenfr = _cur();
            if (!symbolManager->hasSymbolInAll(idenfr.getTkvalue()))
            {
                // ErrorManager
                errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::UndefinedName,
                                          "Undefined function name call");
                _skipUntil({config::SEMICN}, config::stopwordsToken);
            }
            else
            {
                // ＜有返回值函数调用语句＞
                if (symbolManager->getInfoInAll(idenfr.getTkvalue()).isValuedFunction())
                {
                    // ＜有返回值函数调用语句＞
                    parseFunctionValuedCallStatement();
                }
                    // ＜无返回值函数调用语句＞
                else if (symbolManager->getInfoInAll(idenfr.getTkvalue()).isVoidFunction())
                {
                    // ＜无返回值函数调用语句＞
                    parseFunctionVoidCallStatement();
                }
                else
                {
                    // TODO: ErrorManager
                    // not a defined function, partly processed before
                }
            }
        }
        // ＜赋值语句＞
        else
        {
            // ＜赋值语句＞
            parseAssignStatement();
        }
        // ; commonly parsed!
        if (!_cur().isToken(config::SEMICN))
        {
            // ErrorManager ;
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectSemicnInStatementEnd,
                                      "In <FuncCallValued>;|<FuncCallVoid>;|<Assign>; statement expect a ;");
            // no skip
        }
        else
            _printAndNext();
    }
    // error
    else
    {
        // TODO: ErrorManager
    }

    printer->printComponent("语句");
}

config::DataType syntactic::Syntactic::parseExpression(string & temp)
{
    bool hasError = false;
    config::DataType retDataType = config::DataType::CHAR;
    bool isFirst = true;
    string sumTemp;
    do
    {
        // not a single term must be an int expr
        if (!isFirst)
            retDataType = config::DataType::INT;
        int flag = 1;
        // +|- with first to be optional
        if (_cur().isPlusMinusOp())
        {
            if (_cur().isToken(config::MINU))
                flag = -1;
            _printAndNext();
            // with a +|- must be an expr
            retDataType = config::DataType::INT;
        }
        else if (!isFirst)
        {
            // TODO: ErrorManager where +|- is not optional
            hasError = true;
        }
        // ＜项＞
        string termTemp;
        config::DataType termDataType = parseTerm(termTemp);
        // update if term is not a char
        if (termDataType == config::DataType::INT)
            retDataType = config::DataType::INT;
        else if (termDataType == config::DataType::DATA_DEFAULT)
            hasError = true;
        // deal with this term related expression
        if (isFirst)
        {
            sumTemp = termTemp;
            if (flag == -1)
            {
                sumTemp = semanticGenerator->genTemp();
                semanticGenerator->addMIR(config::IRCode::MINUS_IR, sumTemp, toString(0), termTemp);
            }
        }
        else
        {
            string newTemp = semanticGenerator->genTemp();
            semanticGenerator->addMIR((flag==1) ? config::IRCode::ADD_IR : config::IRCode::MINUS_IR, newTemp, sumTemp, termTemp);
            sumTemp = newTemp;
        }
        isFirst = false;
    } while (_cur().isPlusMinusOp());

    printer->printComponent("表达式");
    temp = sumTemp;
    return hasError ? config::DataType::DATA_DEFAULT : retDataType;
}

config::DataType syntactic::Syntactic::parseTerm(string & temp)
{
    bool hasError = false;
    config::DataType retDataType = config::DataType::CHAR;
    bool isFirst = true;
    string prodTemp;
    bool isMultOp;
    do
    {
        // *|/
        if (!isFirst)
        {
            if (!_cur().isMultDivOp())
            {
                // TODO: ErrorManager
                hasError = true;
            }
            isMultOp = _cur().isToken(config::TokenCode::MULT);
            _printAndNext();
            // with * and / must be an expr
            retDataType = config::DataType::INT;
        }
        // ＜因子＞
        string factorTemp;
        config::DataType factorDataType = parseFactor(factorTemp);
        // Factor is a int type
        if (factorDataType == config::DataType::INT)
            retDataType = config::DataType::INT;
        else if (factorDataType == config::DataType::DATA_DEFAULT)
            hasError = true;
        // deal with factor related term
        if (isFirst)
        {
            prodTemp = factorTemp;
        }
        else
        {
            string newTemp = semanticGenerator->genTemp();
            semanticGenerator->addMIR((isMultOp) ? config::IRCode::MULT_IR : config::IRCode::DIV_IR, newTemp, prodTemp, factorTemp);
            prodTemp = newTemp;
        }
        isFirst = false;
    } while (_cur().isMultDivOp());

    printer->printComponent("项");
    temp = prodTemp;
    return hasError ? config::DataType::DATA_DEFAULT : retDataType;
}

config::DataType syntactic::Syntactic::parseFactor(string & temp)
{
    bool hasError = false;
    config::DataType retDataType = config::DataType::DATA_DEFAULT;
    // ＜字符＞
    if (_cur().isToken(config::CHARCON))
    {
        // ＜字符＞
        char _ch = _cur().getTkvalue()[0];
        _printAndNext();
        temp = semanticGenerator->genTemp();
        semanticGenerator->addMIR(config::MOVE_IR, temp, toString((int) _ch));
        // CASE 1 of single CHARCON is a char
        retDataType = config::DataType::CHAR;
    }
    // ＜整数＞
    else if (_cur().isTokens({config::PLUS, config::MINU, config::INTCON}))
    {
        // ＜整数＞
        int _int = 0;
        parseInteger(_int);
        temp = semanticGenerator->genTemp();
        semanticGenerator->addMIR(config::MOVE_IR, temp, toString(_int));
        // not a char for sure
        retDataType = config::DataType::INT;
    }
    // '('＜表达式＞')'
    else if (_cur().isToken(config::LPARENT))
    {
        // (
        _printAndNext();
        // ＜表达式＞
        string exprTemp;
        config::DataType exprDataType = parseExpression(exprTemp);
        if (exprDataType == config::DataType::DATA_DEFAULT)
            hasError = true;
        // )
        if (!_cur().isToken(config::RPARENT))
        {
            // ErrorManager
            errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRParentAtExpression,
                                      "Expect ) at (<Expr>)");
            // no skip
            hasError = true;
        }
        else
            _printAndNext();
        temp = exprTemp;
        // a recursive expr must be a int type
        retDataType = config::DataType::INT;
    }
    // ＜标识符＞ ｜ ＜标识符＞'['＜表达式＞']' | ＜标识符＞'['＜表达式＞']''['＜表达式＞']' | ＜有返回值函数调用语句＞ by token 2
    else if (_cur().isToken(config::IDENFR))
    {
        // ＜有返回值函数调用语句＞
        if (queue->peek(2).isToken(config::LPARENT))
        {
            Token idenfr = _cur();
            // ＜有返回值函数调用语句＞
            errorManager->watchErrors();
            parseFunctionValuedCallStatement();
            // return type based on function call
            if (errorManager->queryWatch())
                hasError = true;
            if (!symbolManager->hasSymbolInAll(idenfr.getTkvalue()))
                hasError = true;
            else
                retDataType = symbolManager->getInfoInAll(idenfr.getTkvalue()).queryDataType();
            // TODO: valued function related push moveret temp
        }
        // ＜标识符＞ ｜ ＜标识符＞'['＜表达式＞']' | ＜标识符＞'['＜表达式＞']''['＜表达式＞']'
        else
        {
            Token idenfr;
            int dim = 0;
            // ＜标识符＞
            if (!_cur().isToken(config::IDENFR))
            {
                // TODO: ErrorManager
                hasError = true;
            }
            idenfr = _cur();
            if (!symbolManager->hasSymbolInAll(idenfr.getTkvalue()))
            {
                // ErrorManager
                errorManager->insertError(idenfr.getRow(), idenfr.getColumn(), config::ErrorType::UndefinedName,
                                          "Undefined idenfr in Factor");
                // no skip
                hasError = true;
            }
            _printAndNext();
            // '['＜表达式＞']'
            while (_cur().isToken(config::LBRACK))
            {
                if (dim >= 2)
                {
                    // TODO: ErrorManager
                    hasError = true;
                    break;
                }
                // [
                _printAndNext();
                // ＜表达式＞
                string exprTemp;
                config::DataType exprDataType = parseExpression(exprTemp);
                if (exprDataType == config::DataType::DATA_DEFAULT)
                    hasError = true;
                if (exprDataType != config::DataType::INT)
                {
                    // ErrorManager
                    errorManager->insertError(_cur().getRow(), _cur().getColumn(), config::ErrorType::ArraySubIndexTypeNotInt,
                                              "In factor call array sub is not int");
                    _skipUntil({config::RBRACK}, config::stopwordsToken);
                    hasError = true;
                }
                // ]
                if (!_cur().isToken(config::RBRACK))
                {
                    // ErrorManager
                    errorManager->insertError(_last().getRow(), _last().getColumn(), config::ErrorType::ExpectRBrackAtArrayUseInFactor,
                                              "Expect ] at array use in factor");
                    // no skip
                    hasError = true;
                }
                else
                    _printAndNext();
                // increase dim
                dim++;
            }
            // retDataType based on idenfr type
            if (symbolManager->hasSymbolInAll(idenfr.getTkvalue()))
                retDataType = symbolManager->getInfoInAll(idenfr.getTkvalue()).queryDataType();
            else
                retDataType = config::DATA_DEFAULT;
            // semantic
            if (dim == 0)
            {
                temp = semanticGenerator->genTemp();
                semanticGenerator->addMIR(config::MOVE_IR, temp, idenfr.getTkvalue());
            }
            else
            {
                // TODO: array use related
            }
        }
    }
    // error
    else
    {
        // TODO: ErrorManager
        retDataType = config::DATA_DEFAULT;
        hasError = true;
    }

    printer->printComponent("因子");
    return hasError ? config::DataType::DATA_DEFAULT : retDataType;
}

void syntactic::Syntactic::_skipUntil(const std::unordered_set<config::TokenCode> &successors,
                                      const std::unordered_set<config::TokenCode> &stopwords, const bool &hardSkipCur)
{
    std::unordered_set<config::TokenCode> wordset;
    for (const auto & word : successors)
        wordset.insert(word);
    for (const auto & word : stopwords)
        wordset.insert(word);
    if (!hardSkipCur && (wordset.find(_cur().getTkcode()) != wordset.end()))
        return;
    do
    {
        _printAndNext();
    } while (wordset.find(_cur().getTkcode()) == wordset.end());
}

Token syntactic::Syntactic::_last()
{
    return cacheLast;
}

bool syntactic::Syntactic::_isExprFirst()
{
    std::unordered_set<config::TokenCode> targetTokens = {
            config::PLUS, config::MINU, config::IDENFR, config::INTCON, config::CHARCON, config::LPARENT};
    return targetTokens.find(_cur().getTkcode()) != targetTokens.end();
}

