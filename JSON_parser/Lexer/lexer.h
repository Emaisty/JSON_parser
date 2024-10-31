#ifndef JSON_PARSER_LEXER_H
#define JSON_PARSER_LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

enum Token {
    tok_eof,

    tok_number,
    tok_identifier,

    tok_dot,
    tok_comma,
    tok_quote,
    tok_column,

    tok_plus,
    tok_minus,
    tok_asterisk,
    tok_bkSlash,

    tok_sqOpBr,
    tok_sqClBr,
    tok_fgOpBr,
    tok_fgClBr,
    tok_rnOpBr,
    tok_rnClBr,

    tok_sum,
    tok_min,
    tok_max,
    tok_size
    //token for new functions here
};

// type of input chars
enum InputCharType {
    LETTER, NUMBER, WHITE_SPACE, NEW_LINE, END, NO_TYPE, SPE_SYMB
};

class Lexer {
public:
    Lexer() = default;

    ~Lexer() = default;

    bool InitInputString(std::string&&);

    bool InitInputFile(const std::string&);

    const std::string &identifierStr() const { return this->m_IdentifierStr; }

    int numVal() { return this->m_NumVal; }

    Token gettok();

private:
    char inputSymbol();

    InputCharType type_of_char();

    Token readString();

    Token readNumber();

    Token readSymbol();

    std::string m_IdentifierStr;
    long long m_NumVal;

    std::ifstream file;
    bool file_opened = false;

    unsigned long long pos_in_expression;
    std::string expression;

    char cur_symb;

};

#endif //JSON_PARSER_LEXER_H
