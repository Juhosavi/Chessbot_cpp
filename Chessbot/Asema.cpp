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

void Asema::tee_siirto(const Siirto& siirto, int pelaaja)
{
    if (onko_laillinen_siirto(siirto, pelaaja) == false)
    {
        std::cout << "Ei laillinen siirto!" << std::endl;
        return; // Palataan eik� tehd� siirtoa
    }

    // Siirto on laillinen, suoritetaan siirto
    int lahto_rivi = siirto._a_r;
    int lahto_linja = siirto._a_l;
    int kohde_rivi = siirto._l_r;
    int kohde_linja = siirto._l_l;

    _lauta[kohde_rivi][kohde_linja] = _lauta[lahto_rivi][lahto_linja];
    _lauta[lahto_rivi][lahto_linja] = NA;
}

void Asema::kysy_siirto(int pelaaja, int& lahto_rivi, int& lahto_linja, int& kohde_rivi, int& kohde_linja)
{
    std::string syote;

    while (true)
    {
        std::cout << "ANNA SIIRTO:  ";
        std::cin >> syote;

        // Muodostetaan Siirto-olio sy�tteest�
        Siirto siirto(syote);

        // Tarkistetaan, ett� siirto on laillinen
        if (onko_laillinen_siirto(siirto, pelaaja))
        {
            // Puretaan siirron tiedot
            lahto_rivi = siirto._a_r;
            lahto_linja = siirto._a_l;
            kohde_rivi = siirto._l_r;
            kohde_linja = siirto._l_l;
            break; // Hyv�ksyt��n siirto ja poistutaan silmukasta
        }
        else
        {
            std::cout << "Ei laillinen siirto! Yrita uudelleen." << std::endl;
        }
    }
}


bool Asema::on_vastustajan_nappula(int ruutu, int pelaaja) const
{
    return (pelaaja == VALKEA && ruutu >= bR && ruutu <= bP) ||
        (pelaaja == MUSTA && ruutu >= wR && ruutu <= wP);
}
bool Asema::onko_laillinen_siirto(const Siirto& siirto, int pelaaja) const
{
    std::vector<Siirto> sallitut_siirrot;

    int rivi = siirto._a_r;
    int linja = siirto._a_l;
    int nappula = _lauta[rivi][linja];

    // Varmistetaan, ett� nappula kuuluu pelaajalle
    if ((pelaaja == VALKEA && !(nappula >= wR && nappula <= wP)) ||
        (pelaaja == MUSTA && !(nappula >= bR && nappula <= bP)))
    {
        return false; // L�ht�ruudussa ei ole pelaajan nappulaa
    }

    // Lis�t��n raakasiirrot siirtojen joukkoon nappulan mukaan
    if (nappula == wR || nappula == bR) // Torni
    {
        anna_tornin_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    }
    // Lis�� muita nappulatyyppej� my�hemmin...

    // Tarkistetaan, onko siirto sallittujen siirtojen joukossa
    for (const auto& s : sallitut_siirrot)
    {
        if (s._a_r == siirto._a_r && s._a_l == siirto._a_l &&
            s._l_r == siirto._l_r && s._l_l == siirto._l_l)
        {
            return true; // Siirto l�ytyi sallitujen siirtojen joukosta
        }
    }

    return false; // Siirto ei ole laillinen
}


void Asema::anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    int nykyinen_rivi, nykyinen_linja;

    // YL�S RIVI PIENENEE
    nykyinen_rivi = rivi - 1; // Aloitetaan tornin yl�puolelta
    while (nykyinen_rivi >= 0) // Niin kauan kuin pysyt��n laudan sis�ll�
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
        nykyinen_rivi--; // Siirry seuraavalle riville yl�sp�in
    }

    // ALASP�IN RIVI KASVAA
    nykyinen_rivi = rivi + 1; // Aloitetaan tornin alapuolelta
    while (nykyinen_rivi <= 7) // Niin kauan kuin pysyt��n laudan sis�ll�
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
        nykyinen_rivi++; // Siirry seuraavalle riville alasp�in
    }

    // VASEMALLE SARAKE LINJA PIENENEE
    nykyinen_linja = linja - 1; // Aloitetaan yhdest� vasemmalle
    while (nykyinen_linja >= 0) // Niin kauan kuin pysyt��n laudan sis�ll�
    {
        if (_lauta[rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
        }
        else if (on_vastustajan_nappula(_lauta[rivi][nykyinen_linja], pelaaja))
        {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
            break; // Lopetetaan, koska t�rm�ttiin vastustajan nappulaan
        }
        else
        {
            break; // Oma nappula, pys�ytet��n suunta
        }
        nykyinen_linja--; // Siirry seuraavaan sarakkeeseen vasemmalle
    }

    // OIKEALLE SARAKE LINJA KASVAA
    nykyinen_linja = linja + 1; // Aloitetaan yhdest� oikealle
    while (nykyinen_linja <= 7) // Niin kauan kuin pysyt��n laudan sis�ll�
    {
        if (_lauta[rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
        }
        else if (on_vastustajan_nappula(_lauta[rivi][nykyinen_linja], pelaaja))
        {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
            break; // Lopetetaan, koska t�rm�ttiin vastustajan nappulaan
        }
        else
        {
            break; // Oma nappula, pys�ytet��n suunta
        }
        nykyinen_linja++; // Siirry seuraavaan sarakkeeseen oikealle
    }
}

void Asema::anna_lahetti_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    int nykyinen_rivi, nykyinen_linja;

    nykyinen_rivi = rivi - 1;
    nykyinen_linja = linja - 1;

    while (nykyinen_rivi >= 0 && nykyinen_linja >= 0)
    {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja);
        }
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][nykyinen_linja], pelaaja))
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
            break;
        }
        else
        {
            break;
        }
        nykyinen_rivi--;
        nykyinen_linja--;
    }
}
void Asema::anna_ratsu_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const 
{

}
void Asema::anna_sotilas_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{

}
void Asema::anna_daami_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{

}
void Asema::anna_kuningas_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{

}