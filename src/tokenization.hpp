#pragma once

#include <optional>
#include <vector>
#include <string>

enum class TipoToken {
    _exit,
    int_lit,
    semicolon
};

struct Token {
    TipoToken tipo;
    std::optional<std::string> valor;
}

class Tokenizador {
    public:
        inline Tokenizador (const std::string fstring) {
            m_fstring(fstring);
        }

        /*Função que realiza a análise lexical/tokenização lexical
        passando por todo o conteúdo do arquivo .ml (tido como uma string)
        e reconhecendo palavras chaves.
        PARÂMETROS:
        - str (std::string): string com todo o conteúdo do arquivo .ml a ser compilado
        RETURNS:
        - tokens (std::vector<Token): vetor de tokens do arquivo .ml
        */
        inline std::vector<Token> tokenizar(std::string str) {
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
                    if (buffer.compare("exit") == 0) {
                        tokens.push_back({.tipo = TipoToken::_exit});
                        buffer.clear();
                        continue;
                    } else {
                        std::cerr << "Erro na tokenização da saída do arquivo .ml" << std::endl;
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

    private:
        const std::string m_fstring;
}