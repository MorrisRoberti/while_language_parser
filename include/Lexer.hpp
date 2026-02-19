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

    class TokenRule
    {

    public:
        TokenRule(const std::regex &pattern, TokenType token_type) : m_pattern(pattern), m_token_type(token_type) {}

        inline const std::regex &getPattern()
        {
            return m_pattern;
        }

        inline TokenType getTokenType()
        {
            return m_token_type;
        }

    private:
        std::regex m_pattern;
        TokenType m_token_type;
    };

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