#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "parser.hpp"

class Generator {
    public:
        inline Generator(node::Program program) 
            : m_program(std::move(program))
        {}


        inline void generate_expr(const node::Expr& expr) {
            struct ExprVisitor {
                Generator* generator;
                void operator()(const node::ExprIntLit& expr_int_lit) {
                    generator->m_out << "mov rax, " << expr_int_lit.token_int.valor.value() << '\n';
                    generator->m_out << "push rax\n";
                }
                void operator()(const node::ExprIdentif& expr_identif) {
                    //TODO
                }
            };

            ExprVisitor visitor {.generator = this};
            std::visit(visitor, expr.variant_expr);
        } 

        /*
        
        */
        inline void generate_statmt(const node::Statmt& statmt) {
            struct StatmtVisitor {
                Generator* generator;
                void operator()(const node::StatmtExit& statmt_exit) {
                    generator->generate_expr(statmt_exit.expr);
                    generator->m_out << "    mov rax, 60\n";
                    generator->m_out << "    pop rdi\n";
                    generator->m_out << "    syscall\n";
                }
                void operator()(const node::StatmtVar statmt_var) const {
                    //TODO
                }
            };

            StatmtVisitor visitor {.generator = this};
            std::visit(visitor, statmt.variant_statmt);
        }


        /*Função que converte os tokens obtidos pela tokenização
        no respectivo código em assembly.
        PARÂMETROS:
        - tokens_file (std::vector<Token>): vetor de tokens lexicais
        do arquivo .ml
        RETURNS:
        - out (std::string): formato em string de uma stringstream
        que contêm o código em asm correspondente ao código em ml
        */
        inline std::string generate_program() {
            m_out << "global _start\n_start:\n";

            for (const node::Statmt& statmt : m_program.statmts) {
                generate_statmt(statmt);
            }

            m_out << "    mov rax, 60\n";
            m_out << "    mov rdi, 0\n";
            m_out << "    syscall\n";
            return m_out.str();
        }


    private:
        const node::Program m_program;
        std::stringstream m_out;
};