#include "../include/Lexer.hpp"

namespace WhileParser
{
    Lexer::Lexer(const std::string &filename)
    {

        if (filename.size() < 4 || filename.substr(filename.size() - 3, 3) != ".wh")
        {
            throw std::invalid_argument("The filename has to be at least 1 charachter and has to have extension .wh");
        }

        m_file_stream.open(filename, std::ios_base::in);

        if (!m_file_stream.is_open())
        {
            throw std::runtime_error("Impossible to open the file");
        }

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
    }

    Lexer::~Lexer()
    {
        if (m_file_stream.is_open())
            m_file_stream.close();
    }

    bool Lexer::isIdChar(char c) const
    {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    }

    Token Lexer::readIdentifierOrKeyword(char first)
    {
        std::string word(1, first);

        while (m_file_stream.peek() != EOF && isIdChar(m_file_stream.peek()))
        {
            word += static_cast<char>(m_file_stream.get());
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
        while (std::isdigit(m_file_stream.peek()))
        {
            word += static_cast<char>(m_file_stream.get());
        }

        if (std::isalpha(m_file_stream.peek()))
        {
            word += static_cast<char>(m_file_stream.get());
            throw std::runtime_error("Invalid identifier: " + word);
        }

        return {TokenType::NUMBER, std::move(word)};
    }

    Token Lexer::readSymbol(char first)
    {
        std::string word(1, first);

        const std::string compound_symbol = {first, static_cast<char>(m_file_stream.peek())};
        // check for compound symbols :=, >=, <=.
        // if the next symbol is a whitespace or in general not a = the instruction below will return 0
        // so we know we're analyzing a single-character symbol
        if (m_keywords.count(compound_symbol))
        {
            word += static_cast<char>(m_file_stream.get());
        }

        if (auto it = m_keywords.find(word); it != m_keywords.end())
        {
            return {it->second, std::move(word)};
        }

        return {TokenType::UNKNOWN, std::move(word)};
    }

    Token Lexer::nextToken()
    {

        char c;
        // empty file
        if (!m_file_stream.get(c))
        {
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
        return m_file_stream.peek() != EOF;
    }
}