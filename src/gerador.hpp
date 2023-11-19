#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <assert.h>

#include "parser.hpp"


struct Variable {
    size_t stack_pos;
};


class Generator {
    public:
        inline Generator(node::Program program) 
            : m_program(std::move(program))
        {}

        /*
        TODO: documentação
        */
        inline void generate_term(const node::Term* term) {
            struct TermVisitor {
                Generator& generator;
                void operator()(const node::TermIntLit* term_int_lit) {
                    generator.m_out << "    mov rax, " << term_int_lit->token_int.valor.value() << '\n';
                    generator.push("rax");
                }
                void operator()(const node::TermIdentif* term_identif) {
                    if (generator.m_variables.contains(term_identif->token_identif.valor.value())) {
                        auto& var = generator.m_variables.at(term_identif->token_identif.valor.value());
                        std::stringstream offset;
                        /*
                        A stack é organizada para que cada elemento tenha um tamanho de 8 bytes. Assim, usamos o operador
                        QWORD para especificar que vamos acessar um local na memória de 8 bytes de tamanho. Para mais, 
                        multiplicamos o offset por 8, pois ele por si só nos da o número de elementos entre o desejado e
                        aquele no topo. Porém, cada elemento da stack possui 8 bytes de espaço, independente do tipo do 
                        dado que ali está. Portanto, multiplicamos por 8 para "descer" o número correto de bytes até o 
                        endereço desejado.
                        */
                        offset << "QWORD [rsp + " << (generator.m_stack_size - var.stack_pos - 1) * 8 << "]"; //stack e registrador %rsp crescem pra baixo. 
                        generator.push(offset.str());
                    } else {
                        std::cerr << "Identificador '" << term_identif->token_identif.valor.value() << "' não inicializado." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                void operator()(const node::TermParen* term_paren) {
                    generator.generate_expr(term_paren->expr);
                }
            };
            TermVisitor visitor {.generator = *this};
            std::visit(visitor, term->variant_term);
        }

        /*
        TODO: documentação
        */
        inline void generate_expr(const node::Expr* expr) {
            struct ExprVisitor {
                Generator& generator;
                void operator()(const node::Term* term) {
                    generator.generate_term(term);
                }
                void operator()(const node::BinExpr* bin_expr) {
                    generator.generate_bin_expr(bin_expr);
                }
            };

            ExprVisitor visitor {.generator = *this};
            std::visit(visitor, expr->variant_expr);
        }

        /*
        TODO: documentação
        */
        inline void generate_bin_expr(const node::BinExpr* bin_expr) {
            struct BinExprVisitor {
                Generator& generator;
                void operator()(const node::BinExprSoma* soma) {
                    generator.generate_expr(soma->lado_esquerdo);
                    generator.generate_expr(soma->lado_direito);
                    generator.pop("rax");
                    generator.pop("rbx");
                    generator.m_out << "    add rax, rbx\n";
                    generator.push("rax");
                }
                void operator()(const node::BinExprSub* subtracao)
                {
                    generator.generate_expr(subtracao->lado_esquerdo);
                    generator.generate_expr(subtracao->lado_direito);
                    generator.pop("rax");
                    generator.pop("rbx");
                    generator.m_out << "    sub rbx, rax\n";
                    generator.push("rbx");
                }
                void operator()(const node::BinExprMulti* multiplicacao) {
                    generator.generate_expr(multiplicacao->lado_esquerdo);
                    generator.generate_expr(multiplicacao->lado_direito);
                    generator.pop("rax");
                    generator.pop("rbx");
                    generator.m_out << "    mul rbx\n";
                    generator.push("rax");
                }
                void operator()(const node::BinExprDiv* divisao) {
                    generator.generate_expr(divisao->lado_esquerdo);
                    generator.generate_expr(divisao->lado_direito);
                    generator.pop("rbx"); 
                    generator.pop("rax"); 
                    generator.m_out << "    div rbx\n";
                    generator.push("rax");
                }
            };

            BinExprVisitor visitor {.generator = *this};
            std::visit(visitor, bin_expr->variant_bin_expr);
        }

        /*
        TODO: documentação
        */
        inline void generate_scope(const node::Scope* scope) {
            begin_scope();
            for (const node::Statmt* statmt : scope->statmts_scope) {
                generate_statmt(statmt);
            }
            end_scope();
        }

        /*
        TODO: documentação
        */
        inline void generate_statmt(const node::Statmt* statmt) {
            struct StatmtVisitor {
                Generator& generator;
                void operator()(const node::StatmtExit* statmt_exit) {
                    generator.generate_expr(statmt_exit->expr);
                    generator.m_out << "    mov rax, 60\n";
                    generator.pop("rdi");
                    generator.m_out << "    syscall\n";
                }
                void operator()(const node::StatmtVar* statmt_var) {
                    struct VarVisitor {
                        void operator()(const node::NewVar* new_var) {
                            if (new_var->token_identif.valor.has_value()) {
                                if (!generator->m_variables.contains(new_var->token_identif.valor.value())) {
                                    Variable nova_var_temp = {.stack_pos = generator->m_stack_size};
                                    generator->m_variables.insert({new_var->token_identif.valor.value(), nova_var_temp}); // apenas guardo posição da variável/valor na stack
                                    generator->generate_expr(new_var->expr);                                        // após identificador encontramos uma expressão, seja essa um inteiro ou otura variável
                                }
                                else {
                                    std::cerr << "Identificador '" << new_var->token_identif.valor.value() << "' já utilizado." << std::endl;
                                    exit(EXIT_FAILURE);
                                }
                            }
                        }
                        void operator()(const node::ReassVar* reass_var) {
                            if (reass_var->token_identif.valor.has_value()) {
                                
                            }
                        }
                    };
                    std::visit(VarVisitor {}, statmt_var->variant_var);


                    if (statmt_var->token_identif.valor.has_value()) {
                        if (!generator.m_variables.contains(statmt_var->token_identif.valor.value())) {
                            Variable new_var = {.stack_pos = generator.m_stack_size};
                            generator.m_variables.insert({statmt_var->token_identif.valor.value(), new_var}); // apenas guardo posição da variável/valor na stack
                            generator.generate_expr(statmt_var->expr); // após identificador encontramos uma expressão, seja essa um inteiro ou otura variável
                        } else {
                            std::cerr << "Identificador '" << statmt_var->token_identif.valor.value() << "' já utilizado." << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                void operator()(const node::Scope* scope) {
                    generator.generate_scope(scope);
                }
                void operator()(const node::StatmtIf* statmt_if) {
                    generator.generate_expr(statmt_if->expr);
                    generator.pop("rax");
                    std::string label = generator.create_label();
                    generator.m_out << "    test rax, rax\n";
                    generator.m_out << "    jz " << label << "\n";
                    generator.generate_scope(statmt_if->scope);
                    generator.m_out << label << ":\n";
                }
            };

            StatmtVisitor visitor {.generator = *this};
            std::visit(visitor, statmt->variant_statmt);
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
                for (const node::Statmt* statmt : m_program.statmts) {   
                    generate_statmt(statmt);
                }
            m_out << "    mov rax, 60\n"; //código da expressão de saída para o assembly
            m_out << "    mov rdi, 0\n"; // código de saída do programa
            m_out << "    syscall\n";
            return m_out.str();
        }


    private:
        const node::Program m_program;
        std::stringstream m_out;
        size_t m_stack_size = 0;
        std::map<std::string, Variable> m_variables;
        std::vector<size_t> m_scopes;
        int m_label_count = 0;

        /*
        TODO: documentação
        */
        inline void push(const std::string& reg) {
            m_out << "    push " << reg << '\n';
            m_stack_size++;
        }

        /*
        TODO: documentação
        */
        inline void pop(const std::string& reg) {
            m_out << "    pop " << reg << '\n';
            m_stack_size--;
        }

        /*
        TODO: documentação
        */
        inline void begin_scope() {
            m_scopes.push_back(m_variables.size());
        }

        /*
        TODO: documentação
        */
        inline void end_scope() {
            size_t num_pops = m_variables.size() - m_scopes.back();
            m_out << "    add rsp, " << num_pops * 8 << "\n";
            m_stack_size -= num_pops;
            for (int i = 0; i < num_pops; i ++) {
                m_variables.erase(m_variables.rbegin()->first);
            }
            m_scopes.pop_back();
        }

        /*
        TODO: documentação
        */
        inline std::string create_label() {
            return "label" + std::to_string(m_label_count++);
        }
};