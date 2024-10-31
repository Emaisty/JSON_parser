#include "AST_Expr.h"

AST_EXPR::FuncCall::FuncCall(std::unique_ptr<Node> &&new_name, std::vector<std::unique_ptr<Node>> &&new_args) {
    name = std::move(new_name);
    args = std::move(new_args);
}

AST_EXPR::MemberAccess::MemberAccess(std::unique_ptr<Node> &&new_map, std::string &&new_member) {
    origin_map = std::move(new_map);
    member = std::move(new_member);
}

AST_EXPR::ElementAccess::ElementAccess(std::unique_ptr<Node> &&new_array, std::unique_ptr<Node> &&new_member) {
    origin_array = std::move(new_array);
    member = std::move(new_member);
}

AST_EXPR::BinOP::BinOP(Operator nop, std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right) {
    op = nop;
    l_node = std::move(left);
    r_node = std::move(right);
}

AST_EXPR::UnOP::UnOP(Operator nop, std::unique_ptr<Node> &&n_node) {
    op = nop;
    node = std::move(n_node);
}

void AST_EXPR::Parser::parse() {
    try {
        cur_tok = lex.gettok();
        expression = EXPR();
    } catch (std::invalid_argument const &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

bool AST_EXPR::Parser::setExpressionInput(std::string &&new_expr) {
    return lex.InitInputString(std::move(new_expr));
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E4() {
    return E4_PRIME(E3());
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E4_PRIME(std::unique_ptr<Node> &&left) {
    switch (cur_tok) {
        case tok_plus:
        case tok_minus: {
            auto op = cur_tok == tok_plus ? AST_EXPR::BinOP::PLUS : AST_EXPR::BinOP::MINUS;
            cur_tok = lex.gettok();
            return E4_PRIME(std::make_unique<AST_EXPR::BinOP>(op, std::move(left), E3()));
        }
        default:
            return left;
    }
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E3() {
    return E3_PRIME(E2());
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E3_PRIME(std::unique_ptr<Node> &&left) {
    switch (cur_tok) {
        case tok_asterisk:
        case tok_bkSlash: {
            auto op = cur_tok == tok_asterisk ? AST_EXPR::BinOP::MULT : AST_EXPR::BinOP::DIV;
            cur_tok = lex.gettok();
            return E3_PRIME(std::make_unique<AST_EXPR::BinOP>(op, std::move(left), E2()));
        }
        default:
            return left;
    }
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E2() {
    switch (cur_tok) {
        case tok_plus:
        case tok_minus: {
            auto op = cur_tok == tok_plus ? AST_EXPR::UnOP::UNPLUS : AST_EXPR::UnOP::UNMINUS;
            cur_tok = lex.gettok();
            return std::make_unique<AST_EXPR::UnOP>(op, E2());
        }
        default:
            return E1();

    }
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E1() {
    return E1_PRIME(E0());
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E1_PRIME(std::unique_ptr<Node> &&left) {
    switch (cur_tok) {
        case tok_rnOpBr: {
            cur_tok = lex.gettok();
            auto args = EXPR_LIST();
            matchTokenAndGoNext(tok_rnClBr);
            return E1_PRIME(std::make_unique<AST_EXPR::FuncCall>(std::move(left), std::move(args)));
        }
        case tok_dot: {
            cur_tok = lex.gettok();
            matchToken(tok_identifier);
            auto name = lex.identifierStr();
            cur_tok = lex.gettok();
            return E1_PRIME(std::make_unique<AST_EXPR::MemberAccess>(std::move(left), std::move(name)));
        }
        case tok_sqOpBr: {
            cur_tok = lex.gettok();
            auto elem = EXPR();
            matchTokenAndGoNext(tok_sqClBr);
            return E1_PRIME(std::make_unique<AST_EXPR::ElementAccess>(std::move(left), std::move(elem)));
        }
        default:
            return left;
    }
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E0() {
    switch (cur_tok) {
        case tok_identifier: {
            auto name = lex.identifierStr();
            auto res = std::make_unique<AST_EXPR::Variable>(std::move(name));
            cur_tok = lex.gettok();
            return res;
        }
        case tok_number: {
            auto res = std::make_unique<AST_EXPR::Number>(lex.numVal());
            cur_tok = lex.gettok();
            return res;
        }
        case tok_rnOpBr: {
            cur_tok = lex.gettok();
            auto res = EXPR();
            matchTokenAndGoNext(tok_rnClBr);
            return res;
        }
        default:
            throw std::invalid_argument("");
    }
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::EXPR() {
    return E4();
}

std::vector<std::unique_ptr<AST_EXPR::Node>> AST_EXPR::Parser::EXPR_LIST() {
    std::vector<std::unique_ptr<Node>> res;

    do {
        res.push_back(EXPR());
    } while (cur_tok == tok_comma && (cur_tok = lex.gettok()));
    return res;
}

void AST_EXPR::Parser::matchToken(Token token) {
    if (cur_tok != token)
        throw std::invalid_argument(
                "ERROR. Unmatched tokens. Expected:" + std::to_string(token) + ". But got:" + std::to_string(cur_tok));
}

void AST_EXPR::Parser::matchTokenAndGoNext(Token token) {
    if (cur_tok != token)
        throw std::invalid_argument(
                "ERROR. Unmatched tokens. Expected:" + std::to_string(token) + ". But got:" + std::to_string(cur_tok));
    cur_tok = lex.gettok();
}