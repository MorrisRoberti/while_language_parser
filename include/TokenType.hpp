#ifndef HH_TOKEN_TYPE_INCLUDE_GUARD
#define HH_TOKEN_TYPE_INCLUDE_GUARD 1

namespace WhileParser
{
    enum class TokenType
    {
        UNKNOWN,
        IDENTIFIER,
        WHITESPACE,
        NUMBER,
        SKIP,
        IF,
        THEN,
        ELSE,
        ENDIF,
        WHILE,
        DO,
        ENDWHILE,
        TRUE,
        FALSE,
        SEMICOLON,
        ASSIGN,
        EQ,
        LT,
        LTE,
        GT,
        GTE,
        PLUS,
        MINUS,
        WILDCARD,
        SLASH,
        AND,
        OR,
        NOT,
        LPAREN,
        RPAREN,
        END_OF_FILE,
        END_OF_LINE
    };
}

#endif