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
        Parser(const std::string &filename) : m_lexer(filename),
                                              m_current_token(Token(TokenType::END_OF_FILE, "EOF")),
                                              m_parenthesis_count(0)
        {
            advance(); // get the first token
        }

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
        void advance();

        Lexer m_lexer;
        Token m_current_token; // lookahead (1)
        int m_parenthesis_count;
    };

}

#endif