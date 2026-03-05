#include <gtest/gtest.h>
#include <sstream>
#include <memory>

#include "../include/Lexer.hpp"

TEST(LexerTest, TokenizesAssignment)
{
    auto stream = std::make_unique<std::istringstream>("x := 10;");
    WhileParser::Lexer lex(std::move(stream), true, true);
    WhileParser::Token t = lex.nextToken();

    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "x");

    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::ASSIGN);
}