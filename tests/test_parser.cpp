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

    auto root = parser.parse();

    auto other_root = std::make_unique<WhileParser::RootNode>();
    other_root->addNode(std::move(simpleAssignment("x", "10")));

    EXPECT_TRUE(root->isEqual(other_root.get()));
}

TEST(ParserTest, BasicIf)
{
    auto stream = std::make_unique<std::istringstream>("x := 10;");

    WhileParser::Parser parser(std::move(stream));

    auto root = parser.parse();

    auto statement = std::move(simpleAssignment("x", "10"));

    auto other_root = std::make_unique<WhileParser::RootNode>();

    other_root->addNode(
        std::move(std::make_unique<WhileParser::AssignmentNode>("x", std::move(std::make_unique<WhileParser::ExpressionNode>("10")))));

    EXPECT_TRUE(root->isEqual(other_root.get()));
}