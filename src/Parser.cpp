#include "../include/Parser.hpp"

namespace WhileParser
{

    std::unique_ptr<RootNode> Parser::parse()
    {

        auto root = std::unique_ptr<RootNode>();

        while (m_lexer.isTokenAvailable())
        {

            // il token lo passo?? o lo prendo dal lexer in ogni funzione?
            // se faccio cosi' qua prendo solo quelli dei top statement
            auto token = m_lexer.nextToken();

            if (token.getType() == TokenType::UNKNOWN)
                throw std::invalid_argument("The following token is unknown: " + token.getValue());

            // at the level 1 of the AST it's only possible to have Statements
            root->addNode(parseStatement());
        }
    }

    std::unique_ptr<StatementNode> Parser::parseStatement()
    {

        // if (token.getType() == TokenType::WHILE)
        //     root->addNode(parseWhileStatement());
        // if (token.getType() == TokenType::IF)
        //     root->addNode(parseIfStatement());
        // if (token.getType() == TokenType::SKIP)
        //     root->addNode(parseSkipStatement());
    }
}