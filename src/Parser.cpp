#include "../include/Parser.hpp"

namespace WhileParser
{

    std::unique_ptr<RootNode> Parser::parse()
    {

        auto root = std::unique_ptr<RootNode>();

        try
        {

            while (m_lexer.isTokenAvailable())
            {
                // at the level 1 of the AST it's only possible to have Statements
                root->addNode(std::move(parseStatement()));
            }
        }
        catch (std::exception exception)
        {
            std::cerr << exception.what() << std::endl;
            throw exception; // to catch in the entrypoint
        }
    }

    std::unique_ptr<StatementNode> Parser::parseStatement()
    {

        auto token = m_lexer.nextToken();

        if (token.getType() == TokenType::UNKNOWN)
            throw std::invalid_argument("The following token is unknown: " + token.getValue());

        if (token.getType() == TokenType::WHILE)
            return parseWhileStatement();
        if (token.getType() == TokenType::IF)
            return parseIfStatement();
        if (token.getType() == TokenType::SKIP)
            return parseSkipStatement();
    }

    std::unique_ptr<AssignmentNode> Parser::parseAssignmentStatement()
    {
        return std::unique_ptr<AssignmentNode>();
    }

    std::unique_ptr<IfNode> Parser::parseIfStatement()
    {
        return std::unique_ptr<IfNode>();
    }

    std::unique_ptr<SkipNode> Parser::parseSkipStatement()
    {
        return std::unique_ptr<SkipNode>();
    }

    std::unique_ptr<SequenceNode> Parser::parseSequenceStatement()
    {
        return std::unique_ptr<SequenceNode>();
    }

    std::unique_ptr<WhileNode> Parser::parseWhileStatement()
    {
        return std::unique_ptr<WhileNode>();
    }
}
