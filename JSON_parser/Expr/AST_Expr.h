#ifndef JSON_PARSER_AST_EXPR_H
#define JSON_PARSER_AST_EXPR_H

#include <string>
#include <vector>
#include <list>
#include <map>

#include "lexer.h"
#include "JSON.h"


namespace AST_EXPR {


    

    // basic abstract class
    class Node {
    public:
        Node() = default;

        virtual ~Node() = default;

        virtual std::unique_ptr<JSON::JsonElement> *evaluate(JSON::Context &) = 0;

    private:

    };

    // a
    class Variable : public Node {
    public:
        Variable() = default;

        Variable(std::string &&new_name) : name(std::move(new_name)) {}

        std::unique_ptr<JSON::JsonElement> *evaluate(JSON::Context &) override;

    private:
        std::string name;

    };

    // 0, 1, ...
    class Number : public Node {
    public:
        Number() = default;

        Number(long long new_value) : value(new_value) {}

        std::unique_ptr<JSON::JsonElement> *evaluate(JSON::Context &) override;

    private:
        long long value;

    };

    // a(b,...)
    class FuncCall : public Node {
    public:
        FuncCall() = default;

        FuncCall(std::unique_ptr<Node> &&, std::vector<std::unique_ptr<Node>> &&);

        std::unique_ptr<JSON::JsonElement> *evaluate(JSON::Context &) override;

    private:
        std::unique_ptr<Node> name;

        std::vector<std::unique_ptr<Node>> args;
    };

    // a.b
    class MemberAccess : public Node {
    public:
        MemberAccess() = default;

        MemberAccess(std::unique_ptr<Node> &&, std::string &&);

        std::unique_ptr<JSON::JsonElement> *evaluate(JSON::Context &) override;

    private:
        std::unique_ptr<Node> origin_map;

        std::string member;

    };

    // a[0]
    class ElementAccess : public Node {
    public:
        ElementAccess() = default;

        ElementAccess(std::unique_ptr<Node> &&, std::unique_ptr<Node> &&);

        std::unique_ptr<JSON::JsonElement> *evaluate(JSON::Context &) override;

    private:
        std::unique_ptr<Node> origin_array, member;

    };

    // Binary operator. Supports: +, -, /, *
    class BinOP : public Node {
    public:
        enum Operator {
            PLUS, MINUS, MULT, DIV
        };

        BinOP() = default;

        BinOP(Operator, std::unique_ptr<Node> &&, std::unique_ptr<Node> &&);

        std::unique_ptr<JSON::JsonElement> *evaluate(JSON::Context &) override;

    private:
        std::unique_ptr<Node> r_node, l_node;

        Operator op;

    };

    // Unary operator. Supports: +, -
    class UnOP : public Node {
    public:
        enum Operator {
            UNPLUS, UNMINUS
        };

        UnOP() = default;

        UnOP(Operator, std::unique_ptr<Node> &&);

        std::unique_ptr<JSON::JsonElement> *evaluate(JSON::Context &) override;

    private:
        std::unique_ptr<Node> node;

        Operator op;

    };


    class Parser {
    public:
        void parse();

        bool setExpressionInput(std::string &&);

        void evalAPrint(std::unique_ptr<JSON::JsonElement> *);

    private:
        std::unique_ptr<Node> E5();

        std::unique_ptr<Node> E5_PRIME(std::unique_ptr<Node> &&);

        std::unique_ptr<Node> E4();

        std::unique_ptr<Node> E4_PRIME(std::unique_ptr<Node> &&);

        std::unique_ptr<Node> E3();

        std::unique_ptr<Node> E3_PRIME(std::unique_ptr<Node> &&);

        std::unique_ptr<Node> E2();

        std::unique_ptr<Node> E1();

        std::unique_ptr<Node> E1_PRIME(std::unique_ptr<Node> &&);

        std::unique_ptr<Node> E0();

        std::unique_ptr<Node> EXPR();

        std::vector<std::unique_ptr<Node>> EXPR_LIST();

        void matchToken(Token);

        void matchTokenAndGoNext(Token);

        std::unique_ptr<Node> *get_expression();


        Lexer lex;
        Token cur_tok;

        std::unique_ptr<Node> expression;

    };
}

#endif //JSON_PARSER_AST_EXPR_H
