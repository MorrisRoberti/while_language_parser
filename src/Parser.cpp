#include "../include/Parser.hpp"
#include "Parser.hpp"

namespace WhileParser
{

    std::unique_ptr<RootNode> Parser::parse()
    {

        auto root = std::make_unique<RootNode>();
        try
        {
            while (m_lexer.isTokenAvailable())
            {
                // at the level 1 of the AST it's only possible to have Statements
                auto statementNode = parseStatement();
                root->addNode(std::move(statementNode));
            }
        }
        catch (std::invalid_argument exception)
        {
            std::cerr << exception.what() << std::endl;
            throw exception; // to catch in the entrypoint
        }

        return std::move(root);
    }

    std::unique_ptr<StatementNode> Parser::parseStatement()
    {

        if (m_current_token.getType() == TokenType::WHILE)
        {
            advance();
            return parseWhileStatement();
        }
        if (m_current_token.getType() == TokenType::IF)
        {
            advance();
            return parseIfStatement();
        }
        if (m_current_token.getType() == TokenType::SKIP)
        {
            advance();
            return std::make_unique<SkipNode>();
        }
        if (m_current_token.getType() == TokenType::IDENTIFIER)
        {
            return parseAssignmentStatement();
        }

        // placeholder
        throw std::invalid_argument("The syntax is not correct");
    }

    std::unique_ptr<AssignmentNode> Parser::parseAssignmentStatement()
    {
        std::unique_ptr<ExpressionNode> leftExpressionNode = nullptr;

        auto identifier = consume(TokenType::IDENTIFIER, "Expected IDENTIFIER").getValue();

        consume(TokenType::ASSIGN, "Expected ASSIGN");

        if (m_current_token.getType() != TokenType::IDENTIFIER && m_current_token.getType() != TokenType::NUMBER)
            throw std::invalid_argument("The ASSIGNMENT construct is malformed: expected EXPRESSION, got " + m_current_token.getTokenTypeString());

        leftExpressionNode = parseExpression();

        consume(TokenType::SEMICOLON, "Expected SEMICOLON");

        return std::move(std::make_unique<AssignmentNode>(identifier, std::move(leftExpressionNode)));
    }

    std::unique_ptr<IfNode> Parser::parseIfStatement()
    {

        auto predicateNode = parsePredicate();

        consume(TokenType::THEN, "Expected THEN");

        auto thenStatementNode = parseStatement();

        consume(TokenType::ELSE, "Expected ELSE");

        auto elseStatementNode = parseStatement();

        consume(TokenType::ENDIF, "Expected ENDIF");

        return std::move(std::make_unique<IfNode>(std::move(predicateNode),
                                                  std::move(thenStatementNode), std::move(elseStatementNode)));
    }

    std::unique_ptr<WhileNode> Parser::parseWhileStatement()
    {

        auto predicateNode = parsePredicate();

        consume(TokenType::DO, "Expected DO");

        auto statementNode = parseStatement();

        consume(TokenType::ENDWHILE, "Expected ENDWHILE");

        return std::move(std::make_unique<WhileNode>(
            std::move(predicateNode), std::move(statementNode)));
    }

    std::unique_ptr<PredicateNode> Parser::parseBooleanPredicate()
    {

        auto leftPredicate = std::make_unique<PredicateNode>(m_current_token.getValue());
        advance();

        if ((m_current_token.getType() == TokenType::AND ||
             m_current_token.getType() == TokenType::OR))
        {

            auto op = m_current_token.getValue();
            advance();

            return std::move(std::make_unique<BooleanPredicateNode>(op, std::move(leftPredicate), parsePredicate()));
        }

        // epsilon case
        return std::move(leftPredicate);
    }

    std::unique_ptr<ExpressionNode> Parser::parseExpression()
    {

        auto leftMulDivExpression = parseMulDivExpression();

        while (m_current_token.getType() == TokenType::PLUS || m_current_token.getType() == TokenType::MINUS)
        {
            auto op = m_current_token;
            advance();
            auto rightMulDivExpression = parseMulDivExpression();
            leftMulDivExpression = std::make_unique<MathExpressionNode>(op.getValue(), std::move(leftMulDivExpression), std::move(rightMulDivExpression));
        }

        return std::move(leftMulDivExpression);
    }

    std::unique_ptr<ExpressionNode> Parser::parseMulDivExpression()
    {

        auto leftExpression = parsePrimaryExpression();

        while (m_current_token.getType() == TokenType::WILDCARD || m_current_token.getType() == TokenType::SLASH)
        {
            auto op = m_current_token.getValue();
            advance();
            auto rightExpression = parsePrimaryExpression();
            leftExpression = std::make_unique<MathExpressionNode>(op, std::move(leftExpression), std::move(rightExpression));
        }

        return std::move(leftExpression);
    }

    std::unique_ptr<ExpressionNode> Parser::parsePrimaryExpression()
    {

        if (m_current_token.getType() == TokenType::LPAREN)
        {
            advance();
            auto expressionNode = parseExpression();
            consume(TokenType::RPAREN, "Expected RPAREN");
            return std::move(expressionNode);
        }

        if (m_current_token.getType() == TokenType::IDENTIFIER || m_current_token.getType() == TokenType::NUMBER)
        {

            auto token = m_current_token;
            advance();
            return std::move(std::make_unique<ExpressionNode>(token.getValue()));
        }

        throw std::invalid_argument("The EXPRESSION is malformed: expected IDENTIFIER/NUMBER, got " + m_current_token.getTokenTypeString());
    }

    std::unique_ptr<RelationalPredicateNode> Parser::parseRelationalPredicate()
    {
        auto leftExpression = parseExpression();

        if (m_current_token.getType() == TokenType::GTE ||
            m_current_token.getType() == TokenType::GT ||
            m_current_token.getType() == TokenType::EQ ||
            m_current_token.getType() == TokenType::LT ||
            m_current_token.getType() == TokenType::LTE)
        {
            auto op = m_current_token.getValue();
            advance();
            auto rightExpression = parseExpression();
            return std::make_unique<RelationalPredicateNode>(op, std::move(leftExpression), std::move(rightExpression));
        }

        throw std::invalid_argument("Expected relational operator after expression, got: " + m_current_token.getValue());
    }

    std::unique_ptr<PredicateNode> Parser::parsePredicate()
    {
        auto leftNode = parseAndPredicate();

        while (m_current_token.getType() == TokenType::OR)
        {
            auto op = m_current_token.getValue();
            advance();
            auto rightNode = parseAndPredicate();
            leftNode = std::make_unique<BooleanPredicateNode>(op, std::move(leftNode), std::move(rightNode));
        }
        return leftNode;
    }

    std::unique_ptr<PredicateNode> Parser::parseAndPredicate()
    {
        auto leftNode = parseUnaryPredicate();

        while (m_current_token.getType() == TokenType::AND)
        {
            auto op = m_current_token.getValue();
            advance();
            auto rightNode = parseUnaryPredicate();
            leftNode = std::make_unique<BooleanPredicateNode>(op, std::move(leftNode), std::move(rightNode));
        }
        return leftNode;
    }

    std::unique_ptr<PredicateNode> Parser::parseUnaryPredicate()
    {
        if (m_current_token.getType() == TokenType::NOT)
        {
            advance();
            return std::make_unique<NotPredicateNode>(parseUnaryPredicate());
        }
        return parsePrimaryPredicate();
    }

    std::unique_ptr<PredicateNode> Parser::parsePrimaryPredicate()
    {
        if (m_current_token.getType() == TokenType::TRUE || m_current_token.getType() == TokenType::FALSE)
        {
            auto val = m_current_token.getValue();
            advance();
            return std::make_unique<PredicateNode>(val);
        }

        if (m_current_token.getType() == TokenType::LPAREN)
        {
            advance();
            auto node = parsePredicate();
            consume(TokenType::RPAREN, "Expected ')' after boolean expression");
            return node;
        }

        return parseRelationalPredicate();
    }

    void Parser::advance()
    {

        auto token = m_lexer.nextToken();

        // skip endlines
        while (token.getType() == TokenType::END_OF_LINE)
            token = m_lexer.nextToken();

        if (token.getType() == TokenType::UNKNOWN)
            throw std::invalid_argument("The following token is unknown: " + token.getValue());

        m_current_token = token;
    }

    Token Parser::consume(TokenType expected, const std::string &errorMessage)
    {
        if (m_current_token.getType() != expected)
            throw std::invalid_argument(errorMessage + " Got: " + m_current_token.getTokenTypeString());

        auto token = m_current_token;
        advance();
        return token;
    }
}
