#ifndef HH_LEXER_INCLUDE_GUARD
#define HH_LEXER_INCLUDE_GUARD 1

#include "./Token.hpp"
#include "./TokenType.hpp"

#include <iostream>
#include <memory>
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
        Lexer(const std::string &filename, bool skip_whitespaces, bool skip_eol);
        Lexer(std::unique_ptr<std::istream> raw_code, bool skip_whitespaces, bool skip_eol)
        {
            init(std::move(raw_code), skip_whitespaces, skip_eol);
        }
        ~Lexer() = default;

        Token nextToken();

        bool isTokenAvailable();
        void skipWhitespaces();
        void skipEOL();

        Lexer(const Lexer &other) {}
        Lexer(const Lexer &&) {}
        Lexer &operator=(const Lexer &) = delete;
        Lexer &&operator=(const Lexer &&) = delete;

    private:
        bool isIdChar(char c) const;
        Token readIdentifierOrKeyword(char first);
        Token readNumber(char first);
        Token readSymbol(char first);

        void init(std::unique_ptr<std::istream> source, bool skip_whitespaces, bool skip_eol);

        std::unique_ptr<std::istream> m_stream;
        std::unordered_map<std::string, TokenType> m_keywords;
        bool m_skip_whitespaces;
        bool m_skip_eol;
        bool m_eof;
    };
}

#endif