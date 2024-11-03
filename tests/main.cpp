#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <vector>
#include <string>

#include <iostream>

#include "parser.h"
#include "AST_Expr.h"

SCENARIO("Evaluator works") {
    std::vector<std::string> expressions = {
            "a",
            "(a + b - c) / (d[0] + d[1])",
            "[0]",
            "a.a[max(0,2,0)] + b[min(c)].a",
            ""
    };

    for (auto i = 0; i < 4; ++i) {
        auto file_name = "./data/test" + std::to_string(i + 1) + ".json";
        JSON::Parser J_p;
        AST_EXPR::Parser E_p;
        REQUIRE(J_p.setFileInput(file_name) == true);
        REQUIRE(J_p.setFileInput(file_name) == true);
        REQUIRE(E_p.setExpressionInput(std::move(expressions[i])) == true);
        REQUIRE_NOTHROW(J_p.parse());
        REQUIRE_NOTHROW(E_p.parse());
        REQUIRE_NOTHROW(E_p.eval(J_p.getJson()));
    }

}