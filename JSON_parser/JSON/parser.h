#ifndef JSON_PARSER_PARSER_H
#define JSON_PARSER_PARSER_H

#include "lexer.h"
#include "JSON.h"

namespace JSON {

    class Parser {
    public:
        void parse();

    private:
        std::unique_ptr<JSON::JsonElement>&& parseElem();

        std::unique_ptr<JSON::IntElem>&& parseInt();

        std::unique_ptr<JSON::StringElem>&& parseString();

        std::unique_ptr<JSON::MapElem>&& parseMap();

        std::unique_ptr<JSON::ArrayElem>&& parseVector();

        void matchToken(Token);

        void matchTokenAndGoNext(Token);


        Lexer lex;

        Token cur_tok;

        std::unique_ptr<JSON::JsonElement> JSON;

    };
}

#endif //JSON_PARSER_PARSER_H
