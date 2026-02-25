#ifndef HH_LEXER_INCLUDE_GUARD
#define HH_LEXER_INCLUDE_GUARD 1

#include "./Token.hpp"
#include "./TokenType.hpp"

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <fstream>
#include <string>

namespace WhileParser
{

    class Lexer
    {
    public:
        Lexer(const std::string &filename);
        ~Lexer();

        Token nextToken();

        bool isTokenAvailable();

        Lexer(const Lexer &other) {}
        Lexer(const Lexer &&) {}
        Lexer &operator=(const Lexer &) = delete;
        Lexer &&operator=(const Lexer &&) = delete;

    private:
        bool isIdChar(char c) const;
        Token readIdentifierOrKeyword(char first);
        Token readNumber(char first);
        Token readSymbol(char first);

        std::ifstream m_file_stream;
        bool m_eof;
        std::unordered_map<std::string, TokenType> m_keywords;
    };
}

#endif