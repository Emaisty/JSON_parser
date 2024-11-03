#include <iostream>
#include "JSON/parser.h"
#include "Expr/AST_Expr.h"

void usage(){
    std::cout << "To use the programm run: \n./j_eval *path_to_the_file* *expression*\n";
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        usage();
        return 0;
    }

    // Parse JSON file
    JSON::Parser J_p;
    if (!J_p.setFileInput(argv[1])) {
        std::cout << "ERROR. Cannot open JSON file";
        return 1;
    }
    J_p.parse();

    // Parse expression
    AST_EXPR::Parser E_p;
    if (!E_p.setExpressionInput(argv[2])) {
        std::cout << "ERROR";
        return 1;
    }
    E_p.parse();

    E_p.evalAPrint(J_p.getJson());

    return 0;
}
