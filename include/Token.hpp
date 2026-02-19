#ifndef HH_TOKEN_INCLUDE_GUARD
#define HH_TOKEN_INCLUDE_GUARD 1

#include "./TokenType.hpp"
#include <string>

namespace WhileParser
{

    class Token
    {
    public:
        Token(TokenType type, const std::string &value) : m_token_type(type), m_value(value) {}

        Token(const Token &) = delete;
        Token(const Token &&) = delete;

        inline TokenType getType()
        {
            return m_token_type;
        }

        inline const std::string &getValue()
        {
            return m_value;
        }

        inline const std::string getTokenTypeString()
        {

            switch (m_token_type)
            {
            case TokenType::UNKNOWN:
                return "UNKNOWN";
            case TokenType::IDENTIFIER:
                return "IDENTIFIER";
            case TokenType::WHITESPACE:
                return "WHITESPACE";
            case TokenType::NUMBER:
                return "NUMBER";
            case TokenType::SKIP:
                return "SKIP";
            case TokenType::IF:
                return "IF";
            case TokenType::THEN:
                return "THEN";
            case TokenType::ELSE:
                return "ELSE";
            case TokenType::ENDIF:
                return "ENDIF";
            case TokenType::WHILE:
                return "WHILE";
            case TokenType::DO:
                return "DO";
            case TokenType::ENDWHILE:
                return "ENDWHILE";
            case TokenType::TRUE:
                return "TRUE";
            case TokenType::FALSE:
                return "FALSE";
            case TokenType::SEMICOLON:
                return "SEMICOLON";
            case TokenType::ASSIGN:
                return "ASSIGN";
            case TokenType::EQ:
                return "EQ";
            case TokenType::LT:
                return "LT";
            case TokenType::LTE:
                return "LTE";
            case TokenType::GT:
                return "GT";
            case TokenType::GTE:
                return "GTE";
            case TokenType::PLUS:
                return "PLUS";
            case TokenType::MINUS:
                return "MINUS";
            case TokenType::WILDCARD:
                return "WILDCARD";
            case TokenType::SLASH:
                return "SLASH";
            case TokenType::AND:
                return "AND";
            case TokenType::OR:
                return "OR";
            case TokenType::NOT:
                return "NOT";
            case TokenType::LPAREN:
                return "LPAREN";
            case TokenType::RPAREN:
                return "RPAREN";
            case TokenType::END_OF_FILE:
                return "END_OF_FILE";
            case TokenType::END_OF_LINE:
                return "END_OF_LINE";

            default:
                return "UNKNOWN_ENUM_VALUE";
            }
        }

    private:
        TokenType m_token_type;
        std::string m_value;
    };
}

#endif