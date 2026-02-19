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

    Token Lexer::nextToken()
    {
        std::string word;
        char c;
        while (m_file_stream.get(c) && c != std::char_traits<char>::eof())
        {
            word += c;

            const auto it = m_keywords.find(word);

            // matching non alphanumeric symbols
            if (!isalnum(c))
            {
                if ((c == '>' || c == '<' || c == ':') && m_file_stream.peek() == '=')
                    continue;

                // special case for underscore, that is part of identifier
                if (c == '_')
                    continue;

                if (it == m_keywords.end())
                    throw std::runtime_error("The following token is not part of the language: " + word);
            }

            if (isdigit(c))
            {
                if (isdigit(m_file_stream.peek()))
                    continue;
                return Token(TokenType::NUMBER, word);
            }

            // matching keywords
            if (it != m_keywords.end())
                return Token(it->second, word);

            // fallback to identifier
            if (!isalnum(m_file_stream.peek()) && std::regex_match(word, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*")))
            {
                return Token(TokenType::IDENTIFIER, word);
            }
        }

        return Token(TokenType::END_OF_FILE, "EOL");
    }

    bool Lexer::isTokenAvailable()
    {
        return m_file_stream.peek() != std::char_traits<char>::eof();
    }
}