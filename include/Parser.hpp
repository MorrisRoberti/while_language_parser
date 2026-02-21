#ifndef HH_PARSER_INCLUDE_GUARD
#define HH_PARSER_INCLUDE_GUARD 1

#include "./Token.hpp"
#include "./AST.hpp"
#include "./Lexer.hpp"
#include "./TokenType.hpp"

namespace WhileParser
{

    class Parser
    {
    public:
        Parser();

        std::unique_ptr<RootNode> parse();

    private:
        // statement parsing
        std::unique_ptr<StatementNode> parseStatement();

        std::unique_ptr<AssignmentNode> parseAssignmentStatement();
        std::unique_ptr<IfNode> parseIfStatement();
        std::unique_ptr<SkipNode> parseSkipStatement();
        std::unique_ptr<SequenceNode> parseSequenceStatement();
        std::unique_ptr<WhileNode> parseWhileStatement();

        // expression parsing

        Lexer m_lexer;
    };

}

#endif