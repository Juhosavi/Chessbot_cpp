#include "asema.h"
#include <iostream>

void Asema::tulosta() const
{
    for (int j = 0; j < 9; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            std::cout << "+---";
        }

        std::cout << "+" << std::endl;
        for (int k = 0; k < 8; k++)
        {
            std::cout << "|";
            std::cout << " x ";
        }
 

        std::cout << "|" << std::endl;
    }
    
    for (int h = 0; h < 8; h++)
    {
        std::cout << "+---";
    }
    std::cout << "+";
}

void Asema::tyhjenna()
{

}