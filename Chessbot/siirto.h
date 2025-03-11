#pragma once
#include <string>
#include "shakki.h"
#include <vector>

/**
 * @class Siirto
 * @brief Kuvaa aseman muutosta eli shakinappulan siirtoa.
 *
 * T�m� luokka sis�lt�� tiedot siirron alku- ja loppukoordinaateista, mahdollisista
 * ohestaly�nneist� sek� ylennett�vist� nappuloista.
 */
class Siirto
{
public:
    /**
     * @brief Oletuskonstruktori.
     *
     * Ei tee oikeastaan mit��n, mutta mahdollistaa Siirto-olion luomisen ilman parametreja.
     */
    Siirto()
    {
    }

    /**
     * @brief Alustaa siirron koordinaateista.
     *
     * M��rittelee siirron alku- ja loppukoordinaatit.
     *
     * @param a_r Siirron alkuruudun rivi (0-7)
     * @param a_l Siirron alkuruudun sarake (0-7)
     * @param l_r Siirron loppuruudun rivi (0-7)
     * @param l_l Siirron loppuruudun sarake (0-7)
     */
    Siirto(int a_r, int a_l, int l_r, int l_l) :
        _a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l)
    {
    }

    /**
     * @brief Alustaa siirron koordinaateista ja ohestaly�nti-flagilla.
     *
     * M��rittelee siirron alku- ja loppukoordinaatit sek� ohestaly�nti-tilan.
     *
     * @param a_r Siirron alkuruudun rivi (0-7)
     * @param a_l Siirron alkuruudun sarake (0-7)
     * @param l_r Siirron loppuruudun rivi (0-7)
     * @param l_l Siirron loppuruudun sarake (0-7)
     * @param ohestalyonti Ohestaly�nti-flag (true, jos siirto on ohestaly�nti)
     * @param korotettava_nappula Korotettavan nappulan tyyppi (esim. wQ, bR). Oletusarvo NA.
     */
    Siirto(int a_r, int a_l, int l_r, int l_l, bool ohestalyonti, int korotettava_nappula = NA) :
        _a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l), _ohestalyonti(ohestalyonti), _korotettava_nappula(korotettava_nappula)
    {
    }

    /**
     * @brief Konstruktori merkkijonosta.
     *
     * Alustaa siirron merkkijonosta, kuten "e2e4", ja tarvittaessa k�sittelee ylennyksen.
     *
     * @param s Merkkijono, joka kuvaa siirron (esim. "e2e4" tai "e7e8Q").
     */
    Siirto(const std::string& s)
    {
        _a_l = s[0] - 'a';
        _a_r = 7 - (s[1] - '1');
        _l_l = s[2] - 'a';
        _l_r = 7 - (s[3] - '1');

        // Jos sy�tteess� on viides merkki, se tarkoittaa ylennyst�
        if (s.length() == 5) {
            char promo = s[4];
            // Oletetaan, ett� asetat oikeat vakioarvot riippuen pelaajasta.
            // T�ss� esimerkiss� oletamme, ett� ylennett�v� nappula on valkoisen nappula.
            // Jos kehit�t kaksipuolista logiikkaa, sinun tulee tarkistaa pelaajan v�ri.
            switch (promo) {
            case 'Q': _korotettava_nappula = wQ; break;
            case 'R': _korotettava_nappula = wR; break;
            case 'B': _korotettava_nappula = wB; break;
            case 'N': _korotettava_nappula = wN; break;
            default:  _korotettava_nappula = wQ; break; // oletuksena kuningatar
            }
        }
    }

    // Julkiset attribuutit

    /** Siirron alkuruudun rivi (0-7) */
    int _a_r;

    /** Siirron alkuruudun sarake (0-7) */
    int _a_l;

    /** Siirron loppuruudun rivi (0-7) */
    int _l_r;

    /** Siirron loppuruudun sarake (0-7) */
    int _l_l;

    /** Korotusnappula, esim. wQ, bR, NA, jos ei ole korotusta */
    int _korotettava_nappula = NA;

    /** Ohestaly�nti-flag (true, jos siirto on ohestaly�nti) */
    bool _ohestalyonti = false;

    /**
     * @brief Operaattori ==, vertailee kahta Siirto-oliota.
     *
     * Palauttaa true, jos molemmat Siirto-oliot ovat samanlaisia (kaikki koordinaatit ja ohestaly�nti-tila ovat samat).
     *
     * @param other Vertailtava Siirto-olio.
     * @return true, jos siirrot ovat samanlaisia, muuten false.
     */
    bool operator==(const Siirto& other) const {
        return _a_r == other._a_r && _a_l == other._a_l &&
            _l_r == other._l_r && _l_l == other._l_l &&
            _ohestalyonti == other._ohestalyonti;
    }

    /** Yst�v�llinen luokka Asema, joka voi k�ytt�� yksityisi� j�seni� */
    friend class Asema;
};
