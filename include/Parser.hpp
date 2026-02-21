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
        // Statement parsing
        std::unique_ptr<StatementNode> parseStatement();

        std::unique_ptr<AssignmentNode> parseAssignmentStatement();
        std::unique_ptr<IfNode> parseIfStatement();
        std::unique_ptr<SkipNode> parseSkipStatement();
        std::unique_ptr<SequenceNode> parseSequenceStatement();
        std::unique_ptr<WhileNode> parseWhileStatement();

        // Expression parsing
        std::unique_ptr<ExpressionNode> parseExpression();

        std::unique_ptr<MathExpressionNode> parseMathExpression();

        // Predicate parsing
        std::unique_ptr<PredicateNode> parsePredicate();

        std::unique_ptr<NotPredicateNode> parseNotPredicate();
        std::unique_ptr<BooleanPredicateNode> parseBooleanPredicate();
        std::unique_ptr<RelationalPredicateNode> parseRelationalPredicate();

        // i encapsulate the check of token validity in here
        Token getToken();

        Lexer m_lexer;
    };

}

#endif