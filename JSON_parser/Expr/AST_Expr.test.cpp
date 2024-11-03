#include <catch.hpp>

#include <list>

#include "AST_Expr.h"

using namespace AST_EXPR;

SCENARIO("Parser can be assigned with expression") {
    Parser parser;
    REQUIRE(parser.setExpressionInput("a.b[1]"));
}

SCENARIO("Parser can parse") {
    std::list<std::string> tests = {
            "a.b.c.u",
            "12 + 56/ 56",
            "(3 / 3) * (5 * 12) + 8 / 4 * 6",
            "a.b[6 + 12 / t.p[6].t]",
            "size(a)",
            "min(1,2,4)",
            "max(1,2,4)",
    };
    for (auto &i: tests) {
        Parser parser;
        parser.setExpressionInput(std::move(i));
        REQUIRE_NOTHROW(parser.parse());
    }

}