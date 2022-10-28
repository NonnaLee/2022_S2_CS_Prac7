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

ParseTree* CompilerParser::AddUntill(char* header, std::vector<char*> stopSymbol)
{
    ParseTree* parseTree = new ParseTree(header, "");
    while (_token != NULL) {
        TokenToParseTree(parseTree);
        for (int i = 0; i < stopSymbol.size(); i++) {
            if (_tokenPrevious->getValue() == stopSymbol[i]) {
                return parseTree;
            }
        }
    }
    return parseTree;
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
    else if (_token->getValue() == "let") {
        return compileStatements();
    }
    else if (_token->getValue() == "static") {
        return compileClassVarDec();
    }
    else if (_token->getValue() == "class") {
        return compileClass();
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
    return AddUntill("classVarDec", { ";" });
}

/**
 * Generates a parse tree for a method, function, or constructor
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* parseTree = new ParseTree("subroutine", "");
    while (_token != NULL) {
        
        if (_token->getValue() == "{") {
            compileSubroutineBody();
            return parseTree;
        }
        else {
            TokenToParseTree(parseTree);
        }
    }
}

/**
 * Generates a parse tree for a subroutine's parameters
 */
ParseTree* CompilerParser::compileParameterList() {
    ParseTree* parseTree = new ParseTree("parameterList", "");
    while (_token != NULL) {
        if (_token->getValue() == ")") {
            return parseTree;
        }
        TokenToParseTree(parseTree);

    }
    return parseTree;
}

/**
 * Generates a parse tree for a subroutine's body
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* parseTree = new ParseTree("subroutineBody", "");
    while (_token != NULL) {
        if (_token->getValue() == "var") {
            parseTree->addChild(compileVarDec());
        }
        else if (_token->getValue() == "let") {
            parseTree->addChild(compileStatements());
        }
        else {
            TokenToParseTree(parseTree);
            if (_tokenPrevious->getValue() == "}") {
                return parseTree;
            }
        }
    }
    
}

/**
 * Generates a parse tree for a variable declaration
 */
ParseTree* CompilerParser::compileVarDec() {
    return AddUntill("varDec", { ";" });
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
     auto parseTree = AddUntill("letStatement", { ";","="});
     if (_tokenPrevious->getValue() == "=") {
         parseTree->addChild(compileExpression());
     }
     
     return parseTree;
}

/**
 * Generates a parse tree for an if statement
 */
ParseTree* CompilerParser::compileIf() {
    return NULL;
}

/**
 * Generates a parse tree for a while statement
 */
ParseTree* CompilerParser::compileWhile() {
    return NULL;
}

/**
 * Generates a parse tree for a do statement
 */
ParseTree* CompilerParser::compileDo() {
    return AddUntill("doStatement", { ";" });
}

/**
 * Generates a parse tree for a return statement
 */
ParseTree* CompilerParser::compileReturn() {
    return AddUntill("returnStatement", { ";" });
}

/**
 * Generates a parse tree for an expression
 */
ParseTree* CompilerParser::compileExpression() {
    return AddUntill("expression", { ";" });
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
