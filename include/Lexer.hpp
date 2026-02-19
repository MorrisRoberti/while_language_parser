#ifndef HH_LEXER_INCLUDE_GUARD
#define HH_LEXER_INCLUDE_GUARD 1

#include "./Token.hpp"
#include "./TokenType.hpp"

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <regex>
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

        Lexer(const Lexer &) = delete;
        Lexer(const Lexer &&) = delete;
        Lexer &operator=(const Lexer &) = delete;
        Lexer &&operator=(const Lexer &&) = delete;

    private:
        std::ifstream m_file_stream;
        std::unordered_map<std::string, TokenType> m_keywords;
    };
}

#endif