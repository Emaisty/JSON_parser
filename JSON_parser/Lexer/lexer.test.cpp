#include <catch.hpp>

#include <list>
#include <string>
#include <vector>

#include <fstream>

#include "lexer.h"

SCENARIO("Lexer can accept string as input") {
    Lexer lex;
    REQUIRE(lex.InitInputString("a.b") == true);
}

SCENARIO("Lexer fail on non existed file") {
    Lexer lex;
    REQUIRE(lex.InitInputFile("test_not_exists_14567fd.kek") == false);
}

SCENARIO("Lexer can open file") {

    std::ofstream outfile("test.txt");
    outfile.close();

    Lexer lex;
    REQUIRE(lex.InitInputFile("test.txt"));
}

SCENARIO("Lexer unparsebale input") {
    std::list<std::string> tests = {
            "&",
            "_",
    };
    Lexer lex;
    for (auto &i: tests) {
        lex.InitInputString(std::move(i));
        REQUIRE_THROWS(lex.gettok());
    }


}

SCENARIO("Lexer can work") {
    std::list<std::pair<std::string, std::vector<Token>>> tests = {
            {"a b c d",    {tok_identifier, tok_identifier, tok_identifier, tok_identifier}},
            {"a + b 12",   {tok_identifier, tok_plus,       tok_identifier, tok_number}},
            {"{}()[]+-*/", {tok_fgOpBr,     tok_fgClBr,     tok_rnOpBr,     tok_rnClBr, tok_sqOpBr, tok_sqClBr, tok_plus, tok_minus, tok_asterisk, tok_bkSlash}}
    };

    for (auto &i: tests) {
        Lexer lex;
        lex.InitInputString(std::move(i.first));
        Token token;
        for (auto &j: i.second) {
            token = lex.gettok();
            REQUIRE(token == j);
        }
    }

}