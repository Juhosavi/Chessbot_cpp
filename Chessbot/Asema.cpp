#include "asema.h"
#include <iostream>
#include "siirto.h"
#include "vector";

void Asema::tulosta() const
{


    for (int rivi = 0; rivi < 8; rivi++)
    {
        // Tulostetaan vaakaviivat
        std::cout << "  ";
        for (int linja = 0; linja < 8; linja++)
        {
            std::cout << "+---";
        }
        std::cout << "+" << std::endl;

        // Tulostetaan rivin numero laudan vasemmalle puolelle
        std::cout << 8 - rivi << " ";

        // Tulostetaan ruudut ja nappulat
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

        // Tulostetaan rivin numero laudan oikealle puolelle
        std::cout << "|" << std::endl;
    }

    // Tulostetaan viimeinen vaakaviiva
    std::cout << "  ";
    for (int h = 0; h < 8; h++)
    {
        std::cout << "+---";
    }
    std::cout << "+" << std::endl;

    // Tulostetaan linjat (sarakekirjaimet)
    std::cout << "    a   b   c   d   e   f   g   h" << std::endl;
}

void Asema::tyhjenna()
{
    std::cout << "\033[2J\033[H";

    for (int rivi = 0; rivi < 8;rivi++)
    {
        for (int linja = 0; linja < 8;linja++)
        {
            _lauta[rivi][linja] = NA;
        }
    }
}
void Asema::etsi_kuningas(int nappula, int& rivi, int& linja) const
{
    for (int i = 0; i < 8;i++)
    {
        for (int j = 0; j < 8;j++)
        {
            if (_lauta[i][j] == nappula)
            {
                rivi = i;
                linja = j;
                std::cout << "Loyty: ";
            }
        }
    }
}

void Asema::tee_siirto(const Siirto& siirto)
{
  
    int lahto_rivi = siirto._a_r;
    int lahto_linja = siirto._a_l;
    int kohde_rivi = siirto._l_r;
    int kohde_linja = siirto._l_l;

    _lauta[kohde_rivi][kohde_linja] = _lauta[lahto_rivi][lahto_linja];
    _lauta[lahto_rivi][lahto_linja] = NA;
}
void anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{

}