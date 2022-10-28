#include <iostream>
#include <vector>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char *argv[]) {
    vector<Token*> tokens;
    /* Tokens for:
     *     class MyClass {
     *
     *     }
     */
    //tokens.push_back(new Token("keyword", "class"));
    //tokens.push_back(new Token("identifier", "MyClass"));
    //tokens.push_back(new Token("symbol", "{"));
    //tokens.push_back(new Token("symbol", "}"));


    //class Main {
    //    static int a;
    //}
    //tokens.push_back(new Token("keyword", "static"));
    //tokens.push_back(new Token("keyword", "int"));
    //tokens.push_back(new Token("identifier", "a"));
    //tokens.push_back(new Token("symbol", ";"));

    //tokens.push_back(new Token("keyword", "class"));
    //tokens.push_back(new Token("identifier", "MyClass"));
    //tokens.push_back(new Token("symbol", "{"));
    //    tokens.push_back(new Token("keyword", "static"));
    //    tokens.push_back(new Token("keyword", "int"));
    //    tokens.push_back(new Token("identifier", "a"));
    //    tokens.push_back(new Token("symbol", ";"));

    //    tokens.push_back(new Token("keyword", "static"));
    //    tokens.push_back(new Token("keyword", "int"));
    //    tokens.push_back(new Token("identifier", "b"));
    //    tokens.push_back(new Token("symbol", ";"));
    //tokens.push_back(new Token("symbol", "}"));




    // let a = skip ;
    //tokens.push_back(new Token("keyword", "let"));
    //tokens.push_back(new Token("identifier", "a"));
    //tokens.push_back(new Token("symbol", "="));
    //tokens.push_back(new Token("keyword", "skip"));
    //tokens.push_back(new Token("symbol", ";"));


    // do skip ;
    tokens.push_back(new Token("keyword", "do"));
    tokens.push_back(new Token("keyword", "skip"));
    tokens.push_back(new Token("symbol", ";"));

    //function void myFunc(int a) {
    //    var int a;
    //    let a = 1;
    //}

    try {
        CompilerParser parser(tokens);
        ParseTree* result = parser.compileProgram();
        if (result != NULL){
            cout << result->tostring() << endl;
        }
    } catch (ParseException e) {
        cout << "Error Parsing!" << endl;
    }
}