#include "asema.h"
#include <iostream>

void Asema::tulosta() const
{
    for (int rivi = 0; rivi < 8; rivi++)
    {
        for (int linja = 0; linja < 8; linja++)
        {
            std::cout << "+---";
        }

        std::cout << "+" << std::endl;
        for (int linja = 0; linja < 8; linja++)
        {
            std::cout << "|";
            int nappula = _lauta[rivi][linja];
            switch (nappula)
            {
            case wR: std::cout << " R "; break;
            case wN: std::cout << " N "; break;
            case wB: std::cout << " B "; break;
            case wQ: std::cout << " Q "; break;
            case wK: std::cout << " K "; break;
            case wP: std::cout << " P "; break;
            case bR: std::cout << " r "; break;
            case bN: std::cout << " n "; break;
            case bB: std::cout << " b "; break;
            case bQ: std::cout << " q "; break;
            case bK: std::cout << " k "; break;
            case bP: std::cout << " p "; break;
            default: std::cout << "   "; break;



            }
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