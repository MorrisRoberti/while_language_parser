#ifndef HH_PARSER_INCLUDE_GUARD
#define HH_PARSER_INCLUDE_GUARD 1

#include "./Token.hpp"
#include "./AST.hpp"

namespace WhileParser
{

    class Parser
    {
    public:
        Parser() {}

        std::unique_ptr<ASTNode> parse();

    private:
        ASTNode *parseNumber();

        ASTNode *parseBinaryExprExpr();

        bool isTokenAvailable();

        void eat();

        Token getNextToken();
    };

}

#endif