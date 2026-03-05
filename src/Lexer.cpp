#include "../include/Lexer.hpp"
#include "Lexer.hpp"

namespace WhileParser
{
    void Lexer::init(std::unique_ptr<std::istream> source, bool skip_whitespaces, bool skip_eol)
    {

        m_keywords = std::unordered_map<std::string, TokenType>{
            {"skip", TokenType::SKIP},
            {" ", TokenType::WHITESPACE},
            {"\n", TokenType::END_OF_LINE},
            {"if", TokenType::IF},
            {"then", TokenType::THEN},
            {"else", TokenType::ELSE},
            {"endif", TokenType::ENDIF},
            {"while", TokenType::WHILE},
            {"do", TokenType::DO},
            {"endwhile", TokenType::ENDWHILE},
            {"true", TokenType::TRUE},
            {"false", TokenType::FALSE},
            {"and", TokenType::AND},
            {"or", TokenType::OR},
            {"not", TokenType::NOT},
            {":=", TokenType::ASSIGN},
            {"<=", TokenType::LTE},
            {">=", TokenType::GTE},
            {";", TokenType::SEMICOLON},
            {"+", TokenType::PLUS},
            {"-", TokenType::MINUS},
            {"<", TokenType::LT},
            {">", TokenType::GT},
            {"=", TokenType::EQ},
            {"(", TokenType::LPAREN},
            {")", TokenType::RPAREN},
            {"*", TokenType::WILDCARD},
            {"/", TokenType::SLASH}};

        m_eof = false;
        m_skip_whitespaces = skip_whitespaces;
        m_skip_eol = skip_eol;

        // initializing my stream with the correct stream
        m_stream = std::move(source);
    }

    Lexer::Lexer(const std::string &filename, bool skip_whitespaces = false, bool skip_eol = false)
    {

        if (filename.size() < 4 || filename.substr(filename.size() - 3, 3) != ".wh")
        {
            throw std::invalid_argument("The filename has to be at least 1 charachter and has to have extension .wh");
        }

        auto file = std::make_unique<std::ifstream>(filename);

        if (!file->is_open())
        {
            throw std::runtime_error("Impossible to open the file");
        }

        init(std::move(file), skip_whitespaces, skip_eol);
    }

    bool Lexer::isIdChar(char c) const
    {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    }

    Token Lexer::readIdentifierOrKeyword(char first)
    {
        std::string word(1, first);

        while (m_stream->peek() != EOF && isIdChar(m_stream->peek()))
        {
            word += static_cast<char>(m_stream->get());
        }

        // found a keyword
        if (auto it = m_keywords.find(word); it != m_keywords.end())
        {
            return {it->second, word};
        }

        // check identifier validity
        if (std::isdigit(word[0]))
        {
            throw std::runtime_error("Invalid identifier: " + word);
        }

        return {TokenType::IDENTIFIER, std::move(word)};
    }

    Token Lexer::readNumber(char first)
    {
        std::string word(1, first);
        while (std::isdigit(m_stream->peek()))
        {
            word += static_cast<char>(m_stream->get());
        }

        if (std::isalpha(m_stream->peek()))
        {
            word += static_cast<char>(m_stream->get());
            throw std::runtime_error("Invalid identifier: " + word);
        }

        return {TokenType::NUMBER, std::move(word)};
    }

    Token Lexer::readSymbol(char first)
    {

        std::string word(1, first);

        const std::string compound_symbol = {first, static_cast<char>(m_stream->peek())};
        // check for compound symbols :=, >=, <=.
        // if the next symbol is a whitespace or in general not a = the instruction below will return 0
        // so we know we're analyzing a single-character symbol

        if (m_keywords.count(compound_symbol))
        {
            word += static_cast<char>(m_stream->get());
        }

        if (auto it = m_keywords.find(word); it != m_keywords.end())
        {
            // skips whitespaces
            if ((word == " " && m_skip_whitespaces) || (word == "\n" && m_skip_eol))
                return nextToken();

            return {it->second, std::move(word)};
        }

        return {TokenType::UNKNOWN, std::move(word)};
    }

    Token Lexer::nextToken()
    {

        char c;
        // empty file
        if (!m_stream->get(c))
        {
            m_eof = true;
            return {TokenType::END_OF_FILE, "EOF"};
        }

        if (std::isalpha(c) || c == '_')
            return readIdentifierOrKeyword(c);
        if (std::isdigit(c))
            return readNumber(c);

        return readSymbol(c);
    }

    bool Lexer::isTokenAvailable()
    {

        return !m_eof;
    }

    void WhileParser::Lexer::skipWhitespaces()
    {
        m_skip_whitespaces = true;
    }

    void Lexer::skipEOL()
    {
        m_skip_eol = true;
    }
}