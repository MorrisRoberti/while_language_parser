#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include <vector>

#include "../include/Parser.hpp"

// helpers to easily create program elements without dealing with messy pointers
std::unique_ptr<WhileParser::AssignmentNode> simpleAssignment(const std::string &variable, const std::string &simpleExpr)
{
    return std::move(std::make_unique<WhileParser::AssignmentNode>(variable, std::move(std::make_unique<WhileParser::ExpressionNode>(simpleExpr))));
}

// it would be better to create 3/4 templatic functions that represent various types ASTNodes, namely: UnaryNodes, BinaryNodes, If/While etc.

TEST(ParserTest, BasicAssignment)
{
    auto stream = std::make_unique<std::istringstream>("x := 10;");

    WhileParser::Parser parser(std::move(stream));

    auto ast_to_test = parser.parse();

    auto correct_ast = std::make_unique<WhileParser::RootNode>();
    correct_ast->addNode(std::move(simpleAssignment("x", "10")));

    EXPECT_TRUE(ast_to_test->isEqual(correct_ast.get()));
}

TEST(ParserTest, MathOperatorPrecedence)
{
    auto stream = std::make_unique<std::istringstream>("x := 10 + 5 * y;");

    WhileParser::Parser parser(std::move(stream));

    auto ast_to_test = parser.parse();

    auto correct_ast = std::make_unique<WhileParser::RootNode>();

    correct_ast->addNode(std::make_unique<WhileParser::AssignmentNode>("x", std::make_unique<WhileParser::MathExpressionNode>(
                                                                                "+",
                                                                                std::make_unique<WhileParser::ExpressionNode>("10"),
                                                                                std::make_unique<WhileParser::MathExpressionNode>("*", std::make_unique<WhileParser::ExpressionNode>("5"), std::make_unique<WhileParser::ExpressionNode>("y")))));

    EXPECT_TRUE(ast_to_test->isEqual(correct_ast.get()));
}

TEST(ParserTest, MathOperatorAssociativity)
{
    auto stream = std::make_unique<std::istringstream>("x := 10 - 5 - 2;");

    WhileParser::Parser parser(std::move(stream));

    auto ast_to_test = parser.parse();

    auto correct_ast = std::make_unique<WhileParser::RootNode>();

    correct_ast->addNode(std::make_unique<WhileParser::AssignmentNode>("x", std::make_unique<WhileParser::MathExpressionNode>(
                                                                                "-",
                                                                                std::make_unique<WhileParser::MathExpressionNode>("-", std::make_unique<WhileParser::ExpressionNode>("10"), std::make_unique<WhileParser::ExpressionNode>("5")),
                                                                                std::make_unique<WhileParser::ExpressionNode>("2"))));

    EXPECT_TRUE(ast_to_test->isEqual(correct_ast.get()));
}

TEST(ParserTest, BasicIf)
{
    auto stream = std::make_unique<std::istringstream>("x := 10; if x > 10 then skip else skip endif");

    WhileParser::Parser parser(std::move(stream));

    auto ast_to_test = parser.parse();

    auto correct_ast = std::make_unique<WhileParser::RootNode>();

    correct_ast->addNode(
        std::move(simpleAssignment("x", "10")));

    auto predicate = std::make_unique<WhileParser::RelationalPredicateNode>(">", (std::make_unique<WhileParser::ExpressionNode>("x")), std::make_unique<WhileParser::ExpressionNode>("10"));

    auto then_expr = std::make_unique<WhileParser::SkipNode>();

    auto else_expr = std::make_unique<WhileParser::SkipNode>();

    correct_ast->addNode(std::move(std::make_unique<WhileParser::IfNode>(std::move(predicate), std::move(then_expr), std::move(else_expr))));

    EXPECT_TRUE(ast_to_test->isEqual(correct_ast.get()));
}

TEST(ParserTest, NotPredicateIf)
{
    auto stream = std::make_unique<std::istringstream>("x := 10; if not x > 10 then skip else skip endif");

    WhileParser::Parser parser(std::move(stream));

    auto ast_to_test = parser.parse();

    auto correct_ast = std::make_unique<WhileParser::RootNode>();

    correct_ast->addNode(
        std::move(simpleAssignment("x", "10")));

    auto predicate = std::make_unique<WhileParser::RelationalPredicateNode>(">", (std::make_unique<WhileParser::ExpressionNode>("x")), std::make_unique<WhileParser::ExpressionNode>("10"));

    auto not_predicate = std::make_unique<WhileParser::NotPredicateNode>(std::move(predicate));

    auto then_expr = std::make_unique<WhileParser::SkipNode>();

    auto else_expr = std::make_unique<WhileParser::SkipNode>();

    correct_ast->addNode(std::move(std::make_unique<WhileParser::IfNode>(std::move(not_predicate), std::move(then_expr), std::move(else_expr))));

    EXPECT_TRUE(ast_to_test->isEqual(correct_ast.get()));
}

TEST(ParserTest, MixedPredicateIf)
{
    auto stream = std::make_unique<std::istringstream>("x := 10; if x > 10 and true then skip else skip endif");

    WhileParser::Parser parser(std::move(stream));

    auto ast_to_test = parser.parse();

    auto correct_ast = std::make_unique<WhileParser::RootNode>();

    correct_ast->addNode(
        std::move(simpleAssignment("x", "10")));

    auto math_predicate = std::make_unique<WhileParser::RelationalPredicateNode>(">", (std::make_unique<WhileParser::ExpressionNode>("x")), std::make_unique<WhileParser::ExpressionNode>("10"));

    auto bool_predicate = std::make_unique<WhileParser::BooleanPredicateNode>("and", std::move(math_predicate), std::make_unique<WhileParser::PredicateNode>("true"));

    auto then_expr = std::make_unique<WhileParser::SkipNode>();

    auto else_expr = std::make_unique<WhileParser::SkipNode>();

    correct_ast->addNode(std::move(std::make_unique<WhileParser::IfNode>(std::move(bool_predicate), std::move(then_expr), std::move(else_expr))));

    EXPECT_TRUE(ast_to_test->isEqual(correct_ast.get()));
}

TEST(ParserTest, BasicWhile)
{
    auto stream = std::make_unique<std::istringstream>("x := 10; while x > 10 do skip endwhile");

    WhileParser::Parser parser(std::move(stream));

    auto ast_to_test = parser.parse();

    auto correct_ast = std::make_unique<WhileParser::RootNode>();

    correct_ast->addNode(
        std::move(simpleAssignment("x", "10")));

    auto predicate = std::make_unique<WhileParser::RelationalPredicateNode>(">", (std::make_unique<WhileParser::ExpressionNode>("x")), std::make_unique<WhileParser::ExpressionNode>("10"));

    auto do_expr = std::make_unique<WhileParser::SkipNode>();

    correct_ast->addNode(std::move(std::make_unique<WhileParser::WhileNode>(std::move(predicate), std::move(do_expr))));

    EXPECT_TRUE(ast_to_test->isEqual(correct_ast.get()));
}