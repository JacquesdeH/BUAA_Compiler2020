//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_CORE_SYNTACTIC_SYNTACTIC_H
#define PROJECT_CORE_SYNTACTIC_SYNTACTIC_H

#include "utils_PeekableQueue.h"
#include "core_syntactic_Printer.h"
#include "symbol_SymbolManager.h"
#include "error_ErrorManager.h"
#include "core_semantic_Semantic.h"

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
        semantic::Semantic* semanticGenerator;

    public:
        Syntactic(const string& fOut, PeekableQueue* _queue, symbol::SymbolManager* _symbolManager,
                  error::ErrorManager* _errorManager, semantic::Semantic* _semanticGenerator);
        ~Syntactic();

    private:
        Token _cur();
        Token _last();
        void _next();
        void _printAndNext();
        bool _isComeFirstThan(const config::TokenCode& tkcode1, const config::TokenCode& tkcode2) const;
        bool _isExprFirst();
        void _skipUntil(const std::unordered_set<config::TokenCode> & successors,
                        const std::unordered_set<config::TokenCode> & stopwords, const bool & hardSkipCur = false);

    public:
        void parseProgram();

    private:
        // Illustration
        void parseConstIllustration(const bool & useGlobal = false);
        void parseVarIllustration(const bool & useGlobal = false);
        // Function
        void parseMainFunction();
        void parseFunctionValuedDeclaration();
        void parseFunctionVoidDeclaration();
        void parseFunctionValuedCallStatement(string & temp);
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
        config::DataType parseExpression(string & temp);
        config::DataType parseTerm(string & temp);
        config::DataType parseFactor(string & temp);
        // Parameter
        vector<config::DataType> parseParameterDeclarationList();
        vector<std::string> parseParameterValueList(const vector<config::DataType> & _paramDataTypeList);
        // Conditional component
        std::vector<std::pair<int, std::vector<inter::Quad> > > parseCaseList(
                bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT,
                config::DataType declaredDataType = config::DATA_DEFAULT);
        std::pair<int, std::vector<inter::Quad> > parseCaseSubStatement(
                bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT,
                config::DataType declaredDataType = config::DATA_DEFAULT);
        std::vector<inter::Quad> parseDefault(bool & hasReturned, config::DataType insideFuncAndType = config::DATA_DEFAULT);
        void parseCondition(config::TokenCode &_operator, std::string &_exprL, std::string &_exprR);
        void parseStepLength(int& _step);
        // Const & Var
        void parseConstDeclaration(const bool & useGlobal = false);
        void parseVarDeclaration(const bool & useGlobal = false);
        void parseVarDeclarationUninitialized(const bool & useGlobal = false);
        void parseVarDeclarationInitialized(const bool & useGlobal = false);
        // Values
        void parseInteger(int& _integer);
        void parseUnsigned(int& _unsigned);
        void parseString(string& _str);
        void parseConstant(int& _value, bool& _isInteger);
    };
}


#endif //PROJECT_CORE_SYNTACTIC_SYNTACTIC_H
