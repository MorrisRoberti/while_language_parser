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
            std::for_each(m_children.begin(), m_children.end(), [](ASTNode &child)
                          {
                std::cout << "\tChild: ";
                child.printNode();
                std::cout << std::endl; });
        }

        inline void addNode(std::unique_ptr<ASTNode> node)
        {
            m_children.push_back(node);
        }

    private:
        std::vector<std::unique_ptr<ASTNode>>
            m_children;
    };

    // Top Level Non Terminals
    class ExpressionNode : public ASTNode
    {
    public:
        virtual void printNode() const = 0;
    };

    class StatementNode : public ASTNode
    {
    public:
        virtual void printNode() const = 0;
    };

    class PredicateNode : public ASTNode
    {
    public:
        virtual void printNode() const = 0;
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

    // Productions
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
            std::cout << "\Condition: ";
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
    };

    class SequenceNode : public StatementNode
    {
    public:
        SequenceNode(std::vector<std::unique_ptr<StatementNode>> statement_list) : m_statement_list(std::move(statement_list)) {}

        inline void printNode() const override
        {
            std::cout << "SequenceNode -> " << std::endl;
            std::for_each(m_statement_list.begin(), m_statement_list.end(), [](StatementNode &statement)
                          {
                std::cout << "\tStatement: ";
                statement.printNode();
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

            std::cout << "\Condition: ";
            m_condition->printNode();
            std::cout << std::endl;

            std::cout << "\Statement: ";
            m_statement->printNode();
            std::cout << std::endl;
        }

    private:
        std::unique_ptr<PredicateNode> m_condition;
        std::unique_ptr<StatementNode> m_statement;
    };
}
#endif