#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "./tokenization.hpp"
#include "./parser.hpp"
#include "gerador.hpp"


int main(int argc, char* argv[]) {
    //garantindo que o programa tenha apenas dois argumentos: argv[0] = path do arquivo em C e argv[1] = path do arquivo .ml
    if (argc != 2) {
        std::cerr << "Uso incorreto do compilador. O uso correto seria..." << std::endl << "compiler <input.ml>" << std::endl;
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
    Parser parser(std::move(tokens));
    std::optional<node::Exit> arvore = parser.parse();
    if (!arvore.has_value()) {
        std::cerr << "Nenhuma operação de saída." << std::endl;
        exit(EXIT_FAILURE);
    }
    Gerador gerador(arvore.value());
    std::string conteudo_asm = gerador.gerar_asm();


    //criando e escrevendo em um arquivo nosso código em assembly
    std::fstream fs_out ("./out.asm", std::ios::out);
    fs_out << conteudo_asm;
    fs_out.close();

    //passando arquivos gerados pelo assembler e linker
    system("nasm -felf64 ./out.asm");
    system("ld -o out out.o");
    

    return EXIT_SUCCESS;
}