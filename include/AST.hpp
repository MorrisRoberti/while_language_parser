#ifndef HH_AST_INCLUDE_GUARD
#define HH_AST_INCLUDE_GUARD 1

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <string>

namespace WhileParser
{
    class ASTNode
    {
    public:
        virtual void printNode() const = 0;
        virtual ~ASTNode() {}
    };

    // Meta-node that represents the entrypoint
    class RootNode : public ASTNode
    {
    public:
        inline void printNode() const override
        {
            std::cout << "RootNode -> " << std::endl;
            std::for_each(m_children.begin(), m_children.end(), [](const std::unique_ptr<ASTNode> &child)
                          {
                std::cout << "\tChild: ";
                child->printNode();
                std::cout << std::endl; });
        }

        inline void addNode(std::unique_ptr<ASTNode> node)
        {
            m_children.push_back(std::move(node));
        }

    private:
        std::vector<std::unique_ptr<ASTNode>>
            m_children;
    };

    // Top Level Non Terminals
    class ExpressionNode : public ASTNode
    {
    public:
        inline void printNode() const override
        {
            std::cout << "ExpressionNode -> " << std::endl;
            std::cout << "\tValue: " << m_terminal_expression << std::endl;
        }

    private:
        std::string m_terminal_expression;
    };

    class StatementNode : public ASTNode
    {
    public:
        virtual void printNode() const = 0;
    };

    class PredicateNode : public ASTNode
    {
    public:
        virtual void printNode() const override
        {
            std::cout << "PredicateNode -> " << std::endl;
            std::cout << "\tValue: " << m_terminal_predicate << std::endl;
        }

    private:
        std::string m_terminal_predicate;
    };

    class MathOpNode : public ASTNode
    {
    public:
        virtual void printNode() const = 0;
    };

    class BooleanOpNode : public ASTNode
    {
    public:
        virtual void printNode() const = 0;
    };

    class RelationalOpNode : public ASTNode
    {
    public:
        virtual void printNode() const = 0;
    };

    // Statement productions
    class AssignmentNode : public StatementNode
    {
    public:
        AssignmentNode(const std::string &var_name, std::unique_ptr<ExpressionNode> expr)
            : m_variable_name(var_name), m_expression(std::move(expr)) {}

        inline void printNode() const override
        {
            std::cout << "AssignmentNode -> " << std::endl;
            std::cout << "\tIdentifier: " << m_variable_name << std::endl;

            std::cout << "\tExpression: ";
            m_expression->printNode();
            std::cout << std::endl;
        }

    private:
        std::string m_variable_name;
        std::unique_ptr<ExpressionNode> m_expression;
    };

    class IfNode : public StatementNode
    {
    public:
        IfNode(std::unique_ptr<PredicateNode> condition, std::unique_ptr<StatementNode> then_statement, std::unique_ptr<StatementNode> else_statement)
            : m_condition(std::move(condition)), m_then_branch(std::move(then_statement)), m_else_branch(std::move(else_statement)) {}

        inline void printNode() const override
        {
            std::cout << "IfNode -> " << std::endl;
            std::cout << "\tCondition: ";
            m_condition->printNode();
            std::cout << std::endl;

            std::cout << "\tThenBranch: ";
            m_then_branch->printNode();
            std::cout << std::endl;

            std::cout << "\tElseBranch: ";
            m_else_branch->printNode();
            std::cout << std::endl;
        }

    private:
        std::unique_ptr<PredicateNode> m_condition;
        std::unique_ptr<StatementNode> m_then_branch;
        std::unique_ptr<StatementNode> m_else_branch;
    };

    class SkipNode : public StatementNode
    {
    public:
        SkipNode() = default;

        inline void printNode() const override
        {
            std::cout << "SkipNode" << std::endl;
        }
    };

    class SequenceNode : public StatementNode
    {
    public:
        SequenceNode(std::vector<std::unique_ptr<StatementNode>> statement_list) : m_statement_list(std::move(statement_list)) {}

        inline void printNode() const override
        {
            std::cout << "SequenceNode -> " << std::endl;
            std::for_each(m_statement_list.begin(), m_statement_list.end(), [](const std::unique_ptr<StatementNode> &statement)
                          {
                std::cout << "\tStatement: ";
                statement->printNode();
                std::cout << std::endl; });
            std::cout << std::endl;
        }

    private:
        std::vector<std::unique_ptr<StatementNode>> m_statement_list;
    };

    class WhileNode : public StatementNode
    {
    public:
        WhileNode(std::unique_ptr<PredicateNode> condition, std::unique_ptr<StatementNode> statement)
            : m_condition(std::move(condition)), m_statement(std::move(statement)) {}

        inline void printNode() const override
        {
            std::cout << "WhileNode -> " << std::endl;

            std::cout << "\tCondition: ";
            m_condition->printNode();
            std::cout << std::endl;

            std::cout << "\tStatement: ";
            m_statement->printNode();
            std::cout << std::endl;
        }

    private:
        std::unique_ptr<PredicateNode> m_condition;
        std::unique_ptr<StatementNode> m_statement;
    };

    // Expression productions
    class MathExpressionNode : public ExpressionNode
    {
    public:
        MathExpressionNode(char math_operation, std::unique_ptr<ExpressionNode> left_expression,
                           std::unique_ptr<ExpressionNode> right_expression) : m_math_operation(math_operation), m_left_expression(std::move(left_expression)),
                                                                               m_right_expression(std::move(right_expression)) {}

        inline void printNode() const override
        {
            std::cout << "MathExpressionNode -> " << std::endl;
            std::cout << "\tMathOp: " << m_math_operation << std::endl;

            std::cout << "\tLeftSideExpression: ";
            m_left_expression->printNode();
            std::cout << std::endl;

            std::cout << "\tRightSideExpression: ";
            m_right_expression->printNode();
            std::cout << std::endl;
        }

    private:
        char m_math_operation;
        std::unique_ptr<ExpressionNode> m_left_expression;
        std::unique_ptr<ExpressionNode> m_right_expression;
    };

    // Predicate productions
    class NotPredicateNode : public PredicateNode
    {
    public:
        NotPredicateNode(std::unique_ptr<PredicateNode> predicate) : m_predicate(std::move(predicate)) {}

        inline void printNode() const override
        {
            std::cout << "NotPredicateNode -> " << std::endl;

            std::cout << "\tPredicate: ";
            m_predicate->printNode();
            std::cout << std::endl;
        }

    private:
        std::unique_ptr<PredicateNode> m_predicate;
    };

    class BooleanPredicateNode : public PredicateNode
    {
    public:
        BooleanPredicateNode(const std::string &boolean_operation, std::unique_ptr<PredicateNode> left_predicate,
                             std::unique_ptr<PredicateNode> right_predicate) : m_boolean_operation(boolean_operation), m_left_predicate(std::move(left_predicate)),
                                                                               m_right_predicate(std::move(m_right_predicate)) {}

        inline void printNode() const override
        {
            std::cout << "BooleanPredicateNode -> " << std::endl;
            std::cout << "\tBooleanOp: " << m_boolean_operation << std::endl;

            std::cout << "\tLeftSidePredicate: ";
            m_left_predicate->printNode();
            std::cout << std::endl;

            std::cout << "\tRightSidePredicate: ";
            m_right_predicate->printNode();
            std::cout << std::endl;
        }

    private:
        std::string m_boolean_operation;
        std::unique_ptr<PredicateNode> m_left_predicate;
        std::unique_ptr<PredicateNode> m_right_predicate;
    };

    class RelationalPredicateNode : public PredicateNode
    {
    public:
        RelationalPredicateNode(const std::string &relational_operation, std::unique_ptr<StatementNode> left_statement,
                                std::unique_ptr<StatementNode> right_statement) : m_relational_operation(relational_operation), m_left_statement(std::move(left_statement)),
                                                                                  m_right_statement(std::move(right_statement)) {}

        inline void printNode() const override
        {
            std::cout << "RelationalPredicateNode -> " << std::endl;
            std::cout << "\tRelationalnOp: " << m_relational_operation << std::endl;

            std::cout << "\tLeftSideStatement: ";
            m_left_statement->printNode();
            std::cout << std::endl;

            std::cout << "\tRightSideStatement: ";
            m_right_statement->printNode();
            std::cout << std::endl;
        }

    private:
        std::string m_relational_operation;
        std::unique_ptr<StatementNode> m_left_statement;
        std::unique_ptr<StatementNode> m_right_statement;
    };

}
#endif