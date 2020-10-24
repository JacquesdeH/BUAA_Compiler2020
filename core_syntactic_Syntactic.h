//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_CORE_SYNTACTIC_SYNTACTIC_H
#define PROJECT_CORE_SYNTACTIC_SYNTACTIC_H

#include "utils_PeekableQueue.h"
#include "core_syntactic_Printer.h"
#include "symbol_SymbolManager.h"
#include "ErrorManager.h"

namespace syntactic
{
    class Syntactic
    {
    private:
        Token cacheLast;
        PeekableQueue* queue;
        Printer* printer;
        symbol::SymbolManager* symbolManager;
        error::ErrorManager* errorManager;

    public:
        Syntactic(const string& fOut, PeekableQueue* _queue, symbol::SymbolManager* _symbolManager, error::ErrorManager* _errorManager);
        ~Syntactic();

    private:
        Token _cur();
        Token _last();
        void _next();
        void _printAndNext();
        bool _isComeFirstThan(const config::TokenCode& tkcode1, const config::TokenCode& tkcode2) const;
        void _skipUntil(const std::unordered_set<config::TokenCode> & successors,
                        const std::unordered_set<config::TokenCode> & stopwords, const bool & keepCur = false);

    public:
        void parseProgram();

    private:
        // Illustration
        void parseConstIllustration();
        void parseVarIllustration();
        // Function
        void parseMainFunction();
        void parseFunctionValuedDeclaration();
        void parseFunctionVoidDeclaration();
        void parseFunctionValuedCallStatement();
        void parseFunctionVoidCallStatement();
        config::DataType parseDeclarationHead(string& _idenfr);
        // Statement
        void parseCompoundStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseStatementList(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseAssignStatement();
        void parseConditionStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseLoopStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseWhileStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseForStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseReadStatement();
        void parsePrintStatement();
        void parseSwitchStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseReturnStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        // Expression
        config::DataType parseExpression();
        config::DataType parseTerm();
        config::DataType parseFactor();
        // Parameter
        vector<config::DataType> parseParameterDeclarationList();
        void parseParameterValueList(const vector<config::DataType> & _paramDataTypeList);
        // Conditional component
        void parseCaseList(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT,
                           config::DataType declaredDataType = config::DATA_DEFAULT);
        void parseCaseSubStatement(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT,
                                   config::DataType declaredDataType = config::DATA_DEFAULT);
        void parseDefault(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseCondition();
        void parseStepLength(int& _step);
        // Const & Var
        void parseConstDeclaration();
        void parseVarDeclaration();
        void parseVarDeclarationUninitialized();
        void parseVarDeclarationInitialized();
        // Values
        void parseInteger(int& _integer);
        void parseUnsigned(int& _unsigned);
        void parseString(string& _str);
        void parseConstant(int& _value, bool& _isInteger);
    };
}


#endif //PROJECT_CORE_SYNTACTIC_SYNTACTIC_H
