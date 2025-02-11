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
    if (onko_laillinen_siirto(siirto, pelaaja) == false)
    {
        std::cout << "Ei laillinen siirto!" << std::endl;
        return; // Palataan eikä tehdä siirtoa
    }

    // Siirto on laillinen, suoritetaan siirto
    int lahto_rivi = siirto._a_r;
    int lahto_linja = siirto._a_l;
    int kohde_rivi = siirto._l_r;
    int kohde_linja = siirto._l_l;

    // Käsitellään ohestalyönti
    if (siirto._ohestalyonti) {
        int vastustajan_rivi = (pelaaja == VALKEA) ? kohde_rivi + 1 : kohde_rivi - 1;
        _lauta[vastustajan_rivi][kohde_linja] = NA;
    }

    // Päivitetään kaksoisaskeleen linja
    if (abs(lahto_rivi - kohde_rivi) == 2 && (_lauta[lahto_rivi][lahto_linja] == wP || _lauta[lahto_rivi][lahto_linja] == bP)) {
        _kaksoisaskel_linjalla = kohde_linja;
    }
    else {
        _kaksoisaskel_linjalla = -1;
    }

    _lauta[kohde_rivi][kohde_linja] = _lauta[lahto_rivi][lahto_linja];
    _lauta[lahto_rivi][lahto_linja] = NA;

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

        // Muodostetaan Siirto-olio syötteestä
        Siirto siirto(syote);

        // Tarkistetaan, että siirto on laillinen
        if (onko_laillinen_siirto(siirto, pelaaja))
        {
            // Puretaan siirron tiedot
            lahto_rivi = siirto._a_r;
            lahto_linja = siirto._a_l;
            kohde_rivi = siirto._l_r;
            kohde_linja = siirto._l_l;
            break; // Hyväksytään siirto ja poistutaan silmukasta
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

    // Varmistetaan, että nappula kuuluu pelaajalle
    if ((pelaaja == VALKEA && !(nappula >= wR && nappula <= wP)) ||
        (pelaaja == MUSTA && !(nappula >= bR && nappula <= bP)))
    {
        return false; // Lähtöruudussa ei ole pelaajan nappulaa
    }

    // Lisätään raakasiirrot siirtojen joukkoon nappulan mukaan
    if (nappula == wR || nappula == bR) // Torni
    {
        anna_tornin_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    }
    if (nappula == wB || nappula == bB) //lähetti
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
    // Lisää muita nappulatyyppejä myöhemmin...

    // Tarkistetaan, onko siirto sallittujen siirtojen joukossa
    for (const auto& s : sallitut_siirrot)
    {
        if (s._a_r == siirto._a_r && s._a_l == siirto._a_l &&
            s._l_r == siirto._l_r && s._l_l == siirto._l_l)
        {
            return true; // Siirto löytyi sallitujen siirtojen joukosta
        }
    }

    return false; // Siirto ei ole laillinen
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
void Asema::anna_lahetti_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    //VASEMMALLE YLÖS
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
    //OIKEALLE YLÖS
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
        {-2, -1}, // Ratsun siirto 2 ruutua ylös ja 1 ruutu vasemmalle
        {-1, -2}, // Ratsun siirto 1 ruutu ylös ja 2 ruutua vasemmalle
        {1, -2},  // Ratsun siirto 1 ruutu alas ja 2 ruutua vasemmalle
        {2, -1},  // Ratsun siirto 2 ruutua alas ja 1 ruutu vasemmalle
        {2, 1},   // Ratsun siirto 2 ruutua alas ja 1 ruutu oikealle
        {1, 2},   // Ratsun siirto 1 ruutu alas ja 2 ruutua oikealle
        {-1, 2},  // Ratsun siirto 1 ruutu ylös ja 2 ruutua oikealle
        {-2, 1}   // Ratsun siirto 2 ruutua ylös ja 1 ruutu oikealle
    };

    // Käydään läpi kaikki mahdolliset ratsun siirrot
    for (int i = 0; i < 8; ++i)
    {
        // Lasketaan uusi rivi ja linja ratsun siirron perusteella
        int uusi_rivi = rivi + ratsun_siirrot[i][0];
        int uusi_linja = linja + ratsun_siirrot[i][1];

        // Tarkistetaan, että uusi rivi ja linja ovat shakkilaudan sisällä
        if (uusi_rivi >= 0 && uusi_rivi < 8 && uusi_linja >= 0 && uusi_linja < 8)
        {
            // Tarkistetaan, että uusi ruutu on tyhjä tai siinä on vastustajan nappula
            if (_lauta[uusi_rivi][uusi_linja] == NA || on_vastustajan_nappula(_lauta[uusi_rivi][uusi_linja], pelaaja))
            {
                // Lisätään siirto mahdollisiin siirtoihin
                siirrot.push_back(Siirto(rivi, linja, uusi_rivi, uusi_linja));
            }
            
        }
    }
}
void Asema::anna_daami_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    // Daami voi liikkua kuin torni
    anna_tornin_raakasiirrot(rivi, linja, pelaaja, siirrot);

    // Daami voi liikkua kuin lähetti
    anna_lahetti_raakasiirrot(rivi, linja, pelaaja, siirrot);
}
void Asema::anna_kuningas_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    const int kuningas_siirrot[8][2] =
    {
        {-1, -1}, // 1 ruutu ylös ja 1 ruutu vasemmalle
        {-1,  0}, // 1 ruutu ylös
        {-1,  1}, // 1 ruutu ylös ja 1 ruutu oikealle
        { 0, -1}, // 1 ruutu vasemmalle
        { 0,  1}, // 1 ruutu oikealle
        { 1, -1}, // 1 ruutu alas ja 1 ruutu vasemmalle
        { 1,  0}, // 1 ruutu alas
        { 1,  1}  // 1 ruutu alas ja 1 ruutu oikealle
    };
    // Käydään läpi kaikki mahdolliset Kuninkaan siirrot
    for (int i = 0; i < 8; ++i)
    {
        // Lasketaan uusi rivi ja linja ratsun siirron perusteella
        int uusi_rivi = rivi + kuningas_siirrot[i][0];
        int uusi_linja = linja + kuningas_siirrot[i][1];

        // Tarkistetaan, että uusi rivi ja linja ovat shakkilaudan sisällä
        if (uusi_rivi >= 0 && uusi_rivi < 8 && uusi_linja >= 0 && uusi_linja < 8)
        {
            // Tarkistetaan, että uusi ruutu on tyhjä tai siinä on vastustajan nappula
            if (_lauta[uusi_rivi][uusi_linja] == NA || on_vastustajan_nappula(_lauta[uusi_rivi][uusi_linja], pelaaja))
            {
                // Lisätään siirto mahdollisiin siirtoihin
                siirrot.push_back(Siirto(rivi, linja, uusi_rivi, uusi_linja));
            }

        }
    }
}
void Asema::anna_sotilas_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const
{
    int suunta;
    if (pelaaja == VALKEA) 
    {
        suunta = -1; // Valkoinen liikkuu ylöspäin
    }
    else 
    {
        suunta = 1; // Musta liikkuu alaspäin
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

    // Yksi askel eteenpäin
    if (_lauta[rivi + suunta][linja] == NA)
    {
        siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja));

        // Kaksi askelta eteenpäin aloitusriviltä
        if (rivi == aloitusrivi && _lauta[rivi + 2 * suunta][linja] == NA)
        {
            siirrot.push_back(Siirto(rivi, linja, rivi + 2 * suunta, linja));
        }
    }

    // Lyönti kulmittain vasemmalle
    if (linja > 0 && on_vastustajan_nappula(_lauta[rivi + suunta][linja - 1], pelaaja))
    {
        siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1));
    }

    // Lyönti kulmittain oikealle
    if (linja < 8 && on_vastustajan_nappula(_lauta[rivi + suunta][linja + 1], pelaaja))
    {
        siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1));
    }
    // Ohestalyönti
    if (_kaksoisaskel_linjalla != -1 && (linja == _kaksoisaskel_linjalla - 1 || linja == _kaksoisaskel_linjalla + 1)) {
        if ((pelaaja == VALKEA && rivi == 3) || (pelaaja == MUSTA && rivi == 4)) {
            int ohestalyonti_rivi = (pelaaja == VALKEA) ? 2 : 5;
            siirrot.push_back(Siirto(rivi, linja, ohestalyonti_rivi, _kaksoisaskel_linjalla, true));
        }
    }
}


void Asema::anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const 
{
    // käy läpi kaikki ruudut
    for (int rivi = 0; rivi < 8; rivi++)
        for (int sarake = 0; sarake < 8; sarake++)
        {
            int nappula = _lauta[rivi][sarake];

            if (_siirtovuoro == VALKEA && nappula == wR){
				anna_tornin_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (_siirtovuoro == VALKEA && nappula == wN) {
                anna_ratsu_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (_siirtovuoro == VALKEA && nappula == wB) 
            {
                anna_lahetti_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (_siirtovuoro == VALKEA && nappula == wQ) {
                anna_daami_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (_siirtovuoro == VALKEA && nappula == wK) {
                anna_kuningas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (_siirtovuoro == VALKEA && nappula == wP) {
                anna_sotilas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            }
            else if (_siirtovuoro == MUSTA && nappula == bR) {
                anna_tornin_raakasiirrot(rivi, sarake, pelaaja, siirrot);
               
            }
            else if (_siirtovuoro == MUSTA && nappula == bN) {
                anna_ratsu_raakasiirrot(rivi, sarake, pelaaja, siirrot);
                
            }
            else if (_siirtovuoro == MUSTA && nappula == bB) {
                anna_lahetti_raakasiirrot(rivi, sarake, pelaaja, siirrot);
               
            }
            else if (_siirtovuoro == MUSTA && nappula == bQ) {
                anna_daami_raakasiirrot(rivi, sarake, pelaaja, siirrot);
                
            }
            else if (_siirtovuoro == MUSTA && nappula == bK) {
                anna_kuningas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
               
            }
            else if (_siirtovuoro == MUSTA && nappula == bP) {
                anna_sotilas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
                
            }
        }

}

bool Asema::onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaja) const
{
    //tarkistaa onko ruutu uhattu ja osuuko joku tähän ruutuun
    std::vector<Siirto> raakasiirrot;
    anna_kaikki_raakasiirrot(uhkaava_pelaaja, raakasiirrot);
    for (auto& rs : raakasiirrot)
	{
		if (rs._l_r == rivi && rs._l_l == linja)
		{
			return true;
            std::cout << "UHATTU!";
		}
	}
    return false;
    std::cout << "EI UHATTU";
}

void Asema::anna_siirrot(std::vector<Siirto>& siirrot) const
{
    std::vector<Siirto> raakasiirrot;


    // generoidaan raakasiirrot, ja testataan ne yksi kerrallaan. 
    anna_kaikki_raakasiirrot(_siirtovuoro, raakasiirrot);
    for (auto& rs : raakasiirrot)
    {
        //tehdään siirto
        Asema uusi_asema = *this;
        uusi_asema.tee_siirto(rs, _siirtovuoro);

        // etsitään kuningas
        int k_rivi, k_linja;
        uusi_asema.etsi_kuningas((_siirtovuoro == VALKEA) ? wK : bK, k_rivi, k_linja);

        //tarkistetaan onko kuningas uhattu
        if (uusi_asema.onko_ruutu_uhattu(k_rivi, k_linja, uusi_asema._siirtovuoro) == false)
        {
            // löytyi laillinen siirto, lisätään siirto-vektoriin
            siirrot.push_back(rs);
        }
    }
}