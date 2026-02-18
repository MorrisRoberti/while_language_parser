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
            auto t = l.nextToken();
            std::cout << t.getTokenTypeString() << ": " << t.getValue() << std::endl;
        }
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}