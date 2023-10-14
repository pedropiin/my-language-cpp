#pragma once

#include <vector>

#include "./tokenization.hpp"


namespace node {
    struct Expr {
        Token token;
    };

    struct Exit {
        node::Expr expr;
    };
};

class Parser {
    public:
        inline Parser(std::vector<Token> tokens) 
            : m_tokens(std::move(tokens))
        {}

        /*
        
        */
        inline std::optional<node::Expr> parse_expr() {
            if (peak().has_value() && peak().value().tipo == TipoToken::int_lit) {
                node::Expr no = {.token = consume()};
                return no;
            } else {
                return {};
            }
        }

        /*
        
        */
        inline std::optional<node::Exit> parse() {
            std::optional<node::Exit> exit_node;

            while (peak().has_value()) {
                if (peak().value().tipo == TipoToken::_exit) {
                    consume();
                    std::optional<node::Expr> expr_node = parse_expr();
                    if (expr_node.has_value()) {
                        exit_node = node::Exit {.expr = expr_node.value()};
                    } else {
                        std::cerr << "Expressão inválida. A função 'exit' deve conter uma expressão do tipo 'int'" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    if (peak().has_value() && peak().value().tipo == TipoToken::ponto_virgula) {
                        consume();
                    } else {
                        std::cerr << "Erro de sintaxe. Por favor coloque um ';' no final da linha." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
            m_index = 0;
            return exit_node;
        }


    private:
        const std::vector<Token> m_tokens;
        int m_index = 0;

        /*
        Método que "olha" o próximo índice do vetor de tokens
        para ver se chegou ao seu fim, ou se é um token
        válido.
        PARÂMETROS:
        - num_tokens (int): número de tokens que o usuário 
        deseja analisar a frente do índice atual. Por padrão
        é settado como = 1.
        RETURNS:
        - m_tokens[m_index] (std::optional<Token>): token no 
        índice de análise do vetor
        */
        inline std::optional<Token> peak(int num_tokens = 1) const {
            if (m_index + num_tokens > m_tokens.size()) {
                return {};
            } else {
                return m_tokens[m_index];
            }
        }

        /*
        Método que retorna o token no índice atual e 
        incrementa o índice com +1.
        PARÂMETROS:
        RETURNS:
        - token (Token): token no índice atual do vetor
        */
        inline Token consume() {
            Token token = m_tokens[m_index];
            m_index++;
            return token;
        }
};