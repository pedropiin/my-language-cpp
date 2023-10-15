#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "parser.hpp"

class Gerador {
    public:
        inline Gerador(node::Program program) 
            : m_program(std::move(program))
        {}

        /*Função que converte os tokens obtidos pela tokenização
        no respectivo código em assembly.
        PARÂMETROS:
        - tokens_file (std::vector<Token>): vetor de tokens lexicais
        do arquivo .ml
        RETURNS:
        - out (std::string): formato em string de uma stringstream
        que contêm o código em asm correspondente ao código em ml
        */
        inline std::string gerar_asm() const {
            std::stringstream out;
            out << "global _start\n_start:\n";
            out << "    mov rax, 60\n    mov rdi, " << m_program.expr.token.valor.value() << '\n' << "    syscall\n";
            return out.str();
        }


    private:
        const node::Program m_program;
};