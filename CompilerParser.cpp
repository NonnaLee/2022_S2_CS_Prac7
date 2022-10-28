#include "CompilerParser.h"

/**
* JIA - ADDED 
*/
void CompilerParser::NextToken()
{
    _index++;
    if (_index == _tokens.size()) {
        _token = NULL;
    }
    else {
        _tokenPrevious = _token;
        _token = _tokens[_index];
    }
}

ParseTree* CompilerParser::TokenToParseTree(ParseTree* parseTreeParent)
{
    auto parseTree = new ParseTree(_token->getType(), _token->getValue());
    parseTreeParent->addChild(parseTree);
    NextToken();
    return parseTree;
}

ParseTree* CompilerParser::AddUntill(char* header, std::vector<char*> stopSymbol, bool includeLast)
{
    ParseTree* parseTree = new ParseTree(header, "");
    while (_token != NULL) {
        
        bool stop = false;
        for (int i = 0; i < stopSymbol.size(); i++) {
            if (_token->getValue() == stopSymbol[i]) {
                stop = true;
            }
        }
        if (stop) {
            if (includeLast) {
                TokenToParseTree(parseTree);
            }
            return parseTree;
        }
        else {
            TokenToParseTree(parseTree);
        }
    }
    return parseTree;
}

bool CompilerParser::IsAVariableType(Token* token)
{
    auto str = token->getValue();
    if (
           str == "int"
        || str == "boolean"
        || str == "char"
        || str == "int"
    ) {
        return true;
    }
    return false;
}

/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::vector<Token*> tokens) {
    _tokens = tokens;
    _token = _tokens[0];
}

/**
 * Generates a parse tree for a single program
 */
ParseTree* CompilerParser::compileProgram() {
    if (_token->getValue() == "var") {
        return compileVarDec();
    }
    else if (_token->getValue() == "if") {
        return compileIf();
    }
    else if (_token->getValue() == "let") {
        return compileStatements();
    }
    else if (_token->getValue() == "do") {
        return compileDo();
    }
    else if (_token->getValue() == "static") {
        return compileClassVarDec();
    }
    else if (_token->getValue() == "class") {
        return compileClass();
    }
    else if (_token->getValue() == "while") {
        return compileWhile();
    }
    else if (_token->getValue() == "function") {
        return compileSubroutine();
    }
    else if (_token->getValue() == "{") {
        return compileSubroutineBody();
    }
    else {
        //const char* error1 = "ParseError (the program doesn't begin with a class)";
        throw ParseException();
    }

    return NULL;
}

/**
 * Generates a parse tree for a single class
 */
ParseTree* CompilerParser::compileClass() {
    ParseTree* parseTree = new ParseTree(_token->getValue(), "");
    while (_token != NULL) {
        if (_token->getValue() == "static") {
            parseTree->addChild(compileClassVarDec());
        }
        else if (_token->getValue() == "function") {
            parseTree->addChild(compileSubroutine());
        }
        else {
            TokenToParseTree(parseTree);
        }
    }
    
    return parseTree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 */
ParseTree* CompilerParser::compileClassVarDec() {
    return AddUntill("classVarDec", { ";" }, true);
}

/**
 * Generates a parse tree for a method, function, or constructor
 */
ParseTree* CompilerParser::compileSubroutine() {
    auto parseTree = AddUntill("subroutine", { "(" }, true);
    parseTree->addChild(compileParameterList());
    TokenToParseTree(parseTree); // )
    parseTree->addChild(compileSubroutineBody());
    return parseTree;
}

/**
 * Generates a parse tree for a subroutine's parameters
 */
ParseTree* CompilerParser::compileParameterList() {
    int typeShould = 0;
    ParseTree* parseTree = new ParseTree("parameterList", "");
    while (_token != NULL) {
        if (_token->getValue() == ")") {
            if (typeShould != 2) {
                throw ParseException();
            }
            return parseTree;
        }
        if (_token->getType() == "keyword") {
            if (!IsAVariableType(_token)) {
                throw ParseException();
            }
        }
        else {
            if (IsAVariableType(_token)) {
                throw ParseException();
            }
        }
        if (typeShould == 0 && _token->getType() != "keyword") {
            throw ParseException();
        }
        else if (typeShould == 1 && _token->getType() != "identifier") {
            throw ParseException();
        }
        else if (typeShould == 2 && _token->getType() != "symbol") {
            throw ParseException();
        }
        typeShould++;
        if (typeShould > 2) {
            typeShould = 0;
        }
        TokenToParseTree(parseTree);

    }
    //if (typeShould != 2) {
    //    throw ParseException();
    //}
    return parseTree;
}

/**
 * Generates a parse tree for a subroutine's body
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* parseTree = new ParseTree("subroutineBody", "");
    TokenToParseTree(parseTree);
    while (_token != NULL) {
        if (_token->getValue() == "var") {
            parseTree->addChild(compileVarDec());
        }
        else if (_token->getValue() == "let") {
            parseTree->addChild(compileStatements());
        }
        else {

            if (_token->getValue() == "}") {
                TokenToParseTree(parseTree);
                return parseTree;
            }
            else {
                // think need to throw an error here
            }
        }
    }
    
}

/**
 * Generates a parse tree for a variable declaration
 */
ParseTree* CompilerParser::compileVarDec() {
    //return AddUntill("varDec", { ";" }, true);

    auto parseTree = AddUntill("varDec", { ";","=" }, true);
    if (_tokenPrevious->getValue() == "=") {
        parseTree->addChild(compileExpression());
        TokenToParseTree(parseTree);
    }
    return parseTree;
}

/**
 * Generates a parse tree for a series of statements
 */
ParseTree* CompilerParser::compileStatements() {
    ParseTree* parseTree = new ParseTree("statements", "");
    while (_token != NULL) {
        if (_token->getValue() == "let") {
            parseTree->addChild(compileLet());
        }
        else if (_token->getValue() == "do") 
        {
            parseTree->addChild(compileDo());
        }
        else if (_token->getValue() == "return")
        {
            parseTree->addChild(compileReturn());
        }
        else if (_token->getValue() == "while") {
            parseTree->addChild(compileWhile());
        }
        else if (_token->getValue() == "}") {
            return parseTree;
        }
        else {
            throw ParseException();
        }
    }
    return parseTree;
}

/**
 * Generates a parse tree for a let statement
 */
ParseTree* CompilerParser::compileLet() {
     auto parseTree = AddUntill("letStatement", { ";","="}, true);
     if (_tokenPrevious->getValue() == "=") {;
         parseTree->addChild(compileExpression());
         TokenToParseTree(parseTree); // ;
     }
     
     return parseTree;
}

/**
 * Generates a parse tree for an if statement
 */
ParseTree* CompilerParser::compileIf() {
    auto parseTree = AddUntill("ifStatement", { "(" }, true);
    parseTree->addChild(compileExpression());
    TokenToParseTree(parseTree); // )
    TokenToParseTree(parseTree); // {
    parseTree->addChild(compileStatements());
    TokenToParseTree(parseTree); // }
    if (_token!= NULL && _token->getValue() == "else") {
        TokenToParseTree(parseTree); // else
        TokenToParseTree(parseTree); // {
        parseTree->addChild(compileStatements());
        TokenToParseTree(parseTree); // }
    }
    return parseTree;
}

/**
 * Generates a parse tree for a while statement
 */
ParseTree* CompilerParser::compileWhile() {
    ParseTree* parseTree = new ParseTree("whileStatement", "");
    TokenToParseTree(parseTree); // while
    TokenToParseTree(parseTree); // (
    parseTree->addChild(compileExpression());
    TokenToParseTree(parseTree); // )
    TokenToParseTree(parseTree); // {
    parseTree->addChild(compileStatements());
    TokenToParseTree(parseTree); // }
    return parseTree;
}

/**
 * Generates a parse tree for a do statement
 */
ParseTree* CompilerParser::compileDo() {
    
    ParseTree* parseTree = new ParseTree("doStatement", "");
    TokenToParseTree(parseTree);
    parseTree->addChild(AddUntill("expression", { ";" }, false));
    TokenToParseTree(parseTree);
    return parseTree;
}

/**
 * Generates a parse tree for a return statement
 */
ParseTree* CompilerParser::compileReturn() {
    return AddUntill("returnStatement", { ";" }, true);
}

/**
 * Generates a parse tree for an expression
 */
ParseTree* CompilerParser::compileExpression() {
    return AddUntill("expression", { ";",")"}, false);
}

/**
 * Generates a parse tree for an expression term
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}


const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
