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

        if (m_current_token.getType() != TokenType::IDENTIFIER)
            throw std::invalid_argument("The ASSIGNMENT construct is malformed: expected IDENTIFIER, got " + m_current_token.getTokenTypeString());

        auto identifier = m_current_token.getValue();
        advance();

        if (m_current_token.getType() != TokenType::ASSIGN)
            throw std::invalid_argument("The ASSIGNMENT construct is malformed: expected :=, got " + m_current_token.getTokenTypeString());
        advance();

        if (m_current_token.getType() != TokenType::IDENTIFIER && m_current_token.getType() != TokenType::NUMBER)
            throw std::invalid_argument("The ASSIGNMENT construct is malformed: expected EXPRESSION, got " + m_current_token.getTokenTypeString());

        leftExpressionNode = parseExpression();

        if (m_current_token.getType() != TokenType::SEMICOLON)
            throw std::invalid_argument("The ASSIGNMENT construct is malformed: expected ;, got " + m_current_token.getTokenTypeString());
        advance();

        return std::move(std::make_unique<AssignmentNode>(identifier, std::move(leftExpressionNode)));
    }

    std::unique_ptr<IfNode> Parser::parseIfStatement()
    {

        auto predicateNode = parsePredicate();

        if (m_current_token.getType() != TokenType::THEN)
            throw std::invalid_argument("The IF construct is malformed: expected THEN, got " + m_current_token.getTokenTypeString());
        advance();

        auto thenStatementNode = parseStatement();

        if (m_current_token.getType() != TokenType::ELSE)
            throw std::invalid_argument("The IF construct is malformed: expected ELSE, got " + m_current_token.getTokenTypeString());
        advance();

        auto elseStatementNode = parseStatement();

        if (m_current_token.getType() != TokenType::ENDIF)
            throw std::invalid_argument("The IF construct is malformed: expected ENDIF, got " + m_current_token.getTokenTypeString());
        advance();

        return std::move(std::make_unique<IfNode>(std::move(predicateNode),
                                                  std::move(thenStatementNode), std::move(elseStatementNode)));
    }

    std::unique_ptr<WhileNode> Parser::parseWhileStatement()
    {

        auto predicateNode = parsePredicate();

        if (m_current_token.getType() != TokenType::DO)
            throw std::invalid_argument("The WHILE construct is malformed: expected DO, got " + m_current_token.getTokenTypeString());
        advance();

        auto statementNode = parseStatement();

        if (m_current_token.getType() != TokenType::ENDWHILE)
            throw std::invalid_argument("The WHILE construct is malformed: expected ENDWHILE, got " + m_current_token.getTokenTypeString());
        advance();

        return std::move(std::make_unique<WhileNode>(
            std::move(predicateNode), std::move(statementNode)));
    }

    std::unique_ptr<PredicateNode> Parser::parsePredicate()
    {
        if (m_current_token.getType() == TokenType::TRUE || m_current_token.getType() == TokenType::FALSE)
            return parseBooleanPredicate();

        if (m_current_token.getType() == TokenType::NOT)
            return parseNotPredicate();

        return parseRelationalPredicate();
    }

    std::unique_ptr<NotPredicateNode> Parser::parseNotPredicate()
    {
        advance();
        return std::move(std::make_unique<NotPredicateNode>(parsePredicate()));
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

    std::unique_ptr<MathExpressionNode> Parser::parseExpression()
    {

        auto mulDivExpr = std::make_unique<MathExpressionNode>(parseMulDivExpression());

        while (m_current_token.getType() == TokenType::PLUS || m_current_token.getType() == TokenType::MINUS)
        {
            auto op = m_current_token;
            advance();
            return std::move(std::make_unique<MathExpressionNode>(op.getValue(), std::move(mulDivExpr), parseExpression()));
        }

        return std::move(std::make_unique<MathExpressionNode>(std::move(mulDivExpr)));
    }

    std::unique_ptr<MathExpressionNode> Parser::parseMulDivExpression()
    {

        auto leftExpression = parsePrimaryExpression();

        if (m_current_token.getType() == TokenType::WILDCARD || m_current_token.getType() == TokenType::SLASH)
        {
            auto op = m_current_token.getValue();
            advance();
            return std::make_unique<MathExpressionNode>(op, std::move(leftExpression), parseMulDivExpression());
        }

        return std::move(std::make_unique<MathExpressionNode>(std::move(leftExpression)));
    }

    std::unique_ptr<ExpressionNode> Parser::parsePrimaryExpression()
    {

        if (m_current_token.getType() == TokenType::LPAREN)
        {
            advance();
            auto expressionNode = parseExpression();
            if (m_current_token.getType() != TokenType::RPAREN)
                throw std::invalid_argument("The PARENTHESIS construct is malformed: expected ), got " + m_current_token.getTokenTypeString());
            advance();
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
        auto leftExpression = std::make_unique<MathExpressionNode>(parseExpression());

        if (m_current_token.getType() == TokenType::GTE ||
            m_current_token.getType() == TokenType::GT ||
            m_current_token.getType() == TokenType::EQ ||
            m_current_token.getType() == TokenType::LT ||
            m_current_token.getType() == TokenType::LTE)
        {

            auto op = m_current_token.getValue();
            advance();

            return std::move(std::make_unique<RelationalPredicateNode>(op, std::move(leftExpression), parseExpression()));
        }

        return std::move(std::make_unique<RelationalPredicateNode>(std::move(leftExpression)));
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

}
