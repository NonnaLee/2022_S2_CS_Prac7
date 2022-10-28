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
        _token = _tokens[_index];
    }
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
    //if (_tokens[0]->getValue() != "class") {
    //    return NULL;
    //}
    ParseTree* programTree = new ParseTree("", "");
    while (_token != NULL) {
        if (_token->getValue() == "class") {
            auto classTree = compileClass();
            if (classTree != NULL) {
                programTree->addChild(classTree);
            }
        }
        else {
            NextToken();
        }
    }
    if (programTree->getChildren().size() == 1) {
        return programTree->getChildren()[0];
    }
    
    return programTree;
}

/**
 * Generates a parse tree for a single class
 */
ParseTree* CompilerParser::compileClass() {
    ParseTree* parseTree = new ParseTree(_token->getValue(), "");
    NextToken();
    while (_token != NULL) {
        ParseTree* parseTree2 = new ParseTree(_token->getType(), _token->getValue());
        parseTree->addChild(parseTree2);
        NextToken();
    }
    
    return parseTree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 */
ParseTree* CompilerParser::compileClassVarDec() {
    return NULL;
}

/**
 * Generates a parse tree for a method, function, or constructor
 */
ParseTree* CompilerParser::compileSubroutine() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's parameters
 */
ParseTree* CompilerParser::compileParameterList() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's body
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    return NULL;
}

/**
 * Generates a parse tree for a variable declaration
 */
ParseTree* CompilerParser::compileVarDec() {
    return NULL;
}

/**
 * Generates a parse tree for a series of statements
 */
ParseTree* CompilerParser::compileStatements() {
    return NULL;
}

/**
 * Generates a parse tree for a let statement
 */
ParseTree* CompilerParser::compileLet() {
    return NULL;
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
    return NULL;
}

/**
 * Generates a parse tree for a return statement
 */
ParseTree* CompilerParser::compileReturn() {
    return NULL;
}

/**
 * Generates a parse tree for an expression
 */
ParseTree* CompilerParser::compileExpression() {
    return NULL;
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
