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

        leftExpressionNode = parseMathExpression();

        if (m_current_token.getType() != TokenType::SEMICOLON)
            throw std::invalid_argument("The ASSIGNMENT construct is malformed: expected ;, got " + m_current_token.getTokenTypeString());
        advance();

        return std::move(std::make_unique<AssignmentNode>(identifier, std::move(leftExpressionNode)));
    }

    std::unique_ptr<IfNode> Parser::parseIfStatement()
    {
        // PredicateNode, StatementNode, StatementNode

        auto predicateNode = parseBooleanPredicate();

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

        auto statements = std::vector<std::unique_ptr<StatementNode>>();

        while (m_current_token.getType() == TokenType::IDENTIFIER)
        {
            auto assignmentStatementNode = parseAssignmentStatement();

            statements.push_back(std::move(assignmentStatementNode));
        }

        // single variable printed
        if (statements.empty())
            throw std::invalid_argument("Error in the syntax: " + m_current_token.getValue());

        return std::move(std::make_unique<SequenceNode>(std::move(statements)));
    }

    std::unique_ptr<WhileNode> Parser::parseWhileStatement()
    {
        // PredicateNode, StatementNode

        auto predicateNode = parseBooleanPredicate();

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

        if (m_current_token.getType() == TokenType::NUMBER || m_current_token.getType() == TokenType::IDENTIFIER)
        {
            auto expressionNode = std::make_unique<ExpressionNode>(m_current_token.getValue());
            advance();
            return std::move(expressionNode);
        }

        throw std::invalid_argument("The expression is not valid, current token: " + m_current_token.getTokenTypeString());
    }

    std::unique_ptr<ExpressionNode> Parser::parseMathExpression()
    {
        // ExpressionNode, MathOp, ExpressionNode

        auto leftExpressionNode = parseExpression();

        // TODO: handle parenthesis and precedence, for now I leave it as it is

        while (
            m_current_token.getType() == TokenType::PLUS ||
            m_current_token.getType() == TokenType::MINUS ||
            m_current_token.getType() == TokenType::WILDCARD ||
            m_current_token.getType() == TokenType::SLASH)
        {

            auto mathOperator = m_current_token.getValue();
            advance();

            auto rightExpressionNode = parseExpression();

            leftExpressionNode = std::make_unique<MathExpressionNode>(mathOperator, std::move(leftExpressionNode), std::move(rightExpressionNode));
        }

        return std::move(leftExpressionNode);
    }

    // TODO
    std::unique_ptr<PredicateNode> Parser::parsePredicate()
    {

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

        return nullptr;
    }

    std::unique_ptr<NotPredicateNode> Parser::parseNotPredicate()
    {
        return std::move(std::make_unique<NotPredicateNode>(std::move(parsePredicate())));
    }

    std::unique_ptr<PredicateNode> Parser::parseBooleanPredicate()
    {
        // PredicateNode, BooleanOp, PredicateNode

        std::unique_ptr<PredicateNode> leftPredicateNode = parsePredicate();
        if (!leftPredicateNode)
            leftPredicateNode = parseRelationalPredicate();

        while (m_current_token.getType() == TokenType::AND ||
               m_current_token.getType() == TokenType::OR)
        {

            std::string binaryOperator = m_current_token.getValue();
            advance();

            std::unique_ptr<PredicateNode> rightPredicateNode = parsePredicate();
            if (!rightPredicateNode)
                rightPredicateNode = parseRelationalPredicate();

            leftPredicateNode = std::make_unique<BooleanPredicateNode>(binaryOperator, std::move(leftPredicateNode),
                                                                       std::move(rightPredicateNode));
        }

        return std::move(leftPredicateNode);
    }
    std::unique_ptr<RelationalPredicateNode> Parser::parseRelationalPredicate()
    {
        // ExpressionNode, RelationalOp, ExpressionNode
        std::unique_ptr<RelationalPredicateNode> relationalPredicateNode = nullptr;
        std::unique_ptr<ExpressionNode> leftExpressionNode = parseMathExpression();

        // the variable count enforces the structure "expression op expression" preventing something like "expression"
        int count = 0;
        while (
            m_current_token.getType() == TokenType::GTE ||
            m_current_token.getType() == TokenType::GT ||
            m_current_token.getType() == TokenType::EQ ||
            m_current_token.getType() == TokenType::LT ||
            m_current_token.getType() == TokenType::LTE)
        {
            std::string relationalOp = m_current_token.getValue();
            advance();

            auto rightExpressionNode = parseMathExpression();

            relationalPredicateNode = std::make_unique<RelationalPredicateNode>(relationalOp, std::move(leftExpressionNode), std::move(rightExpressionNode));
            ++count;
        }
        if (count == 0)
            throw std::invalid_argument("The relational predicate is not valid, current token: " + m_current_token.getTokenTypeString());

        return std::move(relationalPredicateNode);
    }

    std::unique_ptr<ASTNode> Parser::parseTerminal()
    {
        std::unique_ptr<ASTNode> terminalNode = nullptr;
        if (m_current_token.getType() == TokenType::TRUE || m_current_token.getType() == TokenType::FALSE)
            terminalNode = std::make_unique<PredicateNode>(m_current_token.getValue());

        if (m_current_token.getType() == TokenType::IDENTIFIER || m_current_token.getType() == TokenType::NUMBER)
            terminalNode = std::make_unique<ExpressionNode>(m_current_token.getValue());

        if (!terminalNode)
            throw std::invalid_argument("The terminal is not valid, current token: " + m_current_token.getTokenTypeString());

        advance();
        return std::move(terminalNode);
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
