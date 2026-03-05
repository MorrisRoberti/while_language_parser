#include <gtest/gtest.h>
#include "../include/Lexer.hpp"

TEST(LexerTest, TokenizesAssignment)
{
    WhileParser::Lexer lex("/home/morris/Workspace/while_parser/program.wh", true);
    WhileParser::Token t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "x");

    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::ASSIGN);
}