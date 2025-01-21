#pragma once
#include <string>
#include "shakki.h"

// Kuvaa aseman muutosta.
class Siirto
{
public:
    // Oletuskonstruktori (ei tee oikeastaan mitään).
    Siirto() {}

    // Alustaa siirron koordinaateista. (a_r, a_l) => (l_r, l_l)
    Siirto(int a_r, int a_l, int l_r, int l_l) :
        _a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l)
    {}

    // Konstruktori, jonka avulla siirto-olio alustetaan
    // annetusta merkkijonosta, esim. "e2e4" tai "g8f6".
    Siirto(const std::string& s)
    {
        _a_r = s[0] - 'a';
        _a_l = s[1] - '1';
        _l_r = s[2] - 'a';
        _l_l = s[3] - '1';
    }

private:
    int _a_r; // Siirron alkuruudun rivi
    int _a_l; // Siirron alkuruudun sarake
    int _l_r; // Siirron loppuruudun rivi
    int _l_l; // Siirron loppuruudun sarake
    int _korotettava_nappula = NA; // Esim. wQ, wR, bN jne.
    bool _ohestalyonti = false;

    friend class Asema;
};
