#include "AST_Expr.h"

std::unique_ptr<JSON::JsonElement> *AST_EXPR::Variable::evaluate(JSON::Context &ctx) {
    if (name == "")
        return ctx.top_element;

    if (ctx.look_for_func) {
        ctx.look_for_func = false;
        if (ctx.functions.find(name) == ctx.functions.end())
            throw std::invalid_argument("ERROR. Function with such name not founded.");

        return &((*ctx.functions.find(name)).second);
    }

    if (!dynamic_cast<JSON::MapElem *>(ctx.top_element->get()))
        throw std::invalid_argument("");

    auto item = dynamic_cast<JSON::MapElem *>(ctx.top_element->get())->getItem(name);

    if (!item)
        throw std::invalid_argument("");

    return item;

}

std::unique_ptr<JSON::JsonElement> *AST_EXPR::Number::evaluate(JSON::Context &ctx) {
    return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(value));
}

AST_EXPR::FuncCall::FuncCall(std::unique_ptr<Node> &&new_name, std::vector<std::unique_ptr<Node>> &&new_args) {
    name = std::move(new_name);
    args = std::move(new_args);
}

std::unique_ptr<JSON::JsonElement> *AST_EXPR::FuncCall::evaluate(JSON::Context &ctx) {

    ctx.look_for_func = true;
    auto func = name->evaluate(ctx);

    std::vector<std::unique_ptr<JSON::JsonElement> *> eval_args;
    for (auto &i: args)
        eval_args.emplace_back(i->evaluate(ctx));

    if (!dynamic_cast<JSON::FuncElem *>(func->get()))
        throw std::invalid_argument("ERROR. Function called on non-function type.");

    return (*dynamic_cast<JSON::FuncElem *>(func->get())->getBody())(eval_args, ctx);

}

AST_EXPR::MemberAccess::MemberAccess(std::unique_ptr<Node> &&new_map, std::string &&new_member) {
    origin_map = std::move(new_map);
    member = std::move(new_member);
}

std::unique_ptr<JSON::JsonElement> *AST_EXPR::MemberAccess::evaluate(JSON::Context &ctx) {
    auto object = origin_map->evaluate(ctx);

    if (!dynamic_cast<JSON::MapElem *>(object->get()))
        throw std::invalid_argument("");

    auto item = dynamic_cast<JSON::MapElem *>(object->get())->getItem(member);

    if (!item)
        throw std::invalid_argument("");

    return item;
}

AST_EXPR::ElementAccess::ElementAccess(std::unique_ptr<Node> &&new_array, std::unique_ptr<Node> &&new_member) {
    origin_array = std::move(new_array);
    member = std::move(new_member);
}

std::unique_ptr<JSON::JsonElement> *AST_EXPR::ElementAccess::evaluate(JSON::Context &ctx) {
    auto array = origin_array->evaluate(ctx);

    if (!dynamic_cast<JSON::ArrayElem *>(array->get()))
        throw std::invalid_argument("");

    auto elem = member->evaluate(ctx);

    if (!dynamic_cast<JSON::IntElem *>(elem->get()))
        throw std::invalid_argument("");

    auto item = dynamic_cast<JSON::ArrayElem *>(array->get())->getElement(
            dynamic_cast<JSON::IntElem *>(elem->get())->getValue());

    if (!item)
        throw std::invalid_argument("");

    return item;
}

AST_EXPR::BinOP::BinOP(Operator nop, std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right) {
    op = nop;
    l_node = std::move(left);
    r_node = std::move(right);
}

std::unique_ptr<JSON::JsonElement> *AST_EXPR::BinOP::evaluate(JSON::Context &ctx) {
    auto left = l_node->evaluate(ctx);
    auto right = r_node->evaluate(ctx);

    if (!(dynamic_cast<JSON::IntElem *>(left->get())) || !(dynamic_cast<JSON::IntElem *>(right->get())))
        throw std::invalid_argument("");


    return [&](long long l, long long r) {
        return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(
                op == PLUS ? l + r : (op == MINUS ? l - r : (op == MULT ? l * r : l / r))));
    }(dynamic_cast<JSON::IntElem *>(left->get())->getValue(), dynamic_cast<JSON::IntElem *>(right->get())->getValue());
}

AST_EXPR::UnOP::UnOP(Operator nop, std::unique_ptr<Node> &&n_node) {
    op = nop;
    node = std::move(n_node);
}

std::unique_ptr<JSON::JsonElement> *AST_EXPR::UnOP::evaluate(JSON::Context &ctx) {
    auto val = node->evaluate(ctx);

    if (!dynamic_cast<JSON::IntElem *>(val->get()))
        throw std::invalid_argument("");

    return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(
            op == UNPLUS ? dynamic_cast<JSON::IntElem *>(val->get())->getValue()
                         : -dynamic_cast<JSON::IntElem *>(val->get())->getValue()));
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

void AST_EXPR::Parser::evalAPrint(std::unique_ptr<JSON::JsonElement> *json) {
    JSON::Context ctx;
    ctx.top_element = json;
    try {
        expression->evaluate(ctx)->get()->print(std::cout);
    } catch (std::invalid_argument const &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E4() {
    return E4_PRIME(E3());
}

std::unique_ptr<AST_EXPR::Node> AST_EXPR::Parser::E4_PRIME(std::unique_ptr<Node> &&left) {
    switch (cur_tok) {
        case tok_plus:
        case tok_minus: {
            auto op = (cur_tok == tok_plus ? AST_EXPR::BinOP::PLUS : AST_EXPR::BinOP::MINUS);
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
            auto op = (cur_tok == tok_asterisk ? AST_EXPR::BinOP::MULT : AST_EXPR::BinOP::DIV);
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
            auto op = (cur_tok == tok_plus ? AST_EXPR::UnOP::UNPLUS : AST_EXPR::UnOP::UNMINUS);
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
        case tok_sqOpBr: {
            cur_tok = lex.gettok();
            auto elem = EXPR();
            matchTokenAndGoNext(tok_sqClBr);
            return std::make_unique<AST_EXPR::ElementAccess>(std::make_unique<AST_EXPR::Variable>(""), std::move(elem));
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