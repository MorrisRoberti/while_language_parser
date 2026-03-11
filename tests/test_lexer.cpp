#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include <vector>

#include "../include/Lexer.hpp"

// Helper to tokenize a comparison string
std::vector<WhileParser::TokenType> tokenizeString(const std::string &input)
{
    auto stream = std::make_unique<std::istringstream>(input);
    WhileParser::Lexer lex(std::move(stream), true, true);
    std::vector<WhileParser::TokenType> tokens;

    WhileParser::Token t = lex.nextToken();
    while (t.getType() != WhileParser::TokenType::END_OF_FILE)
    {
        if (t.getType() != WhileParser::TokenType::WHITESPACE)
        {
            tokens.push_back(t.getType());
        }
        t = lex.nextToken();
    }
    return tokens;
}

// 1. Test Base: Assignment and Numbers
TEST(LexerTest, BasicAssignment)
{
    auto stream = std::make_unique<std::istringstream>("x := 10;");
    WhileParser::Lexer lex(std::move(stream), true, true);

    auto t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "x");

    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::ASSIGN);
    EXPECT_EQ(t.getValue(), ":=");

    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::NUMBER);
    EXPECT_EQ(t.getValue(), "10");

    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::SEMICOLON);
}

// 2. Test Keyword and Case Sensitivity
TEST(LexerTest, KeywordsAndCaseSensitivity)
{
    auto stream = std::make_unique<std::istringstream>("while WHILE if IF skip SKIP");
    WhileParser::Lexer lex(std::move(stream), true, true);

    // While (lowercase = keyword, uppercase = identifier)
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::WHILE);
    auto t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "WHILE");

    // If
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::IF);
    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "IF");

    // Skip
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::SKIP);
    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "SKIP");
}

// 3. Test Relational and Logical Operators
TEST(LexerTest, LogicalAndRelational)
{
    auto stream = std::make_unique<std::istringstream>("and or not true false < <= = >= >");
    WhileParser::Lexer lex(std::move(stream), true, true);

    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::AND);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::OR);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::NOT);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::TRUE);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::FALSE);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::LT);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::LTE);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::EQ);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::GTE);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::GT);
}

// 4. Test Parenthesis and Arithmetic
TEST(LexerTest, ArithmeticAndParens)
{
    auto stream = std::make_unique<std::istringstream>("(a + b) * c / d - 5");
    WhileParser::Lexer lex(std::move(stream), true, true);

    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::LPAREN);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::PLUS);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::RPAREN);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::WILDCARD);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::SLASH);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::MINUS);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::NUMBER);
}

// 5. Test Complex Structures (IF/WHILE)
TEST(LexerTest, ControlStructures)
{
    std::string code = "if true then x := 1; else skip; endif";
    auto tokens = tokenizeString(code);

    std::vector<WhileParser::TokenType> expected = {
        WhileParser::TokenType::IF, WhileParser::TokenType::TRUE, WhileParser::TokenType::THEN,
        WhileParser::TokenType::IDENTIFIER, WhileParser::TokenType::ASSIGN, WhileParser::TokenType::NUMBER, WhileParser::TokenType::SEMICOLON,
        WhileParser::TokenType::ELSE, WhileParser::TokenType::SKIP, WhileParser::TokenType::SEMICOLON, WhileParser::TokenType::ENDIF};

    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        EXPECT_EQ(tokens[i], expected[i]) << "Mismatch at index " << i;
    }
}

// 6. Test Unknown characters and EOF
TEST(LexerTest, UnknownAndEOF)
{
    auto stream = std::make_unique<std::istringstream>("@ #");
    WhileParser::Lexer lex(std::move(stream), true, true);

    auto t1 = lex.nextToken();
    EXPECT_EQ(t1.getType(), WhileParser::TokenType::UNKNOWN);
    EXPECT_EQ(t1.getValue(), "@");

    auto t2 = lex.nextToken();
    EXPECT_EQ(t2.getType(), WhileParser::TokenType::UNKNOWN);
    EXPECT_EQ(t2.getValue(), "#");

    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::END_OF_FILE);
}

// 7. Test Identifiers containing keywords (es. "if_var", "endif1")
TEST(LexerTest, KeywordsInsideIdentifiers)
{
    auto stream = std::make_unique<std::istringstream>("if_var then123 endif_ xendif");
    WhileParser::Lexer lex(std::move(stream), true, true);

    auto t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "if_var");

    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "then123");

    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "endif_");

    t = lex.nextToken();
    EXPECT_EQ(t.getType(), WhileParser::TokenType::IDENTIFIER);
    EXPECT_EQ(t.getValue(), "xendif");
}

// 8. Test Sticky Tokens
TEST(LexerTest, StickyTokens)
{
    auto stream = std::make_unique<std::istringstream>("x:=10+y;while(true)do");
    WhileParser::Lexer lex(std::move(stream), true, true);

    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::IDENTIFIER); // x
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::ASSIGN);     // :=
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::NUMBER);     // 10
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::PLUS);       // +
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::IDENTIFIER); // y
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::SEMICOLON);  // ;
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::WHILE);      // while
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::LPAREN);     // (
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::TRUE);       // true
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::RPAREN);     // )
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::DO);         // do
}

// 9. Test Numbers and Identifiers
TEST(LexerTest, InvalidIdentifierStartingWithNumber)
{
    auto stream = std::make_unique<std::istringstream>("123abc456");
    WhileParser::Lexer lex(std::move(stream), true, true);

    auto t1 = lex.nextToken();

    if (t1.getType() == WhileParser::TokenType::UNKNOWN)
    {
        EXPECT_EQ(t1.getType(), WhileParser::TokenType::UNKNOWN);
    }
    else
    {
        // Se invece li separa forzatamente:
        EXPECT_EQ(t1.getType(), WhileParser::TokenType::NUMBER);
        EXPECT_EQ(t1.getValue(), "123");

        auto t2 = lex.nextToken();
        EXPECT_EQ(t2.getType(), WhileParser::TokenType::IDENTIFIER);
        EXPECT_EQ(t2.getValue(), "abc456");
    }
}

// 10. Test Operator Ambiguity
TEST(LexerTest, DeepOperatorAmbiguity)
{
    auto stream = std::make_unique<std::istringstream>("< <= > >= := :");
    WhileParser::Lexer lex(std::move(stream), true, true);

    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::LT);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::LTE);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::GT);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::GTE);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::ASSIGN);

    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::UNKNOWN);
}

// 11. Test Whitespace
TEST(LexerTest, StandardWhitespaces)
{
    auto stream = std::make_unique<std::istringstream>("x  :=  \n  5  ;");
    WhileParser::Lexer lex(std::move(stream), true, false);

    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::IDENTIFIER);

    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::ASSIGN);

    auto t = lex.nextToken();
    if (t.getType() == WhileParser::TokenType::END_OF_LINE)
    {
        t = lex.nextToken();
    }

    EXPECT_EQ(t.getType(), WhileParser::TokenType::NUMBER);
    EXPECT_EQ(lex.nextToken().getType(), WhileParser::TokenType::SEMICOLON);
}