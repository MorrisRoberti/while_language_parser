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

    private:
        TokenType m_token_type;
        std::string m_value;
    };
}

#endif