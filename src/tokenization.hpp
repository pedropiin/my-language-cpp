#pragma once

#include <optional>
#include <vector>
#include <string>

enum class TipoToken {
    _exit,
    int_lit,
    ponto_virgula
};

struct Token {
    TipoToken tipo;
    std::optional<std::string> valor;
};

class Tokenizador {
    public:
        inline Tokenizador(const std::string src) 
            : m_src(src) 
        {}

        /*
        Função que realiza a análise lexical/tokenização lexical
        passando por todo o conteúdo do arquivo .ml (tido como uma string)
        e reconhecendo palavras chaves.
        PARÂMETROS:
        - str (std::string): string com todo o conteúdo do arquivo .ml a ser compilado
        RETURNS:
        - tokens (std::vector<Token): vetor de tokens do arquivo .ml
        */
        inline std::vector<Token> tokenizar() {
            std::vector<Token> tokens;
            std::string buffer;

            while (peak().has_value()) {
                if (std::isalpha(peak().value())) { //estamos garantindo que peak() vai retornar um caracter
                    buffer.push_back(consume());
                    while (peak().has_value() && std::isalnum(peak().value())) {
                        buffer.push_back(consume());
                    }
                    if (buffer.compare("exit") == 0) {
                        tokens.push_back({.tipo = TipoToken::_exit});
                        buffer.clear();
                        continue;
                    } else {
                        std::cerr << "Erro com a tokenização da chamada de saída." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                } else if (std::isdigit(peak().value())) {
                    buffer.push_back(consume());
                    while (peak().has_value() && std::isalnum(peak().value())) {
                        buffer.push_back(consume());
                    }
                    tokens.push_back({.tipo = TipoToken::int_lit, .valor = buffer});
                    buffer.clear();
                    continue;
                } else if (peak().value() == ';') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::ponto_virgula});
                    continue;
                } else if (std::isspace(peak().value())) {
                    consume();
                    continue;
                } else {
                    std::cerr << "Erro." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            m_index = 0;
            return tokens;
        }


    private:
        const std::string m_src;
        int m_index = 0;

        /*
        Método que "olha" o próximo indíce da string para
        ver se chegou ao seu fim, ou se é um caracter válido
        PARÂMETROS:
        - num_chars (int): número de caracteres que a função 
        vai analisar a frente do índice atual. Por padrão é
        settado como = 1.
        RETURNS:
        - m_src[m_index] (std::optional<char>): caracter no 
        índice de análise.
        */
        inline std::optional<char> peak(int num_chars = 1) const {
            if (m_index + num_chars > m_src.length()) {
                return {};
            } else {
                return m_src[m_index];
            }
        }

        /*
        Método que retorna o caracter no índice atual e
        incrementa o índice com +1.
        PARÂMETROS:
        RETURNS:
        - c (char): caracter no índice atual.
        */
        inline char consume() {
            char c = m_src[m_index];
            m_index++;
            return c;
        }
};