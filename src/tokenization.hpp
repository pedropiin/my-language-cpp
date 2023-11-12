#pragma once

#include <optional>
#include <vector>
#include <string>

enum class TipoToken {
    _exit,
    int_lit,
    ponto_virgula,
    parenteses_abre,
    parenteses_fecha,
    identif,
    var,
    igual,
    mais,
    menos,
    asterisco,
    barra_div,
    chaves_abre,
    chaves_fecha
};

//TODO: implementar tipos diferentes de enum para
//tokens de diferentes funções

struct Token {
    TipoToken tipo;
    std::optional<std::string> valor;
};

inline std::optional<int> bin_prec(TipoToken tipo) {
    switch(tipo) {
        case TipoToken::mais:
            return 0;
        case TipoToken::menos:
            return 0;
        case TipoToken::asterisco:
            return 1;
        case TipoToken::barra_div:
            return 1;
        default:
            return {};
    }
}

class Tokenizer {
    public:
        inline Tokenizer(const std::string src) 
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
        inline std::vector<Token> tokenize() {
            std::vector<Token> tokens;
            std::string buffer;

            while (peek().has_value()) {
                if (std::isalpha(peek().value())) { //estamos garantindo que peek() vai retornar um caracter
                    buffer.push_back(consume());
                    while (peek().has_value() && std::isalnum(peek().value())) {
                        buffer.push_back(consume());
                    }
                    if (buffer.compare("exit") == 0) {
                        tokens.push_back({.tipo = TipoToken::_exit});
                        buffer.clear();
                    } else if (buffer.compare("var") == 0) {
                        tokens.push_back({.tipo = TipoToken::var});
                        buffer.clear();
                    } else {
                        tokens.push_back({.tipo = TipoToken::identif, .valor = buffer});
                        buffer.clear();
                    }
                } else if (std::isdigit(peek().value())) {
                    buffer.push_back(consume());
                    while (peek().has_value() && std::isalnum(peek().value())) {
                        buffer.push_back(consume());
                    }
                    tokens.push_back({.tipo = TipoToken::int_lit, .valor = buffer});
                    buffer.clear();
                } else if (peek().value() == '=') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::igual});
                } else if (peek().value() == '+') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::mais});
                } else if (peek().value() == '-') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::menos});
                } else if (peek().value() == '*') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::asterisco});
                } else if (peek().value() == '/') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::barra_div});
                } else if (peek().value() == '(') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::parenteses_abre});
                } else if (peek().value() == ')') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::parenteses_fecha});
                } else if (peek().value() == ';') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::ponto_virgula});
                } else if (peek().value() == '{') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::chaves_abre});
                } else if (peek().value() == '}') {
                    consume();
                    tokens.push_back({.tipo = TipoToken::chaves_fecha});
                } else if (std::isspace(peek().value())) {
                    consume();
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
        - offset (int): número de caracteres que a função 
        vai analisar a frente do índice atual. Por padrão é
        settado como = 0.
        RETURNS:
        - m_src[m_index] (std::optional<char>): caracter no 
        índice de análise.
        */
        inline std::optional<char> peek(int offset = 0) const {
            if (m_index + offset >= m_src.length()) {
                return {};
            } else {
                return m_src[m_index + offset];
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