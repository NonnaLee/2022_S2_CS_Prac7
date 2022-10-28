#ifndef COMPILERPARSER_H
#define COMPILERPARSER_H

#include <vector>
#include <exception>

#include "ParseTree.h"
#include "Token.h"

class CompilerParser {
    // --- JIA Added ---
    private:
        std::vector<Token*> _tokens;
        Token* _token;
        Token* _tokenPrevious;
        int _index = 0;
        void NextToken();
        ParseTree* TokenToParseTree(ParseTree*);
        ParseTree* AddUntill(char*, std::vector<char*>, bool includeLast);
        bool IsAVariableType(Token* token);
    // -----------------
    public:
        CompilerParser(std::vector<Token*> tokens);

        ParseTree* compileProgram();
        ParseTree* compileClass();
        ParseTree* compileClassVarDec();
        ParseTree* compileSubroutine();
        ParseTree* compileParameterList();
        ParseTree* compileSubroutineBody();
        ParseTree* compileVarDec();

        ParseTree* compileStatements();
        ParseTree* compileLet();
        ParseTree* compileIf();
        ParseTree* compileWhile();
        ParseTree* compileDo();
        ParseTree* compileReturn();

        ParseTree* compileExpression();
        ParseTree* compileTerm();
        ParseTree* compileExpressionList();
};

class ParseException : public std::exception {
    public:
        const char* what();
};

#endif /*COMPILERPARSER_H*/