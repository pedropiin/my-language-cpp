#pragma once

#include <vector>
#include <variant>
#include <assert.h>

#include "./arena.hpp"
#include "./tokenization.hpp"

/*
Namespace que contêm todos os nós necessários para a 
implementação da árvore de parsing. Cada nó representa
uma expressão sintática da linguagem (.ml)
*/
namespace node {
    struct BinExpr;

    struct TermIntLit {
        Token token_int;
    };

    struct TermIdentif {
        Token token_identif;
    };

    struct Term {
        std::variant<node::TermIntLit*, node::TermIdentif*> variant_term;
    };

    struct Expr {
        std::variant<node::Term*, node::BinExpr*> variant_expr;
    };

    struct BinExprSoma {
        node::Expr* lado_esquerdo;
        node::Expr* lado_direito;
    };

    struct BinExprSub {
        node::Expr* lado_esquerdo;
        node::Expr* lado_direito;
    };

    struct BinExprMulti {
        node::Expr* lado_esquerdo;
        node::Expr* lado_direito;
    };

    struct BinExprDiv {
        node::Expr* lado_esquerdo;
        node::Expr* lado_direito;
    };

    struct BinExpr {
        std::variant<node::BinExprSoma*, node::BinExprSub*, node::BinExprMulti*, node::BinExprDiv*> variant_bin_expr;
    };

    struct StatmtExit {
        node::Expr* expr;
    };

    struct StatmtVar {
        Token token_identif;
        node::Expr* expr;
    };

    struct Statmt {
        std::variant<node::StatmtVar*, node::StatmtExit*> variant_statmt;
    };

    struct Program {
        std::vector<node::Statmt*> statmts;
    };
};

//TODO: implementar Token operador dentro das structs de operação binaria

class Parser {
    public:
        inline Parser(std::vector<Token> tokens) 
            : m_tokens(std::move(tokens)), m_alloc(1024 * 1024 * 4)
        {}


        // TODO: implementar parse_bin_expr com visitor, permitindo

        //ter a propria função
        // inline std::optional<node::BinExpr*> parse_bin_expr() {
        //     if (auto lado_esquerdo = parse_expr()) {
        //         auto bin_expr = m_alloc.alloc<node::BinExpr>();
        //         if (peek().has_value() && peek().value().tipo == TipoToken::mais) {
        //             auto bin_expr_soma = m_alloc.alloc<node::BinExprSoma>();
        //             bin_expr_soma->lado_esquerdo = lado_esquerdo.value();
        //             consume();
        //             if (auto lado_direito = parse_expr()) {
        //                 bin_expr_soma->lado_direito = lado_direito.value();
        //                 bin_expr->variant_bin_expr = bin_expr_soma;
        //                 return bin_expr;
        //             } else {
        //                 std::cerr << "Esperava-se expressão após operador binário." << std::endl;
        //                 exit(EXIT_FAILURE);
        //             }
        //         } else if (peek().has_value() && peek().value().tipo == TipoToken::asterisco) {
        //             assert(false);
        //         } else {
        //             std::cerr << "Operador binário não reconhecido." << std::endl;
        //             exit(EXIT_FAILURE);
        //         }
        //     }
        //     return {};
        // }


        /*
        TODO: documentação
        */
        inline std::optional<node::Term*> parse_term() {
            auto term = m_alloc.alloc<node::Term>();
            if (peek().has_value()) {
                if (peek().value().tipo == TipoToken::int_lit) {
                    auto term_int_lit = m_alloc.alloc<node::TermIntLit>();
                    term_int_lit->token_int = consume();
                    term->variant_term = term_int_lit;
                } else if (peek().value().tipo == TipoToken::identif) {
                    auto term_identif = m_alloc.alloc<node::TermIdentif>();
                    term_identif->token_identif = consume();
                    term->variant_term = term_identif;
                }
            }
            return term;
        }

        /*
        TODO: documentacao
        https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing
        */
        inline std::optional<node::Expr*> parse_expr(int min_prec = 0) {
            auto term_esquerda = parse_term();
            if (!term_esquerda.has_value()) {
                return {};
            }
            auto expr_esquerda = m_alloc.alloc<node::Expr>();
            expr_esquerda->variant_expr = term_esquerda.value();

            while (true) {
                std::optional<Token> token_atual = peek();
                std::optional<int> prec;
                if (token_atual.has_value()) {
                    prec = bin_prec(token_atual.value().tipo);
                    if (!prec.has_value() || prec < min_prec) {
                        break;
                    }
                } else {
                    break;
                }
                Token operador = consume();
                auto expr_direita = parse_expr(prec.value() + 1);
                if (!expr_direita.has_value()) {
                    std::cerr << "Expressão inválida. Esperava-se um termo após o operador." << std::endl;
                    exit(EXIT_FAILURE);
                } 
                auto bin_expr = m_alloc.alloc<node::BinExpr>();
                auto expr_esquerda_temp = m_alloc.alloc<node::Expr>();
                expr_esquerda_temp->variant_expr = expr_esquerda->variant_expr; 

                switch (operador.tipo) {
                    case TipoToken::mais:
                        {
                            auto bin_expr_soma = m_alloc.alloc<node::BinExprSoma>();
                            bin_expr_soma->lado_esquerdo = expr_esquerda_temp;
                            bin_expr_soma->lado_direito = expr_direita.value();
                            bin_expr->variant_bin_expr = bin_expr_soma;
                        }
                        break;
                    case TipoToken::menos:
                        {
                            auto bin_expr_sub = m_alloc.alloc<node::BinExprSub>();
                            bin_expr_sub->lado_esquerdo = expr_esquerda_temp;
                            bin_expr_sub->lado_direito = expr_direita.value();
                            bin_expr->variant_bin_expr = bin_expr_sub;
                        }
                        break;
                    case TipoToken::asterisco:
                        {
                            auto bin_expr_multi = m_alloc.alloc<node::BinExprMulti>();
                            bin_expr_multi->lado_esquerdo = expr_esquerda_temp;
                            bin_expr_multi->lado_direito = expr_direita.value();
                            bin_expr->variant_bin_expr = bin_expr_multi;
                        }
                        break;
                    case TipoToken::barra_div:
                        {
                            auto bin_expr_div = m_alloc.alloc<node::BinExprDiv>();
                            bin_expr_div->lado_esquerdo = expr_esquerda_temp;
                            bin_expr_div->lado_direito = expr_direita.value();
                            bin_expr->variant_bin_expr = bin_expr_div;
                        }
                        break;
                }
                expr_esquerda->variant_expr = bin_expr;
            }
            return expr_esquerda;
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
        inline std::optional<node::Statmt*> parse_statmt() {
            if (peek().has_value() && peek().value().tipo == TipoToken::_exit) {
                consume();
                auto statmt_exit = m_alloc.alloc<node::StatmtExit>();
                try_consume(TipoToken::parenteses_abre, "Erro de sintaxe. A função deve conter '('.");
                if (auto node_expr = parse_expr()) {
                    statmt_exit->expr = node_expr.value();
                } else {
                    std::cerr << "Expressão inválida. A função 'exit' deve conter uma expressão 'int_lit' ou um identificador." << std::endl;
                    exit(EXIT_FAILURE);
                }
                try_consume(TipoToken::parenteses_fecha, "Erro de sintaxe. Esperava-se ')' ao final da função.");
                try_consume(TipoToken::ponto_virgula, "Erro de sintaxe. Esperava-se ';' no final da linha.");
                auto statmt = m_alloc.alloc<node::Statmt>();
                statmt->variant_statmt = statmt_exit;
                return statmt;

            } else if (peek().has_value() && peek().value().tipo == TipoToken::var) {
                consume();
                auto statmt_var = m_alloc.alloc<node::StatmtVar>();
                if (peek().has_value() && peek().value().tipo == TipoToken::identif) {
                    statmt_var->token_identif = consume();
                    try_consume(TipoToken::igual, "Erro de sintaxe. Esperava-se '=' após declaração de variável.");
                    if (auto node_expr = parse_expr()) {
                        statmt_var->expr = node_expr.value();
                    } else {
                        std::cerr << "Expressão inválida." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    try_consume(TipoToken::ponto_virgula, "Erro de sintaze. Esperava-se ';' no final da linha.");
                } else {
                    std::cerr << "Declaração inválida. Uma variável precisa de um identificador." << std::endl;
                    exit(EXIT_FAILURE);
                }
                auto statmt = m_alloc.alloc<node::Statmt>();
                statmt->variant_statmt = statmt_var;
                return statmt;
            } else {
                return {};
            }
        }

        /*
        TODO: documentação
        */
        inline std::optional<node::Program> parse_program() {
            node::Program program;
            while(peek().has_value()) {
                if (auto node_statmt = parse_statmt()) {
                    program.statmts.push_back(node_statmt.value());
                } else {
                    std::cerr << "Declaração inválida." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            return program;
        }



    private:
        const std::vector<Token> m_tokens;
        int m_index = 0;
        ArenaAlloc m_alloc;

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

        /*
        TODO: documentação
        */
        inline Token try_consume(TipoToken tipo, const std::string& erro_desc) {
            if (peek().has_value() && peek().value().tipo == tipo) {
                return consume();
            } else {
                std::cerr << erro_desc << std::endl;
                exit(EXIT_FAILURE);
            }
        }
};