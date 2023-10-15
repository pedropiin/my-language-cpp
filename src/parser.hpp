#pragma once

#include <vector>
#include <variant>

#include "./tokenization.hpp"

/*
Namespace que contêm todos os nós necessários para a 
implementação da árvore de parsing. Cada nó representa
uma expressão sintática da linguagem (.ml)
*/
namespace node {
    struct ExprIntLit {
        Token token_int;
    };

    struct ExprIdentif {
        Token token_identif;
    };

    struct Expr {
        std::variant<node::ExprIntLit, node::ExprIdentif> variant_expr ;
    };

    struct DeclarExit {
        node::Expr expr;
    };

    struct DeclarVar {
        Token token_identif;
        node::Expr expr;
    };

    struct Declar {
        std::variant<node::DeclarVar, node::DeclarExit> variant_declar;
    };

    struct Programa {
        std::vector<node::Declar> declars;
    };
};

class Parser {
    public:
        inline Parser(std::vector<Token> tokens) 
            : m_tokens(std::move(tokens))
        {}

        /*
        Método responsável por checar existência de expressão
        após o encontro de um nó atrelado à "_exit"
        PARÂMETROS:
        RETURNS:
        - no (std::optional<node::Expr): caso haja expressão, 
        retorna um nó relacionado ao token da expressão. Caso
        contrário, retorna NULL.
        */
        inline std::optional<node::Expr> parse_expr() {
            if (peek().has_value() && peek().value().tipo == TipoToken::int_lit) {
                node::Expr no = {.token = consume()};
                return no;
            } else {
                return {};
            }
        }

        /*
        Método responsável por parsear todo o vetor de tokens
        do arquivo fonte, procurando por erros de sintaxe
        e nós para cada uma das entidades necessárias
        PARÂMETROS:
        RETURNS:
        - exit_node (std::optional<node::Exit>): nó atrelado
        à expressão de saída do código.
        */
        inline std::optional<node::Exit> parse() {
            std::optional<node::Exit> exit_node;
            while (peek().has_value()) {
                if (peek().value().tipo == TipoToken::_exit) { 
                    consume();
                    if (peek().value().tipo == TipoToken::parenteses_abre) {
                        consume();
                        std::optional<node::Expr> expr_node = parse_expr();
                        if (expr_node.has_value()) {
                            if (peek().value().tipo == TipoToken::parenteses_fecha) {
                                exit_node = node::Exit{.expr = expr_node.value()};
                                consume();
                            } else {
                                std::cerr << "Expressão inválida. Esperava-se ')' ao final da função" << std::endl;
                                exit(EXIT_FAILURE);
                            }
                        } else {
                            std::cerr << "Expressão inválida. A função 'exit' deve conter uma expressão do tipo 'int'" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        if (peek().has_value() && peek().value().tipo == TipoToken::ponto_virgula) {
                            consume();
                        } else {
                            std::cerr << "Erro de sintaxe. Por favor coloque um ';' no final da linha." << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        std::cerr << "Erro de sintaxe. A função deve conter '('." << std::endl;
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
        - offset (int): número de tokens que o usuário 
        deseja analisar a frente do índice atual. Por padrão
        é settado como = 0.
        RETURNS:
        - m_tokens[m_index] (std::optional<Token>): token no 
        índice de análise do vetor
        */
        inline std::optional<Token> peek(int offset = 0) const {
            if (m_index + offset >= m_tokens.size()) {
                return {};
            } else {
                return m_tokens[m_index + offset];
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