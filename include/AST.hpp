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
        virtual void print_node() const = 0;
        virtual ~ASTNode() {}
    };

    // Top Level Non Terminals
    class Expression : public ASTNode
    {
    public:
        virtual void print_node() const = 0;
    };

    class Statement : public ASTNode
    {
    public:
        virtual void print_node() const = 0;
    };

    class Predicate : public ASTNode
    {
    public:
        virtual void print_node() const = 0;
    };

    class MathOp : public ASTNode
    {
    public:
        virtual void print_node() const = 0;
    };

    class BooleanOp : public ASTNode
    {
    public:
        virtual void print_node() const = 0;
    };

    class RelationalOp : public ASTNode
    {
    public:
        virtual void print_node() const = 0;
    };

    // Productions
    class AssignmentNode : public Statement
    {
    public:
        AssignmentNode(const std::string &var_name, std::unique_ptr<Expression> expr)
            : m_variable_name(var_name), m_expression(std::move(expr)) {}

        inline void print_node() const override
        {
            std::cout << "AssignmentNode -> " << std::endl;
            std::cout << "\tIdentifier: " << m_variable_name << std::endl;

            std::cout << "\tExpression: ";
            m_expression->print_node();
            std::cout << std::endl;
        }

    private:
        std::string m_variable_name;
        std::unique_ptr<Expression> m_expression;
    };

    class IfNode : public Statement
    {
    public:
        IfNode(std::unique_ptr<Predicate> condition, std::unique_ptr<Statement> then_statement, std::unique_ptr<Statement> else_statement)
            : m_condition(std::move(condition)), m_then_branch(std::move(then_statement)), m_else_branch(std::move(else_statement)) {}

        inline void print_node() const override
        {
            std::cout << "IfNode -> " << std::endl;
            std::cout << "\Condition: ";
            m_condition->print_node();
            std::cout << std::endl;

            std::cout << "\tThenBranch: ";
            m_then_branch->print_node();
            std::cout << std::endl;

            std::cout << "\tElseBranch: ";
            m_else_branch->print_node();
            std::cout << std::endl;
        }

    private:
        std::unique_ptr<Predicate> m_condition;
        std::unique_ptr<Statement> m_then_branch;
        std::unique_ptr<Statement> m_else_branch;
    };

    class SkipNode : public Statement
    {
    };

    class SequenceNode : public Statement
    {
    public:
        SequenceNode(std::vector<std::unique_ptr<Statement>> statement_list) : m_statement_list(std::move(statement_list)) {}

        inline void print_node() const override
        {
            std::cout << "SequenceNode -> " << std::endl;
            std::for_each(m_statement_list.begin(), m_statement_list.end(), [](Statement &statement)
                          {
                std::cout << "\tStatement: ";
                statement.print_node();
                std::cout << std::endl; });
            std::cout << std::endl;
        }

    private:
        std::vector<std::unique_ptr<Statement>> m_statement_list;
    };

    class WhileNode : public Statement
    {
    public:
        WhileNode(std::unique_ptr<Predicate> condition, std::unique_ptr<Statement> statement)
            : m_condition(std::move(condition)), m_statement(std::move(statement)) {}

        inline void print_node() const override
        {
            std::cout << "WhileNode -> " << std::endl;

            std::cout << "\Condition: ";
            m_condition->print_node();
            std::cout << std::endl;

            std::cout << "\Statement: ";
            m_statement->print_node();
            std::cout << std::endl;
        }

    private:
        std::unique_ptr<Predicate> m_condition;
        std::unique_ptr<Statement> m_statement;
    };
}
#endif