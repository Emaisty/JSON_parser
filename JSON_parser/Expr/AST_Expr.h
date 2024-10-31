#ifndef JSON_PARSER_AST_EXPR_H
#define JSON_PARSER_AST_EXPR_H


namespace AST_EXPR {

    // basic abstract class
    class Node {
    public:

    private:

    };

    // a
    class Variable : public Node {
    public:

    private:

    };

    // a.b
    class MemberAccess : public Node {
    public:

    private:

    };

    // a[0]
    class ElementAccess : public Node {
    public:

    private:

    };

    // Binary operator. Supports: +, -, /, *
    class BinOP : public Node {
    public:

    private:

    };

}

#endif //JSON_PARSER_AST_EXPR_H
