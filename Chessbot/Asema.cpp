#include "asema.h"
#include <iostream>
#include "siirto.h"
#include <vector>

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
bool Asema::on_vastustajan_nappula(int ruutu, int pelaaja) const
{
    return (pelaaja == VALKEA && ruutu >= bR && ruutu <= bP) ||
        (pelaaja == MUSTA && ruutu >= wR && ruutu <= wP);
}

void Asema::anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    int nykyinen_rivi, nykyinen_linja;

    // YLÖS RIVI PIENENEE
    nykyinen_rivi = rivi - 1; // Aloitetaan tornin yläpuolelta
    while (nykyinen_rivi >= 0) // Niin kauan kuin pysytään laudan sisällä
    {
        if (_lauta[nykyinen_rivi][linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, linja));
        }
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][linja], pelaaja))
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, linja));
            break;
        }
        else
        {
            break;
        }
        nykyinen_rivi--; // Siirry seuraavalle riville ylöspäin
    }

    // ALASPÄIN RIVI KASVAA
    nykyinen_rivi = rivi + 1; // Aloitetaan tornin alapuolelta
    while (nykyinen_rivi <= 7) // Niin kauan kuin pysytään laudan sisällä
    {
        if (_lauta[nykyinen_rivi][linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, linja));
        }
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][linja], pelaaja))
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, linja));
            break;
        }
        else
        {
            break;
        }
        nykyinen_rivi++; // Siirry seuraavalle riville alaspäin
    }

    // VASEMALLE SARAKE LINJA PIENENEE
    nykyinen_linja = linja - 1; // Aloitetaan yhdestä vasemmalle
    while (nykyinen_linja >= 0) // Niin kauan kuin pysytään laudan sisällä
    {
        if (_lauta[rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
        }
        else if (on_vastustajan_nappula(_lauta[rivi][nykyinen_linja], pelaaja))
        {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
            break; // Lopetetaan, koska törmättiin vastustajan nappulaan
        }
        else
        {
            break; // Oma nappula, pysäytetään suunta
        }
        nykyinen_linja--; // Siirry seuraavaan sarakkeeseen vasemmalle
    }

    // OIKEALLE SARAKE LINJA KASVAA
    nykyinen_linja = linja + 1; // Aloitetaan yhdestä oikealle
    while (nykyinen_linja <= 7) // Niin kauan kuin pysytään laudan sisällä
    {
        if (_lauta[rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
        }
        else if (on_vastustajan_nappula(_lauta[rivi][nykyinen_linja], pelaaja))
        {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
            break; // Lopetetaan, koska törmättiin vastustajan nappulaan
        }
        else
        {
            break; // Oma nappula, pysäytetään suunta
        }
        nykyinen_linja++; // Siirry seuraavaan sarakkeeseen oikealle
    }
}
