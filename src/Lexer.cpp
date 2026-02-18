#include "../include/Lexer.hpp"
#include "Lexer.hpp"

namespace WhileParser
{
    Lexer::Lexer(const std::string &filename)
    {

        if (filename.size() < 4 || filename.substr(filename.size() - 4, 3) != ".wh")
        {
            throw std::invalid_argument("The filename has to be at least 1 charachter and has to have extension .wh");
        }

        m_file_stream = std::ifstream(filename, std::ios_base::in);

        if (!m_file_stream.is_open())
        {
            throw std::runtime_error("Impossible to open the file");
        }

        m_rules = std::vector<TokenRule>{
            {std::regex("^skip"), TokenType::SKIP},

            {std::regex("^if"), TokenType::IF},
            {std::regex("^then"), TokenType::THEN},
            {std::regex("^else"), TokenType::ELSE},
            {std::regex("^endif"), TokenType::ENDIF},

            {std::regex("^while"), TokenType::WHILE},
            {std::regex("^do"), TokenType::DO},
            {std::regex("^endwhile"), TokenType::ENDWHILE},

            {std::regex("^true"), TokenType::TRUE},
            {std::regex("^false"), TokenType::FALSE},
            {std::regex("^and"), TokenType::AND},
            {std::regex("^or"), TokenType::OR},
            {std::regex("^not"), TokenType::NOT},

            {std::regex("^:="), TokenType::ASSIGN},
            {std::regex("^<="), TokenType::LTE},
            {std::regex("^>="), TokenType::GTE},

            {std::regex("^[a-zA-Z_][a-zA-Z0-9_]*"), TokenType::IDENTIFIER},
            {std::regex("^[0-9]+"), TokenType::NUMBER},
            {std::regex("^\\+"), TokenType::PLUS},
            {std::regex("^-"), TokenType::MINUS},
            {std::regex("^;"), TokenType::SEMICOLON},
            {std::regex("^<"), TokenType::LT},
            {std::regex("^>"), TokenType::GT},
            {std::regex("^="), TokenType::EQ}};
    }

    Lexer::~Lexer()
    {
        if (m_file_stream.is_open())
            m_file_stream.close();
    }

    Token Lexer::nextToken()
    {

        // reads the next word and finds the correspondence with a token
        std::string word;
        m_file_stream.getline(word.data(), sizeof(word), ' ');

        const auto it = std::find(m_rules.begin(), m_rules.end(), [word, this](int &i)
                                  { return std::regex_match(word, m_rules[i].getPattern()); });

        if (it == m_rules.end())
            throw std::runtime_error("The following token does not match any syntax rule: " + word);

        return Token(it->getTokenType(), word);
    }
}