#include <iostream>
#include "JSON/parser.h"
#include "Expr/AST_Expr.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    // Parse JSON file
    JSON::Parser J_p;
    if (!J_p.setFileInput(argv[1])) {
        return 1;
    }
    J_p.parse();

    // Parse expression
    AST_EXPR::Parser E_p;
    if (!E_p.setExpressionInput(argv[2])) {
        return 1;
    }
    E_p.parse();

    E_p.evalAPrint(J_p.getJson());

    return 0;
}
