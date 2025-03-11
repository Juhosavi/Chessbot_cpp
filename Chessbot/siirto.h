#pragma once
#include <string>
#include "shakki.h"
#include <vector>

/**
 * @class Siirto
 * @brief Kuvaa aseman muutosta eli shakinappulan siirtoa.
 *
 * Tämä luokka sisältää tiedot siirron alku- ja loppukoordinaateista, mahdollisista
 * ohestalyönneistä sekä ylennettävistä nappuloista.
 */
class Siirto
{
public:
    /**
     * @brief Oletuskonstruktori.
     *
     * Ei tee oikeastaan mitään, mutta mahdollistaa Siirto-olion luomisen ilman parametreja.
     */
    Siirto()
    {
    }

    /**
     * @brief Alustaa siirron koordinaateista.
     *
     * Määrittelee siirron alku- ja loppukoordinaatit.
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
     * @brief Alustaa siirron koordinaateista ja ohestalyönti-flagilla.
     *
     * Määrittelee siirron alku- ja loppukoordinaatit sekä ohestalyönti-tilan.
     *
     * @param a_r Siirron alkuruudun rivi (0-7)
     * @param a_l Siirron alkuruudun sarake (0-7)
     * @param l_r Siirron loppuruudun rivi (0-7)
     * @param l_l Siirron loppuruudun sarake (0-7)
     * @param ohestalyonti Ohestalyönti-flag (true, jos siirto on ohestalyönti)
     * @param korotettava_nappula Korotettavan nappulan tyyppi (esim. wQ, bR). Oletusarvo NA.
     */
    Siirto(int a_r, int a_l, int l_r, int l_l, bool ohestalyonti, int korotettava_nappula = NA) :
        _a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l), _ohestalyonti(ohestalyonti), _korotettava_nappula(korotettava_nappula)
    {
    }

    /**
     * @brief Konstruktori merkkijonosta.
     *
     * Alustaa siirron merkkijonosta, kuten "e2e4", ja tarvittaessa käsittelee ylennyksen.
     *
     * @param s Merkkijono, joka kuvaa siirron (esim. "e2e4" tai "e7e8Q").
     */
    Siirto(const std::string& s)
    {
        _a_l = s[0] - 'a';
        _a_r = 7 - (s[1] - '1');
        _l_l = s[2] - 'a';
        _l_r = 7 - (s[3] - '1');

        // Jos syötteessä on viides merkki, se tarkoittaa ylennystä
        if (s.length() == 5) {
            char promo = s[4];
            // Oletetaan, että asetat oikeat vakioarvot riippuen pelaajasta.
            // Tässä esimerkissä oletamme, että ylennettävä nappula on valkoisen nappula.
            // Jos kehität kaksipuolista logiikkaa, sinun tulee tarkistaa pelaajan väri.
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

    /** Ohestalyönti-flag (true, jos siirto on ohestalyönti) */
    bool _ohestalyonti = false;

    /**
     * @brief Operaattori ==, vertailee kahta Siirto-oliota.
     *
     * Palauttaa true, jos molemmat Siirto-oliot ovat samanlaisia (kaikki koordinaatit ja ohestalyönti-tila ovat samat).
     *
     * @param other Vertailtava Siirto-olio.
     * @return true, jos siirrot ovat samanlaisia, muuten false.
     */
    bool operator==(const Siirto& other) const {
        return _a_r == other._a_r && _a_l == other._a_l &&
            _l_r == other._l_r && _l_l == other._l_l &&
            _ohestalyonti == other._ohestalyonti;
    }

    /** Ystävällinen luokka Asema, joka voi käyttää yksityisiä jäseniä */
    friend class Asema;
};
