#include "../include/Lexer.hpp"
#include <iostream>

int main()
{
    try
    {

        std::cout << "Tokenizing..." << std::endl;

        WhileParser::Lexer lexer("/home/morris/Workspace/while_parser/program.wh", true, true);

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