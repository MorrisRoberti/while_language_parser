#include "../include/Lexer.hpp"
#include <iostream>

int main()
{
    try
    {

        WhileParser::Lexer l("/home/morris/Workspace/while_parser/program.wh");

        std::cout << "Reading program tokens..." << std::endl;

        while (l.isTokenAvailable())
        {
            std::cout << l.nextToken().getValue() << std::endl;
        }
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}