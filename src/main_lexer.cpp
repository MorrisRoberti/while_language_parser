#include "../include/Lexer.hpp"
#include <iostream>
#include <memory>
#include <sstream>

int main()
{
    try
    {

        std::cout << "Tokenizing..." << std::endl;

        auto stream = std::make_unique<std::istringstream>("x := 10; ");

        WhileParser::Lexer lexer(std::move(stream), true, true);
        // WhileParser::Lexer lexer("/home/morris/Workspace/while_parser/program.wh", true, true);

        while (lexer.isTokenAvailable())
        {
            auto token = lexer.nextToken();

            std::cout << token.getTokenTypeString() << ": " << token.getValue() << std::endl;
        }
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}