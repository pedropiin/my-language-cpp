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

    struct ExprIntLit {
        Token token_int;
    };

    struct ExprIdentif {
        Token token_identif;
    };

    struct Expr {
        std::variant<node::ExprIntLit*, node::ExprIdentif*, node::BinExpr*> variant_expr;
    };

    struct BinExprSoma {
        node::Expr* lado_esquerdo;
        node::Expr* lado_direito;
    };

    struct BinExprMulti {
        node::Expr* lado_esquerdo;
        node::Expr* lado_direito;
    };

    struct BinExpr {
        std::variant<node::BinExprSoma*, node::BinExprMulti*> variant_bin_expr;
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


class Parser {
    public:
        inline Parser(std::vector<Token> tokens) 
            : m_tokens(std::move(tokens)), m_alloc(1024 * 1024 * 4)
        {}


        inline std::optional<node::BinExpr*> parse_bin_expr() {
            if (auto lado_esquerdo = parse_expr()) {
                auto bin_expr = m_alloc.alloc<node::BinExpr>();
                if (peek().has_value() && peek().value().tipo == TipoToken::soma) {
                    auto bin_expr_soma = m_alloc.alloc<node::BinExprSoma>();
                    bin_expr_soma->lado_esquerdo = lado_esquerdo;
                    consume();
                    if (auto lado_direito = parse_expr()) {
                        bin_expr_soma->lado_direito = lado_direito;
                        bin_expr->variant_bin_expr = bin_expr_soma;
                    } else {
                        std::cerr << "Esperava-se expressão após operador binário." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                } else if (peek().has_value() && peek().value().tipo == TipoToken::multi) {
                    assert(false);
                } else {
                    std::cerr << "Operador binário não reconhecido." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            return bin_expr;
        }

        /*
        Método responsável por checar existência de expressão
        após o encontro de um nó atrelado à "_exit"
        PARÂMETROS:
        RETURNS:
        - no (std::optional<node::Expr): caso haja expressão, 
        retorna um nó relacionado ao token da expressão. Caso
        contrário, retorna NULL.
        */
        inline std::optional<node::Expr*> parse_expr() {
            if (peek().has_value()) {
                if (peek().value().tipo == TipoToken::int_lit) {
                    auto expr_int_lit = m_alloc.alloc<node::ExprIntLit>();
                    expr_int_lit->token_int = consume();
                    auto expr = m_alloc.alloc<node::Expr>();
                    expr->variant_expr = expr_int_lit;
                    return expr;
                } else if (peek().value().tipo == TipoToken::identif) {
                    auto expr_identif = m_alloc.alloc<node::ExprIdentif>();
                    expr_identif->token_identif = consume();
                    auto expr = m_alloc.alloc<node::Expr>();
                    expr->variant_expr = expr_identif;
                    return expr;
                } else if (auto bin_expr = parse_bin_expr()){
                    auto expr = m_alloc.alloc<node::Expr>();
                    expr->variant_expr = bin_expr;
                } else {
                    std::cerr << "Uma expressão pode ser apenas 'int_lit' ou um identificador." << std::endl;
                    exit(EXIT_FAILURE);
                }
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
        inline std::optional<node::Statmt*> parse_statmt() {
            if (peek().has_value() && peek().value().tipo == TipoToken::_exit) {
                consume();
                auto statmt_exit = m_alloc.alloc<node::StatmtExit>();
                if (peek().has_value() && peek().value().tipo == TipoToken::parenteses_abre) {
                    consume();
                    if (auto node_expr = parse_expr()) {
                        statmt_exit->expr = node_expr.value();
                    } else {
                        std::cerr << "Expressão inválida. A função 'exit' deve conter uma expressão 'int_lit' ou um identificador." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    if (peek().has_value() && peek().value().tipo == TipoToken::parenteses_fecha) {
                        consume();
                    } else {
                        std::cerr << "Erro de sintaxe. Esperava-se ')' ao final da função." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    if (peek().has_value() && peek().value().tipo == TipoToken::ponto_virgula) {
                        consume();
                    } else {
                        std::cerr << "Erro de sintaxe. Esperava-se ';' no final da linha." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                } else {
                    std::cerr << "Erro de sintaxe. A função deve conter '('." << std::endl;
                    exit(EXIT_FAILURE);
                }
                auto statmt = m_alloc.alloc<node::Statmt>();
                statmt->variant_statmt = statmt_exit;
                return statmt;

            } else if (peek().has_value() && peek().value().tipo == TipoToken::var) {
                consume();
                auto statmt_var = m_alloc.alloc<node::StatmtVar>();
                if (peek().has_value() && peek().value().tipo == TipoToken::identif) {
                    statmt_var->token_identif = consume();
                    // statmt_var = node::StatmtVar {.token_identif = consume()}; //TALVEZ ERRO NISSO AQUI PORQUE CRIEI NOVO NODE PARA FUNCIONAR
                    if (peek().has_value() && peek().value().tipo == TipoToken::igual) {
                        consume();
                        if (auto node_expr = parse_expr()) {
                            statmt_var->expr = node_expr.value();
                        } else {
                            std::cerr << "Expressão inválida." << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        if (peek().has_value() && peek().value().tipo == TipoToken::ponto_virgula) {
                            consume();
                        } else {
                            std::cerr << "Erro de sintaxe. Esperava-se ';' no final da linha." << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        std::cerr << "Erro de sintaxe. Esperava-se '=' após declaração de variável." << std::endl;
                        exit(EXIT_FAILURE);
                    }
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
};