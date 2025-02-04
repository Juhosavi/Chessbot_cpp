#pragma once
#include <string>
#include "shakki.h"
#include <vector>

// Kuvaa aseman muutosta.
class Siirto
{
public:
    // Oletuskonstruktori (ei tee oikeastaan mitään).
    Siirto() {}

    // Alustaa siirron koordinaateista. (a_r, a_l) => (l_r, l_l)
    Siirto(int a_r, int a_l, int l_r, int l_l) :
        _a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l)
    {
    }

    // Konstruktori merkkijonosta, esim. "e2e4"
    Siirto(const std::string& s)
    {
        _a_l = s[0] - 'a';
        _a_r = 7 - (s[1] - '1');
        _l_l = s[2] - 'a';
        _l_r = 7 - (s[3] - '1');
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
            _l_r == other._l_r && _l_l == other._l_l;
    }

  friend class Asema;
};
