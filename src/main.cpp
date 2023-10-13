#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "./tokenization.hpp"

/*Função que converte os tokens obtidos pela
tokenização no respectivo código em assembly.
PARÂMETROS:
- tokens_file (std::vector<Token>): vetor de tokens lexicais
do arquivo .ml
RETURNS:
- out (std::string): formato em string de uma stringstream
que contêm o código em asm correspondente ao código em ml
*/
std::string tokens_to_asm(std::vector<Token> tokens_file) {
    std::stringstream out;
    out << "global _start\n_start:\n";
    for (int i = 0; i < tokens_file.size(); i++) {
        Token token = tokens_file[i];
        switch (tokens_file[i].tipo) {
            case TipoToken::_exit:
                if ((i + 1) < tokens_file.size() && tokens_file[i + 1].tipo == TipoToken::int_lit) {
                    if ((i + 2) < tokens_file.size() && tokens_file[i + 2].tipo == TipoToken::ponto_virgula) {
                        out << "    mov rax, 60\n    mov rdi, " << tokens_file[i + 1].valor.value() << '\n' << "    syscall\n";
                    }
                }
                break;
            case TipoToken::int_lit:
                break;
            case TipoToken::ponto_virgula:
                break;
        }
    }
    return out.str();
}

int main(int argc, char* argv[]) {
    //garantindo que o programa tenha apenas dois argumentos: argv[0] = path do arquivo em C e argv[1] = path do arquivo .ml
    if (argc != 2) {
        std::cerr << "Uso incorreto do compilador. O uso correto seria..." << std::endl << "language <input.ml>" << std::endl;
        return EXIT_FAILURE;
    }

    //preparando para ler conteúdos do arquivo .ml como string
    std::fstream fs_in (argv[1], std::ios::in);
    std::stringstream conteudo_stream;
    conteudo_stream << fs_in.rdbuf();
    fs_in.close();
    std::string conteudo_arquivo = conteudo_stream.str();

    //realizando tokenização no arquivo e convertendo em assembly
    Tokenizador tokenizador(std::move(conteudo_arquivo));
    std::vector<Token> tokens = tokenizador.tokenizar();
    std::string conteudo_asm = tokens_to_asm(tokens);

    //criando e escrevendo em um arquivo nosso código em assembly
    std::fstream fs_out ("./out.asm", std::ios::out);
    fs_out << conteudo_asm;
    fs_out.close();

    system("nasm -felf64 ./out.asm");
    system("ld -o out out.o");
    

    return EXIT_SUCCESS;
}