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
        virtual void printNode(int indent = 0) const = 0;
        inline void printIndentation(const std::string &print_string, int indent) const
        {
            for (int i = 0; i < indent; ++i)
            {
                if (i == indent - 1)
                    std::cout << "|-- ";
                else
                    std::cout << "|   ";
            }
            std::cout << print_string << std::endl;
        }
        virtual ~ASTNode() {}
    };

    // Meta-node that represents the entrypoint
    class RootNode : public ASTNode
    {
    public:
        RootNode() : m_children(std::vector<std::unique_ptr<ASTNode>>{}) {}

        inline void printNode(int indent = 0) const override
        {
            std::cout << "RootNode" << std::endl;
            std::for_each(m_children.begin(), m_children.end(), [this](const std::unique_ptr<ASTNode> &child)
                          { child->printNode(1); });
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
        inline void printNode(int indent = 0) const override
        {
            printIndentation("ExpressionNode", indent);
            printIndentation(m_terminal_expression, indent + 2);
        }

    private:
        std::string m_terminal_expression;
    };

    class StatementNode : public ASTNode
    {
    public:
        virtual void printNode(int indent = 0) const = 0;
    };

    class PredicateNode : public ASTNode
    {
    public:
        PredicateNode() = default;
        PredicateNode(const std::string &terminal_predicate) : m_terminal_predicate(terminal_predicate) {}

        virtual void printNode(int indent = 0) const override
        {
            printIndentation("PredicateNode", indent);
            printIndentation(m_terminal_predicate, indent + 2);
        }

    private:
        std::string m_terminal_predicate;
    };

    class MathOpNode : public ASTNode
    {
    public:
        virtual void printNode(int indent = 0) const = 0;
    };

    class BooleanOpNode : public ASTNode
    {
    public:
        virtual void printNode(int indent = 0) const = 0;
    };

    class RelationalOpNode : public ASTNode
    {
    public:
        virtual void printNode(int indent = 0) const = 0;
    };

    // Statement productions
    class AssignmentNode : public StatementNode
    {
    public:
        AssignmentNode(const std::string &var_name, std::unique_ptr<ExpressionNode> expr)
            : m_variable_name(var_name), m_expression(std::move(expr)) {}

        inline void printNode(int indent = 0) const override
        {
            printIndentation("AssignmentNode", indent);
            printIndentation("Identifier", indent + 1);
            printIndentation(m_variable_name, indent + 2);
            printIndentation("Expression", indent + 1);
            m_expression->printNode(indent + 2);
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

        inline void printNode(int indent = 0) const override
        {
            printIndentation("IfNode", indent);
            printIndentation("Condition", indent + 1);
            m_condition->printNode(indent + 2);

            printIndentation("ThenBranch", indent + 1);
            m_then_branch->printNode(indent + 2);

            printIndentation("ElseBranch", indent + 1);
            m_else_branch->printNode(indent + 2);
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

        inline void printNode(int indent = 0) const override
        {
            printIndentation("SkipNode", indent);
        }
    };

    class SequenceNode : public StatementNode
    {
    public:
        SequenceNode(std::vector<std::unique_ptr<StatementNode>> statement_list) : m_statement_list(std::move(statement_list)) {}

        inline void printNode(int indent = 0) const override
        {
            printIndentation("SequenceNode", indent);

            std::for_each(m_statement_list.begin(), m_statement_list.end(), [this, indent](const std::unique_ptr<StatementNode> &statement)
                          {
                            printIndentation("SequenceNode", indent + 1);
                            statement->printNode(indent + 2); });
        }

    private:
        std::vector<std::unique_ptr<StatementNode>> m_statement_list;
    };

    class WhileNode : public StatementNode
    {
    public:
        WhileNode(std::unique_ptr<PredicateNode> condition, std::unique_ptr<StatementNode> statement)
            : m_condition(std::move(condition)), m_statement(std::move(statement)) {}

        inline void printNode(int indent = 0) const override
        {
            printIndentation("WhileNode", indent);
            printIndentation("Condition", indent + 1);
            m_condition->printNode(indent + 2);

            printIndentation("Statement", indent + 1);
            m_statement->printNode(indent + 2);
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

        inline void printNode(int indent = 0) const override
        {
            printIndentation("MathExpressionNode", indent);
            printIndentation("MathOp", indent + 1);
            printIndentation("" + m_math_operation, indent + 2);

            printIndentation("LeftSideExpression", indent + 1);
            m_left_expression->printNode(indent + 2);

            printIndentation("RightSideExpression", indent + 1);
            m_right_expression->printNode(indent + 2);
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

        inline void printNode(int indent = 0) const override
        {
            printIndentation("NotPredicateNode", indent);
            printIndentation("Predicate", indent + 1);
            m_predicate->printNode(indent + 2);
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

        inline void printNode(int indent = 0) const override
        {
            printIndentation("BooleanPredicateNode", indent);

            printIndentation("BooleanOp", indent + 1);
            printIndentation(m_boolean_operation, indent + 2);

            printIndentation("LeftSidePredicate", indent + 1);
            m_left_predicate->printNode(indent + 2);

            printIndentation("RightSidePredicate", indent + 1);
            m_right_predicate->printNode(indent + 2);
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

        inline void printNode(int indent = 0) const override
        {
            printIndentation("RelationalPredicateNode", indent);

            printIndentation("RelationalOp", indent + 1);
            printIndentation(m_relational_operation, indent + 2);

            printIndentation("LeftSideStatement", indent + 1);
            m_left_statement->printNode(indent + 2);

            printIndentation("RightSideStatement", indent + 1);
            m_right_statement->printNode(indent + 2);
        }

    private:
        std::string m_relational_operation;
        std::unique_ptr<StatementNode> m_left_statement;
        std::unique_ptr<StatementNode> m_right_statement;
    };

}
#endif