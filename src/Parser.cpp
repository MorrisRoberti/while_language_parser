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
        auto token = getToken();

        if (token.getType() == TokenType::WHILE)
            return parseWhileStatement();
        if (token.getType() == TokenType::IF)
            return parseIfStatement();
        if (token.getType() == TokenType::SKIP)
            return parseSkipStatement();
        // assignment
        // sequence

        // placeholder
        throw std::invalid_argument("The syntax is not correct");
    }

    std::unique_ptr<AssignmentNode> Parser::parseAssignmentStatement()
    {
        return std::unique_ptr<AssignmentNode>();
    }

    std::unique_ptr<IfNode> Parser::parseIfStatement()
    {
        // PredicateNode, StatementNode, StatementNode

        auto predicateNode = parsePredicate();

        if (auto token = getToken(); token.getType() != TokenType::THEN)
            throw std::invalid_argument("The IF construct is malformed: expected THEN, got " + token.getTokenTypeString());

        auto thenStatementNode = parseStatement();

        if (auto token = getToken(); token.getType() != TokenType::THEN)
            throw std::invalid_argument("The IF construct is malformed: expected ELSE, got " + token.getTokenTypeString());

        auto elseStatementNode = parseStatement();

        if (auto token = getToken(); token.getType() != TokenType::THEN)
            throw std::invalid_argument("The IF construct is malformed: expected ENDIF, got " + token.getTokenTypeString());

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
        return std::unique_ptr<SequenceNode>();
    }

    std::unique_ptr<WhileNode> Parser::parseWhileStatement()
    {
        // PredicateNode, StatementNode

        auto predicateNode = parsePredicate();

        if (auto token = getToken(); token.getType() != TokenType::DO)
            throw std::invalid_argument("The WHILE construct is malformed: expected DO, got " + token.getTokenTypeString());

        auto statementNode = parseStatement();

        if (auto token = getToken(); token.getType() != TokenType::ENDWHILE)
            throw std::invalid_argument("The WHILE construct is malformed: expected ENDWHILE, got " + token.getTokenTypeString());

        return std::move(std::make_unique<WhileNode>(
            std::move(predicateNode), std::move(statementNode)));
    }

    // TODO
    std::unique_ptr<ExpressionNode> Parser::parseExpression()
    {

        auto token = getToken();

        return std::unique_ptr<ExpressionNode>();
    }

    std::unique_ptr<MathExpressionNode> Parser::parseMathExpression()
    {
        // ExpressionNode, MathOp, ExpressionNode

        auto leftExpressionNode = parseExpression();

        // TODO: handle parenthesis and precedence, for now I leave it as it is

        auto token = getToken();
        if (
            token.getType() != TokenType::PLUS &&
            token.getType() != TokenType::MINUS &&
            token.getType() != TokenType::WILDCARD &&
            token.getType() != TokenType::SLASH)
            throw std::invalid_argument("The MATH EXPRESSION construct is malformed: expected PLUS, MINUS, WILDCARD or SLASH, got " + token.getTokenTypeString());

        auto rightExpressionNode = parseExpression();

        return std::move(std::make_unique<MathExpressionNode>(token.getValue().data()[0], std::move(leftExpressionNode), std::move(rightExpressionNode)));
    }

    // TODO
    std::unique_ptr<PredicateNode> Parser::parsePredicate()
    {
        auto token = getToken();

        // parseBooleanPredicate
        // parseRelationalPredicate

        // base case
        if (token.getType() == TokenType::TRUE || token.getType() == TokenType::FALSE)
            return std::move(std::make_unique<PredicateNode>(token.getValue()));
        if (token.getType() == TokenType::NOT)
            return parseNotPredicate();

        throw std::invalid_argument("The predicate is not valid");
    }

    std::unique_ptr<NotPredicateNode> Parser::parseNotPredicate()
    {
        return std::move(std::make_unique<NotPredicateNode>(std::move(parsePredicate())));
    }

    std::unique_ptr<BooleanPredicateNode> Parser::parseBooleanPredicate()
    {
        // PredicateNode, BooleanOp, PredicateNode

        auto leftPredicateNode = parsePredicate();

        auto token = getToken();
        if (
            token.getType() != TokenType::AND &&
            token.getType() != TokenType::OR)
            throw std::invalid_argument("The BOOLEAN PREDICATE construct is malformed: expected OR or AND, got " + token.getTokenTypeString());

        auto rightPredicateNode = parsePredicate();

        return std::move(std::make_unique<BooleanPredicateNode>(token.getValue(), std::move(leftPredicateNode),
                                                                std::move(rightPredicateNode)));
    }
    std::unique_ptr<RelationalPredicateNode> Parser::parseRelationalPredicate()
    {
        // StatementNode, RelationalOp, StatementNode

        auto leftStatementNode = parseStatement();

        auto token = getToken();
        if (
            token.getType() != TokenType::GTE &&
            token.getType() != TokenType::GT &&
            token.getType() != TokenType::EQ &&
            token.getType() != TokenType::LT &&
            token.getType() != TokenType::LTE)
            throw std::invalid_argument("The RELATIONAL PREDICATE construct is malformed: expected <, <=, =, >= or >, got " + token.getTokenTypeString());

        auto rightStatementNode = parseStatement();

        return std::move(std::make_unique<RelationalPredicateNode>(token.getValue(), std::move(leftStatementNode),
                                                                   std::move(rightStatementNode)));
    }

    Token Parser::getToken()
    {

        auto token = m_lexer.nextToken();

        // skip whitespaces and endlines
        while (token.getType() == TokenType::WHITESPACE || token.getType() == TokenType::END_OF_LINE)
            token = m_lexer.nextToken();

        if (token.getType() == TokenType::UNKNOWN)
            throw std::invalid_argument("The following token is unknown: " + token.getValue());

        return token;
    }
}
