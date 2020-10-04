//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_SYNTACTIC_SYNTACTIC_H
#define PROJECT_SYNTACTIC_SYNTACTIC_H

#include "utils_PeekableQueue.h"
#include "syntactic_Printer.h"

namespace syntactic
{
    class Syntactic
    {
    private:
        PeekableQueue* queue;
        Printer* printer;

    public:
        Syntactic(const string& fOut, PeekableQueue* _queue);
        ~Syntactic();

    private:
        Token _cur();
        void _next();
        void _printAndNext();

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
        void parseDeclarationHead(string& _idenfr);
        // Statement
        void parseCompoundStatement();
        void parseStatementList();
        void parseStatement();
        void parseAssignStatement();
        void parseConditionStatement();
        void parseLoopStatement();
        void parseWhileStatement();
        void parseForStatement();
        void parseReadStatement();
        void parsePrintStatement();
        void parseSwitchStatement();
        void parseReturnStatement();
        // Expression
        void parseExpression();
        void parseTerm();
        void parseFactor();
        // Parameter
        void parseParameterDeclarationList();
        void parseParameterValueList();
        // Conditional component
        void parseCaseList();
        void parseCaseSubStatement();
        void parseDefault();
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


#endif //PROJECT_SYNTACTIC_SYNTACTIC_H
