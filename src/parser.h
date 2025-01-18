#pragma once
#include "tokenization.h"

struct NodeExpr{
    Token int_lit;
};

struct NodeExit{
    NodeExpr expr;
};

class Parser{
public:
    explicit Parser(std::vector<Token> tokens): m_tokens(std::move(tokens)){}

    std::optional<NodeExpr> parse_expr(){
        if (peek().has_value() && peek()->type == TokenType::int_literal) {
            return NodeExpr{.int_lit = consume()};
        }
        return {};
    }

    std::optional<NodeExit> parse(){
        std::optional<NodeExit> exit_node;
        while (peek().has_value()) {
            if (peek().value().type == TokenType::exit) {
                consume();
                if (auto node_expr = parse_expr()) {
                    exit_node = NodeExit{.expr = node_expr.value()};
                }
                else {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (peek().has_value() && peek()->type == TokenType::semicolon) {
                    consume();
                }
                else {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        m_index = 0;
        return exit_node;
    }

private:
    const std::vector<Token> m_tokens;
    size_t m_index = 0;

    [[nodiscard]] std::optional<Token> peek(int ahead = 1) const{
        if (m_index + ahead > m_tokens.size()) {
            return {};
        }
        return m_tokens.at(m_index);
    }

    Token consume(){
        return m_tokens.at(m_index++);
    }
};
