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
            if (buffer.compare("69") == 0) {
                tokens.push_back({.tipo = TipoToken::int_lit, .value = buffer});
                buffer.clear();
                continue;
            } else {
                std::cerr << "Erro na tokenização do código de retorno do arquivo .ml" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (str[i] == ';') {
            token.push_back({.tipo = TipoToken::semicolon});
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

int main(int argc, char* argv[]) {
    //garantindo que o programa tenha apenas dois argumentos: argv[0] = path do arquivo em C e argv[1] = path do arquivo .ml
    if (argc != 2) {
        std::cerr << "Uso incorreto do compilador. O uso correto seria..." << std::endl << "language <input.ml>" << std::endl;
        return EXIT_FAILURE;
    }

    //preparando para ler conteúdos do arquivo .ml como string
    std::fstream fs (argv[1], std::ios::in);
    std::stringstream conteudo_stream;
    conteudo_stream << fs.rdbuf();
    fs.close();
    std::string conteudo_arquivo = conteudo_stream.str();

    std::vector<Token> teste = tokenization(conteudo_arquivo);

    return EXIT_SUCCESS;
}