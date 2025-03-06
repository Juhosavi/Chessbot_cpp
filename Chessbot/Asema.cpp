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
               
            }
        }
    }
}

void Asema::tee_siirto(const Siirto& siirto, int pelaaja)
{
   
    //if (onko_laillinen_siirto(siirto, pelaaja) == false)
    //{
    //    std::cout << "Ei laillinen siirto!" << std::endl;
    //    return; // Palataan eik� tehd� siirtoa
    //}

    // Siirto on laillinen, suoritetaan siirto
    int lahto_rivi = siirto._a_r;
    int lahto_linja = siirto._a_l;
    int kohde_rivi = siirto._l_r;
    int kohde_linja = siirto._l_l;

    // K�sitell��n ohestaly�nti
    if (siirto._ohestalyonti) {
        int vastustajan_rivi = (pelaaja == VALKEA) ? kohde_rivi + 1 : kohde_rivi - 1;
        _lauta[vastustajan_rivi][kohde_linja] = NA;
    }

    // P�ivitet��n kaksoisaskeleen linja
    if (abs(lahto_rivi - kohde_rivi) == 2 && (_lauta[lahto_rivi][lahto_linja] == wP || _lauta[lahto_rivi][lahto_linja] == bP)) {
        _kaksoisaskel_linjalla = kohde_linja;
    }
    else {
        _kaksoisaskel_linjalla = -1;
    }

    // Tarkistetaan, onko kyseess� linnoitus
    if (_lauta[lahto_rivi][lahto_linja] == wK || _lauta[lahto_rivi][lahto_linja] == bK) {
        if (pelaaja == VALKEA) {
            if (lahto_linja == 4 && kohde_linja == 6 && _valkea_lyhyt_linna_sallittu) { // Valkoisen lyhyt linnoitus
                _lauta[7][5] = wR;
                _lauta[7][7] = NA;
            }
            else if (lahto_linja == 4 && kohde_linja == 2 && _valkea_pitka_linna_sallittu) { // Valkoisen pitk� linnoitus
                _lauta[7][3] = wR;
                _lauta[7][0] = NA;
            }
            // Asetetaan linnoitusliput falseksi, kun valkoinen kuningas liikkuu
            _valkea_lyhyt_linna_sallittu = false;
            _valkea_pitka_linna_sallittu = false;
        }
        else if (pelaaja == MUSTA) {
            if (lahto_linja == 4 && kohde_linja == 6 && _musta_lyhyt_linna_sallittu) { // Mustan lyhyt linnoitus
                _lauta[0][5] = bR;
                _lauta[0][7] = NA;
            }
            else if (lahto_linja == 4 && kohde_linja == 2 && _musta_pitka_linna_sallittu) { // Mustan pitk� linnoitus
                _lauta[0][3] = bR;
                _lauta[0][0] = NA;
            }
            // Asetetaan linnoitusliput falseksi, kun musta kuningas liikkuu
            _musta_lyhyt_linna_sallittu = false;
            _musta_pitka_linna_sallittu = false;
        }
    }

    // Asetetaan linnoitusliput falseksi, kun valkoinen torni liikkuu
    if (_lauta[lahto_rivi][lahto_linja] == wR) {
        if (lahto_rivi == 7 && lahto_linja == 0) {
            _valkea_pitka_linna_sallittu = false;
        }
        if (lahto_rivi == 7 && lahto_linja == 7) {
            _valkea_lyhyt_linna_sallittu = false;
        }
    }

    // Asetetaan linnoitusliput falseksi, kun musta torni liikkuu
    if (_lauta[lahto_rivi][lahto_linja] == bR) {
        if (lahto_rivi == 0 && lahto_linja == 0) {
            _musta_pitka_linna_sallittu = false;
        }
        if (lahto_rivi == 0 && lahto_linja == 7) {
            _musta_lyhyt_linna_sallittu = false;
        }
    }

    //Tehd��n siirto
    _lauta[kohde_rivi][kohde_linja] = _lauta[lahto_rivi][lahto_linja];
    _lauta[lahto_rivi][lahto_linja] = NA;

    //Suoriteaan korotus
    if (siirto._korotettava_nappula != NA)
    {
        _lauta[lahto_rivi][lahto_linja] = NA;
        _lauta[kohde_rivi][kohde_linja] = siirto._korotettava_nappula;
    }


    // Vaihdetaan vuoroa
    _siirtovuoro = (_siirtovuoro == VALKEA) ? MUSTA : VALKEA;
}

void Asema::kysy_siirto(int pelaaja, int& lahto_rivi, int& lahto_linja, int& kohde_rivi, int& kohde_linja)
{
    std::string syote;

    while (true)
    {
        std::cout << "Pelaaja " << pelaaja << ", ANNA SIIRTO:  ";
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
    sallitut_siirrot.reserve(100);

    int rivi = siirto._a_r;
    int linja = siirto._a_l;
    int nappula = _lauta[rivi][linja];

    // Tarkistetaan, onko kyseess� linnoitus
    if (pelaaja == VALKEA) {
        if (siirto._a_r == 7 && siirto._a_l == 4) {
            if (siirto._l_l == 6 && _valkea_lyhyt_linna_sallittu) { // Lyhyt linnoitus
                return true;
            }
            else if (siirto._l_l == 2 && _valkea_pitka_linna_sallittu) { // Pitk� linnoitus
                return true;
            }
        }
    }
    else if (pelaaja == MUSTA) {
        if (siirto._a_r == 0 && siirto._a_l == 4) {
            if (siirto._l_l == 6 && _musta_lyhyt_linna_sallittu) { // Lyhyt linnoitus
                return true;
            }
            else if (siirto._l_l == 2 && _musta_pitka_linna_sallittu) { // Pitk� linnoitus
                return true;
            }
        }
    }

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
    if (nappula == wB || nappula == bB) //l�hetti
    {
        anna_lahetti_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    }
    if (nappula == wN || nappula == bN) //Ratsu
    {
        anna_ratsu_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    }
    if (nappula == wQ || nappula == bQ) //Kuningatar
    {
        anna_daami_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    }
    if (nappula == wK || nappula == bK) //Kuningatar
    {
        anna_kuningas_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    }
    if (nappula == wP || nappula == bP) //Kuningatar
    {
        anna_sotilas_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
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
    //VASEMMALLE YL�S
    int nykyinen_rivi, nykyinen_linja;

    nykyinen_rivi = rivi - 1;
    nykyinen_linja = linja - 1;

    while (nykyinen_rivi >= 0 && nykyinen_linja >= 0)
    {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
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

    //OIKEALLE ALAS
    nykyinen_rivi = rivi +1;
    nykyinen_linja = linja +1;
    while (nykyinen_rivi <= 7 && nykyinen_linja <= 7)
    {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
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
        nykyinen_rivi++;
        nykyinen_linja++;

    }
    //VASEN ALAS
    nykyinen_rivi = rivi +1;
    nykyinen_linja = linja -1;
    while (nykyinen_rivi <= 7 && nykyinen_linja >= 0)
    {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
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
        nykyinen_rivi++;
        nykyinen_linja--;

    }
    //OIKEALLE YL�S
    nykyinen_rivi = rivi -1;
    nykyinen_linja = linja +1;
    while (nykyinen_rivi >= 0 && nykyinen_linja <= 7)
    {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
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
        nykyinen_linja++;

    }
}
void Asema::anna_ratsu_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{

    // Alustetaan ratsun siirrot shakkilaudalla
    const int ratsun_siirrot[8][2] =
    {
        {-2, -1}, // Ratsun siirto 2 ruutua yl�s ja 1 ruutu vasemmalle
        {-1, -2}, // Ratsun siirto 1 ruutu yl�s ja 2 ruutua vasemmalle
        {1, -2},  // Ratsun siirto 1 ruutu alas ja 2 ruutua vasemmalle
        {2, -1},  // Ratsun siirto 2 ruutua alas ja 1 ruutu vasemmalle
        {2, 1},   // Ratsun siirto 2 ruutua alas ja 1 ruutu oikealle
        {1, 2},   // Ratsun siirto 1 ruutu alas ja 2 ruutua oikealle
        {-1, 2},  // Ratsun siirto 1 ruutu yl�s ja 2 ruutua oikealle
        {-2, 1}   // Ratsun siirto 2 ruutua yl�s ja 1 ruutu oikealle
    };

    // K�yd��n l�pi kaikki mahdolliset ratsun siirrot
    for (int i = 0; i < 8; ++i)
    {
        // Lasketaan uusi rivi ja linja ratsun siirron perusteella
        int uusi_rivi = rivi + ratsun_siirrot[i][0];
        int uusi_linja = linja + ratsun_siirrot[i][1];

        // Tarkistetaan, ett� uusi rivi ja linja ovat shakkilaudan sis�ll�
        if (uusi_rivi >= 0 && uusi_rivi < 8 && uusi_linja >= 0 && uusi_linja < 8)
        {
            // Tarkistetaan, ett� uusi ruutu on tyhj� tai siin� on vastustajan nappula
            if (_lauta[uusi_rivi][uusi_linja] == NA || on_vastustajan_nappula(_lauta[uusi_rivi][uusi_linja], pelaaja))
            {
                // Lis�t��n siirto mahdollisiin siirtoihin
                siirrot.push_back(Siirto(rivi, linja, uusi_rivi, uusi_linja));
            }
            
        }
    }
}
void Asema::anna_daami_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    // Daami voi liikkua kuin torni
    anna_tornin_raakasiirrot(rivi, linja, pelaaja, siirrot);

    // Daami voi liikkua kuin l�hetti
    anna_lahetti_raakasiirrot(rivi, linja, pelaaja, siirrot);
}
void Asema::anna_kuningas_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    const int kuningas_siirrot[8][2] =
    {
        {-1, -1}, // 1 ruutu yl�s ja 1 ruutu vasemmalle
        {-1,  0}, // 1 ruutu yl�s
        {-1,  1}, // 1 ruutu yl�s ja 1 ruutu oikealle
        { 0, -1}, // 1 ruutu vasemmalle
        { 0,  1}, // 1 ruutu oikealle
        { 1, -1}, // 1 ruutu alas ja 1 ruutu vasemmalle
        { 1,  0}, // 1 ruutu alas
        { 1,  1}  // 1 ruutu alas ja 1 ruutu oikealle
    };
    // K�yd��n l�pi kaikki mahdolliset Kuninkaan siirrot
    for (int i = 0; i < 8; ++i)
    {
        // Lasketaan uusi rivi ja linja ratsun siirron perusteella
        int uusi_rivi = rivi + kuningas_siirrot[i][0];
        int uusi_linja = linja + kuningas_siirrot[i][1];

        // Tarkistetaan, ett� uusi rivi ja linja ovat shakkilaudan sis�ll�
        if (uusi_rivi >= 0 && uusi_rivi < 8 && uusi_linja >= 0 && uusi_linja < 8)
        {
            // Tarkistetaan, ett� uusi ruutu on tyhj� tai siin� on vastustajan nappula
            if (_lauta[uusi_rivi][uusi_linja] == NA || on_vastustajan_nappula(_lauta[uusi_rivi][uusi_linja], pelaaja))
            {
                // Lis�t��n siirto mahdollisiin siirtoihin
                siirrot.push_back(Siirto(rivi, linja, uusi_rivi, uusi_linja));
            }

        }
    }
  
}
void Asema::hae_linnoitukset(int pelaaja, int rivi, int linja, std::vector<Siirto>& siirrot) const
{
    // Tarkistetaan linnoitusmahdollisuudet
    if (pelaaja == VALKEA && rivi == 7 && linja == 4)
    {
        if (_valkea_lyhyt_linna_sallittu && _lauta[7][5] == NA && !onko_ruutu_uhattu(7, 4, MUSTA) && !onko_ruutu_uhattu(7, 5, MUSTA))
        {
            siirrot.push_back(Siirto(7, 4, 7, 6)); // Lyhyt linnoitus
        }
        if (_valkea_pitka_linna_sallittu && _lauta[7][3] == NA && !onko_ruutu_uhattu(7, 4, MUSTA) && !onko_ruutu_uhattu(7, 3, MUSTA))
        {
            siirrot.push_back(Siirto(7, 4, 7, 2)); // Pitk� linnoitus
        }
    }

    else if (pelaaja == MUSTA && rivi == 0 && linja == 4)
    {
        if (_musta_lyhyt_linna_sallittu && _lauta[0][5] == NA && !onko_ruutu_uhattu(0, 4, VALKEA) && !onko_ruutu_uhattu(0, 5, VALKEA))
        {
            siirrot.push_back(Siirto(0, 4, 0, 6)); // Lyhyt linnoitus
        }
        if (_musta_pitka_linna_sallittu && _lauta[0][3] == NA && !onko_ruutu_uhattu(0, 4, VALKEA) && !onko_ruutu_uhattu(0, 3, VALKEA))
        {
            siirrot.push_back(Siirto(0, 4, 0, 2)); // Pitk� linnoitus
        }
    }
}
void Asema::anna_sotilas_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    int suunta;
    if (pelaaja == VALKEA)
    {
        suunta = -1; // Valkoinen liikkuu yl�sp�in
    }
    else
    {
        suunta = 1; // Musta liikkuu alasp�in
    }

    int aloitusrivi;
    if (pelaaja == VALKEA)
    {
        aloitusrivi = 6; // Valkoisen aloitusrivi on 6
    }
    else
    {
        aloitusrivi = 1; // Mustan aloitusrivi on 1
    }

    int ylennysrivi;
    if (pelaaja == VALKEA)
    {
        ylennysrivi = 0; // Valkoisen ylennysrivi on 0
    }
    else
    {
        ylennysrivi = 7; // Mustan ylennysrivi on 7
    }

    // Yksi askel eteenp�in
    if (_lauta[rivi + suunta][linja] == NA)
    {
        if (rivi + suunta == ylennysrivi)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, false, (pelaaja == VALKEA ? wQ : bQ)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, false, (pelaaja == VALKEA ? wR : bR)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, false, (pelaaja == VALKEA ? wB : bB)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, false, (pelaaja == VALKEA ? wN : bN)));

        }
        else
        {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja));
        }

        // Kaksi askelta eteenp�in aloitusrivilt�
        if (rivi == aloitusrivi && _lauta[rivi + 2 * suunta][linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi + 2 * suunta, linja));
        }
    }
    //sy�nti kulmittain vaasemmalle
    if (linja > 0 && on_vastustajan_nappula(_lauta[rivi + suunta][linja - 1], pelaaja))
    {
        if (rivi + suunta == ylennysrivi)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, false, (pelaaja == VALKEA ? wQ : bQ)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, false, (pelaaja == VALKEA ? wR : bR)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, false, (pelaaja == VALKEA ? wB : bB)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, false, (pelaaja == VALKEA ? wN : bN)));
        }
        else
        {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1));
        }
    }

    // Ly�nti kulmittain oikealle
    if (linja < 7 && on_vastustajan_nappula(_lauta[rivi + suunta][linja + 1], pelaaja))
    {
        if (rivi + suunta == ylennysrivi)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, false, (pelaaja == VALKEA ? wQ : bQ)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, false, (pelaaja == VALKEA ? wR : bR)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, false, (pelaaja == VALKEA ? wB : bB)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, false, (pelaaja == VALKEA ? wN : bN)));
        }
        else
        {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1));
        }
    }

    // Ohestaly�nti
    if (_kaksoisaskel_linjalla != -1 && (linja == _kaksoisaskel_linjalla - 1 || linja == _kaksoisaskel_linjalla + 1)) {
        if ((pelaaja == VALKEA && rivi == 3) || (pelaaja == MUSTA && rivi == 4)) {
            int ohestalyonti_rivi = (pelaaja == VALKEA) ? 2 : 5;
            siirrot.push_back(Siirto(rivi, linja, ohestalyonti_rivi, _kaksoisaskel_linjalla, true));
        }
    }
}

void Asema::anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const
{   
    int rivi = 0;
    int sarake = 0;
    // K�yd��n l�pi kaikki ruudut
    for (int rivi = 0; rivi < 8; rivi++)
    {
        for (int sarake = 0; sarake < 8; sarake++)
        {
            int nappula = _lauta[rivi][sarake];
            if (_siirtovuoro == VALKEA && nappula == wK)
            {
                hae_linnoitukset(pelaaja, rivi, sarake, siirrot);
            }
            if (_siirtovuoro == MUSTA && nappula == bK)
            {
                hae_linnoitukset(pelaaja, rivi, sarake, siirrot);
            }
            if (pelaaja == VALKEA && nappula == wR) {
                anna_tornin_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == VALKEA && nappula == wN) {
                anna_ratsu_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == VALKEA && nappula == wB) {
                anna_lahetti_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == VALKEA && nappula == wQ) {
                anna_daami_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == VALKEA && nappula == wK) {
                anna_kuningas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == VALKEA && nappula == wP) {
                anna_sotilas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == MUSTA && nappula == bR) {
                anna_tornin_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == MUSTA && nappula == bN) {
                anna_ratsu_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == MUSTA && nappula == bB) {
                anna_lahetti_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == MUSTA && nappula == bQ) {
                anna_daami_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (pelaaja == MUSTA && nappula == bK) {
                anna_kuningas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
                
            }
            else if (pelaaja == MUSTA && nappula == bP) {
                anna_sotilas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
        }
        
    }
}


bool Asema::onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaja) const
{
    std::vector<Siirto> raakasiirrot;
    raakasiirrot.reserve(100);
    anna_kaikki_raakasiirrot(uhkaava_pelaaja, raakasiirrot);

    for (const auto& rs : raakasiirrot)
    {
        if (rs._l_r == rivi && rs._l_l == linja)
        {
            return true;
        }
    }
    return false;
}

void Asema::anna_siirrot(std::vector<Siirto>& siirrot) const
{
    std::vector<Siirto> raakasiirrot;
    raakasiirrot.reserve(100);


    // generoidaan raakasiirrot, ja testataan ne yksi kerrallaan. 
    anna_kaikki_raakasiirrot(_siirtovuoro, raakasiirrot);
    for (auto& rs : raakasiirrot)
    {
        //tehd��n siirto
        Asema uusi_asema = *this;
        uusi_asema.tee_siirto(rs, _siirtovuoro);

        // etsit��n kuningas
        int k_rivi, k_linja;
        uusi_asema.etsi_kuningas((_siirtovuoro == VALKEA) ? wK : bK, k_rivi, k_linja);

        //tarkistetaan onko kuningas uhattu
        if (uusi_asema.onko_ruutu_uhattu(k_rivi, k_linja, uusi_asema._siirtovuoro) == false)
        {
            // l�ytyi laillinen siirto, lis�t��n siirto-vektoriin
            siirrot.push_back(rs);
        }
    }
}

float Asema::pisteyta_lopputulos(int syvyys) const
{
    int k_rivi, k_linja;
    if (_siirtovuoro == VALKEA) {
        etsi_kuningas(wK, k_rivi, k_linja);
        if (onko_ruutu_uhattu(k_rivi, k_linja, MUSTA))
            return -1000000 - syvyys; 
        else
            return 0;
    }
    else {
        etsi_kuningas(bK, k_rivi, k_linja);
        if (onko_ruutu_uhattu(k_rivi, k_linja, VALKEA))
            return 1000000 + syvyys;
        else
            return 0;
    }
}

