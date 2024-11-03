#include "lexer.h"

bool Lexer::InitInputString(std::string &&new_expression) {
    if (file.is_open()) {
        file.close();
        file_opened = false;
    }

    pos_in_expression = 0;
    expression = std::move(new_expression);

    cur_symb = inputSymbol();

    return true;
}

bool Lexer::InitInputFile(const std::string &file_name) {
    expression = "";
    if (file.is_open())
        file.close();

    file.open(file_name);
    if (!file.is_open())
        return false;

    file_opened = true;
    cur_symb = inputSymbol();

    return true;
}

Token Lexer::gettok() {
    switch (type_of_char()) {
        case LETTER:
            return readString();
        case NUMBER:
            return readNumber();
        case NEW_LINE:
        case WHITE_SPACE:
            cur_symb = inputSymbol();
            return gettok();
        case END:
            return tok_eof;
        case SPE_SYMB:
            return readSymbol();
        default:
            throw std::invalid_argument("ERROR. Unknown token");
    }
}

char Lexer::inputSymbol() {
    char c = 0;
    if (file_opened) {
        if (!file.eof())
            file >> c;

        if (c == EOF || c == 0)
            return EOF;

    } else {
        if (pos_in_expression >= expression.size())
            return EOF;

        c = expression[pos_in_expression++];
    }
    return c;
}

InputCharType Lexer::type_of_char() {
    if ((cur_symb >= 'A' && cur_symb <= 'Z') || (cur_symb >= 'a' && cur_symb <= 'z'))
        return LETTER;
    else if (cur_symb >= '0' && cur_symb <= '9')
        return NUMBER;
    else if (cur_symb == EOF || cur_symb == 0)
        return END;
    else if (cur_symb == '\n') {
        return NEW_LINE;
    } else if (cur_symb <= ' ')
        return WHITE_SPACE;
    else if ((cur_symb >= '{' && cur_symb <= '}') || cur_symb == '_' || (cur_symb >= '[' && cur_symb <= ']') ||
             (cur_symb >= ':' && cur_symb <= '?') || (cur_symb >= '!' && cur_symb <= '/'))
        return SPE_SYMB;
    return NO_TYPE;
}

Token Lexer::readString() {
    std::string str;
    while (type_of_char() == LETTER || type_of_char() == NUMBER || (type_of_char() == SPE_SYMB && cur_symb == '_')) {
        str += (char) cur_symb;
        cur_symb = inputSymbol();
    }

    m_IdentifierStr = str;

    return tok_identifier;
}

Token Lexer::readNumber() {
    int sum = 0;
    if (cur_symb == '0') {
        m_NumVal = sum;
        cur_symb = inputSymbol();
        return tok_number;
    }

    while (type_of_char() == NUMBER) {
        sum *= 10;
        sum += cur_symb - 48;
        cur_symb = inputSymbol();
    }

    m_NumVal = sum;
    return tok_number;
}

Token Lexer::readSymbol() {
    char pr_symb = cur_symb;
    cur_symb = inputSymbol();
    switch (pr_symb) {
        case '+':
            return tok_plus;
        case '-':
            return tok_minus;
        case '*':
            return tok_asterisk;
        case '/':
            return tok_bkSlash;
        case '{':
            return tok_fgOpBr;
        case '}':
            return tok_fgClBr;
        case '[':
            return tok_sqOpBr;
        case ']':
            return tok_sqClBr;
        case '(':
            return tok_rnOpBr;
        case ')':
            return tok_rnClBr;
        case '.':
            return tok_dot;
        case ',':
            return tok_comma;
        case '"':
            return tok_quote;
        case ':':
            return tok_column;
        default:
            throw std::invalid_argument("kekjsdklfjksldfj sdklfjsdjfklsdjfkl");
    }
}