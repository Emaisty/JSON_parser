#include "parser.h"

void JSON::Parser::parse() {
    try {
        cur_tok = lex.gettok();
        JSON = parseElem();
    } catch (std::invalid_argument const &e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }
}

bool JSON::Parser::setFileInput(const std::string &name_of_file) {
    if (lex.InitInputFile(name_of_file))
        return true;
    return false;
}

std::unique_ptr<JSON::JsonElement> *JSON::Parser::getJson() {
    return &JSON;
}

std::unique_ptr<JSON::JsonElement> JSON::Parser::parseElem() {
    switch (cur_tok) {
        case tok_fgOpBr:
            return parseMap();
        case tok_sqOpBr:
            return parseVector();
        case tok_quote:
            return parseString();
        case tok_minus:
        case tok_plus:
        case tok_number:
            return parseInt();
        default:
            throw std::invalid_argument("ERROR. Unknown type.");
    }
}

std::unique_ptr<JSON::IntElem> JSON::Parser::parseInt() {
    bool positive_sign = true;

    while (cur_tok == tok_minus || cur_tok == tok_plus) {
        if (cur_tok == tok_minus)
            positive_sign = !positive_sign;
        cur_tok = lex.gettok();
    }
    matchToken(tok_number);
    auto value = lex.numVal();

    cur_tok = lex.gettok();

    if (positive_sign)
        return std::make_unique<JSON::IntElem>(value);

    return std::make_unique<JSON::IntElem>(-value);
}

std::unique_ptr<JSON::StringElem> JSON::Parser::parseString() {
    matchTokenAndGoNext(tok_quote);
    matchToken(tok_identifier);

    auto name = lex.identifierStr();

    cur_tok = lex.gettok();
    matchTokenAndGoNext(tok_quote);

    return std::make_unique<JSON::StringElem>(std::move(name));
}

std::unique_ptr<JSON::MapElem> JSON::Parser::parseMap() {
    matchTokenAndGoNext(tok_fgOpBr);
    auto res = std::make_unique<JSON::MapElem>();

    if (cur_tok == tok_fgClBr) {
        cur_tok = lex.gettok();
        return std::move(res);
    }

    do {
        matchTokenAndGoNext(tok_quote);
        matchToken(tok_identifier);

        auto name = lex.identifierStr();

        cur_tok = lex.gettok();
        matchTokenAndGoNext(tok_quote);
        matchTokenAndGoNext(tok_column);

        res->addNewEntry(name, parseElem());
    } while (cur_tok == tok_comma && (cur_tok = lex.gettok()));

    matchTokenAndGoNext(tok_fgClBr);

    return std::move(res);
}

std::unique_ptr<JSON::ArrayElem> JSON::Parser::parseVector() {
    matchTokenAndGoNext(tok_sqOpBr);
    auto res = std::make_unique<JSON::ArrayElem>();

    if (cur_tok == tok_sqClBr) {
        cur_tok = lex.gettok();
        return std::move(res);
    }

    do {
        res->addNewEntry(parseElem());
    } while (cur_tok == tok_comma && (cur_tok = lex.gettok()));

    matchTokenAndGoNext(tok_sqClBr);

    return std::move(res);
}

void JSON::Parser::matchToken(Token token) {
    if (cur_tok != token)
        throw std::invalid_argument(
                "ERROR. Unmatched tokens. Expected:" + std::to_string(token) + ". But got:" +
                std::to_string(cur_tok));
}

void JSON::Parser::matchTokenAndGoNext(Token token) {
    if (cur_tok != token)
        throw std::invalid_argument(
                "ERROR. Unmatched tokens. Expected:" + std::to_string(token) + ". But got:" +
                std::to_string(cur_tok));
    cur_tok = lex.gettok();
}