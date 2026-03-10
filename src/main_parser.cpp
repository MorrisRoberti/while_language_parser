// #include "../include/Parser.hpp"
#include <iostream>
#include "./ParserrX.cpp"

int main()
{
    try
    {

        std::cout << "Parsing..." << std::endl;

        WhileParser::ParserrX parser("/home/morris/Workspace/while_parser/program.wh");

        auto root = parser.parse();

        root->printNode();
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}