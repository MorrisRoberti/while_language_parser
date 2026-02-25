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
            return parseSkipStatement();
        }
        if (m_current_token.getType() == TokenType::IDENTIFIER)
        {
            return parseSequenceStatement();
        }
        // sequence

        // placeholder
        throw std::invalid_argument("The syntax is not correct");
    }

    std::unique_ptr<AssignmentNode> Parser::parseAssignmentStatement()
    {
        auto identifier = m_current_token.getValue();
        advance();

        if (m_current_token.getType() == TokenType::ASSIGN)
            advance();

        if (m_current_token.getType() == TokenType::IDENTIFIER || m_current_token.getType() == TokenType::NUMBER)
            advance();

        if (m_current_token.getType() == TokenType::SEMICOLON)
            advance();

        return std::move(std::make_unique<AssignmentNode>(identifier, std::move(std::make_unique<ExpressionNode>(m_current_token.getValue()))));
    }

    std::unique_ptr<IfNode> Parser::parseIfStatement()
    {
        // PredicateNode, StatementNode, StatementNode

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

    std::unique_ptr<SkipNode> Parser::parseSkipStatement()
    {

        return std::move(std::make_unique<SkipNode>());
    }

    // TODO
    std::unique_ptr<SequenceNode> Parser::parseSequenceStatement()
    {

        auto firstStatement = parseAssignmentStatement();

        auto secondStatement = parseAssignmentStatement();
        advance();

        return std::move(std::make_unique<SequenceNode>(
            std::vector<std::unique_ptr<StatementNode>>{std::move(firstStatement), std::move(secondStatement)}));
    }

    std::unique_ptr<WhileNode> Parser::parseWhileStatement()
    {
        // PredicateNode, StatementNode

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

    // TODO
    std::unique_ptr<ExpressionNode> Parser::parseExpression()
    {

        return std::unique_ptr<ExpressionNode>();
    }

    std::unique_ptr<MathExpressionNode> Parser::parseMathExpression()
    {
        // ExpressionNode, MathOp, ExpressionNode

        auto leftExpressionNode = parseExpression();

        // TODO: handle parenthesis and precedence, for now I leave it as it is

        if (
            m_current_token.getType() != TokenType::PLUS &&
            m_current_token.getType() != TokenType::MINUS &&
            m_current_token.getType() != TokenType::WILDCARD &&
            m_current_token.getType() != TokenType::SLASH)
            throw std::invalid_argument("The MATH EXPRESSION construct is malformed: expected PLUS, MINUS, WILDCARD or SLASH, got " + m_current_token.getTokenTypeString());

        auto rightExpressionNode = parseExpression();

        return std::move(std::make_unique<MathExpressionNode>(m_current_token.getValue().data()[0], std::move(leftExpressionNode), std::move(rightExpressionNode)));
    }

    // TODO
    std::unique_ptr<PredicateNode> Parser::parsePredicate()
    {
        // parseRelationalPredicate
        // parseBooleanPredicate true o false o not
        // base case
        if (m_current_token.getType() == TokenType::TRUE || m_current_token.getType() == TokenType::FALSE)
        {
            auto predicateNode = std::make_unique<PredicateNode>(m_current_token.getValue());
            advance();
            return std::move(predicateNode);
        }
        if (m_current_token.getType() == TokenType::NOT)
        {
            advance();
            return parseNotPredicate();
        }

        throw std::invalid_argument("The predicate is not valid, current token: " + m_current_token.getTokenTypeString());
    }

    std::unique_ptr<NotPredicateNode> Parser::parseNotPredicate()
    {
        return std::move(std::make_unique<NotPredicateNode>(std::move(parsePredicate())));
    }

    std::unique_ptr<BooleanPredicateNode> Parser::parseBooleanPredicate()
    {
        // PredicateNode, BooleanOp, PredicateNode

        auto leftPredicateNode = std::make_unique<PredicateNode>(m_current_token.getValue());
        if (m_current_token.getType() != TokenType::TRUE && m_current_token.getType() != TokenType::FALSE)
            leftPredicateNode = parsePredicate();
        advance();

        if (
            m_current_token.getType() != TokenType::AND &&
            m_current_token.getType() != TokenType::OR)
            return nullptr;

        std::string binaryOperator = m_current_token.getValue();
        advance();

        auto rightPredicateNode = std::make_unique<PredicateNode>(m_current_token.getValue());
        if (m_current_token.getType() != TokenType::TRUE && m_current_token.getType() != TokenType::FALSE)
            rightPredicateNode = parsePredicate();

        return std::move(std::make_unique<BooleanPredicateNode>(binaryOperator, std::move(leftPredicateNode),
                                                                std::move(rightPredicateNode)));
    }
    std::unique_ptr<RelationalPredicateNode> Parser::parseRelationalPredicate()
    {
        // StatementNode, RelationalOp, StatementNode

        auto leftStatementNode = parseStatement();

        if (
            m_current_token.getType() != TokenType::GTE &&
            m_current_token.getType() != TokenType::GT &&
            m_current_token.getType() != TokenType::EQ &&
            m_current_token.getType() != TokenType::LT &&
            m_current_token.getType() != TokenType::LTE)
            throw std::invalid_argument("The RELATIONAL PREDICATE construct is malformed: expected <, <=, =, >= or >, got " + m_current_token.getTokenTypeString());

        auto rightStatementNode = parseStatement();

        return std::move(std::make_unique<RelationalPredicateNode>(m_current_token.getValue(), std::move(leftStatementNode),
                                                                   std::move(rightStatementNode)));
    }

    void Parser::advance()
    {

        auto token = m_lexer.nextToken();

        // skip whitespaces and endlines
        while (token.getType() == TokenType::WHITESPACE || token.getType() == TokenType::END_OF_LINE)
            token = m_lexer.nextToken();

        if (token.getType() == TokenType::UNKNOWN)
            throw std::invalid_argument("The following token is unknown: " + token.getValue());

        m_current_token = token;
    }
}
