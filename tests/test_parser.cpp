#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include <vector>

#include "../include/Parser.hpp"

TEST(ParserTest, BasicAssignment)
{
    auto stream = std::make_unique<std::istringstream>("x := 10;");

    WhileParser::Parser parser(std::move(stream));

    auto root = parser.parse();

    auto other_root = std::make_unique<WhileParser::RootNode>();
    other_root->addNode(
        std::move(std::make_unique<WhileParser::AssignmentNode>("x", std::move(std::make_unique<WhileParser::ExpressionNode>("10")))));

    EXPECT_TRUE(root->isEqual(other_root.get()));
}