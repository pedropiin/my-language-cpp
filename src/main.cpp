#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>


enum class TipoToken {
    _return,
    int_lit,
    semicolon
};

struct Token {
    TipoToken tipo;
    std::optional<std::string> valor;
};


/*Função que realiza a análise lexical/tokenização lexical
passando por todo o conteúdo do arquivo .ml (tido como uma string)
e reconhecendo palavras chaves.*/
std::vector<Token> tokenization(std::string str) {
    std::vector<Token> tokens;
    std::string buffer;

    for (int i = 0; i < str.length(); i++) {
        if (std::isalpha(str[i])) {
            buffer.push_back(str[i]);
            i++;
            while (std::isalnum(str[i])) {
                buffer.push_back(str[i]);
                i++;
            }
            i--;
            if (buffer.compare("return") == 0) {
                tokens.push_back({.tipo = TipoToken::_return});
                buffer.clear();
                continue;
            } else {
                std::cerr << "Erro na tokenização do return do arquivo .ml" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (std::isdigit(str[i])) {
            buffer.push_back(str[i]);
            i++;
            while (std::isdigit(str[i])) {
                buffer.push_back(str[i]);
                i++;
            }
            i--;
            if (buffer.compare("5") == 0) {
                tokens.push_back({.tipo = TipoToken::int_lit, .valor = buffer});
                buffer.clear();
                continue;
            } else {
                std::cerr << "Erro na tokenização do código de retorno do arquivo .ml" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (str[i] == ';') {
            tokens.push_back({.tipo = TipoToken::semicolon});
            continue;
        } else if (std::isspace(str[i])) {
            continue;
        } else {
            std::cerr << "Erro na tokenização geral do arquivo .ml" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}

/*Função que converte os tokens obtidos pela
tokenização no respectivo código em assembly*/
std::string tokens_to_asm(std::vector<Token> tokens_file) {
    std::stringstream out;
    out << "global _start\n_start:\n";
    for (int i = 0; i < tokens_file.size(); i++) {
        Token token = tokens_file[i];
        switch (tokens_file[i].tipo) {
            case TipoToken::_return:
                if ((i + 1) < tokens_file.size() && tokens_file[i + 1].tipo == TipoToken::int_lit) {
                    if ((i + 2) < tokens_file.size() && tokens_file[i + 2].tipo == TipoToken::semicolon) {
                        out << "    mov rax, 60\n    mov rdi, " << tokens_file[i + 1].valor.value() << '\n' << "    syscall\n";
                    }
                }
                break;
            case TipoToken::int_lit:
                break;
            case TipoToken::semicolon:
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
    std::vector<Token> tokens = tokenization(conteudo_arquivo);
    std::string conteudo_asm = tokens_to_asm(tokens);

    //criando e escrevendo em um arquivo nosso código em assembly
    std::fstream fs_out ("./out.asm", std::ios::out);
    fs_out << conteudo_asm;
    fs_out.close();

    system("nasm -felf64 ./out.asm");
    system("ld -o out out.o");
    

    return EXIT_SUCCESS;
}