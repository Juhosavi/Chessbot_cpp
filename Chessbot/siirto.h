#pragma once
#include <string>
#include "shakki.h"
#include <vector>

// Kuvaa aseman muutosta.
class Siirto
{
public:
    // Oletuskonstruktori (ei tee oikeastaan mitään).
    Siirto() 
    {}

    // Alustaa siirron koordinaateista. (a_r, a_l) => (l_r, l_l)
    Siirto(int a_r, int a_l, int l_r, int l_l) :
        _a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l)
    {
    }
    // Alustaa siirron koordinaateista ja ohestalyönti-flagilla. (a_r, a_l) => (l_r, l_l), ohestalyonti
    Siirto(int a_r, int a_l, int l_r, int l_l, bool ohestalyonti, int korotettava_nappula = NA) :
        _a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l), _ohestalyonti(ohestalyonti), _korotettava_nappula(korotettava_nappula) 
    {
    }

    // Konstruktori merkkijonosta, esim. "e2e4"
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
    int _a_r; // Siirron alkuruudun rivi
    int _a_l; // Siirron alkuruudun sarake
    int _l_r; // Siirron loppuruudun rivi
    int _l_l; // Siirron loppuruudun sarake
    int _korotettava_nappula = NA; // Korotusnappula, esim. wQ, bR
    bool _ohestalyonti = false;


    // Operaattori ==
    bool operator==(const Siirto& other) const {
        return _a_r == other._a_r && _a_l == other._a_l &&
            _l_r == other._l_r && _l_l == other._l_l &&
            _ohestalyonti == other._ohestalyonti;
    }

  friend class Asema;
};
