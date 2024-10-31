#include <iostream>
#include "Lexer/lexer.h"

int main() {
    Lexer lex;

    lex.InitInputString("{\"a\": { \"b\": [ 1, 2, { \"c\": \"test\" }, [11, 12] ]}}");
    auto tokken = lex.gettok();

    while (tokken != tok_eof){
        std::cout << tokken << std::endl;
        tokken = lex.gettok();
    }

    return 0;
}
