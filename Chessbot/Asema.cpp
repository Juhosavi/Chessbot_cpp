#include "asema.h"
#include <iostream>
#include "siirto.h"
#include <vector>

void Asema::tulosta() const {
    for (int rivi = 0; rivi < 8; rivi++) {
        std::cout << "  ";
        for (int linja = 0; linja < 8; linja++) {
            std::cout << "+---";
        }
        std::cout << "+" << std::endl;
        std::cout << 8 - rivi << " ";
        for (int linja = 0; linja < 8; linja++) {
            std::cout << "|";
            int nappula = _lauta[rivi][linja];
            switch (nappula) {
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
        std::cout << "|" << std::endl;
    }
    std::cout << "  ";
    for (int h = 0; h < 8; h++) {
        std::cout << "+---";
    }
    std::cout << "+" << std::endl;
    std::cout << "    a   b   c   d   e   f   g   h" << std::endl;
}

void Asema::tyhjenna() {
    std::cout << "\033[2J\033[H";
    for (int rivi = 0; rivi < 8; rivi++) {
        for (int linja = 0; linja < 8; linja++) {
            _lauta[rivi][linja] = NA;
        }
    }
}

void Asema::etsi_kuningas(int nappula, int& rivi, int& linja) const {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (_lauta[i][j] == nappula) {
                rivi = i;
                linja = j;
            }
        }
    }
}

void Asema::tee_siirto(const Siirto& siirto, int pelaaja) {
    int lahto_rivi = siirto._a_r;
    int lahto_linja = siirto._a_l;
    int kohde_rivi = siirto._l_r;
    int kohde_linja = siirto._l_l;

    if (siirto._ohestalyonti) {
        int vastustajan_rivi = (pelaaja == VALKEA) ? kohde_rivi + 1 : kohde_rivi - 1;
        _lauta[vastustajan_rivi][kohde_linja] = NA;
    }
    if (abs(lahto_rivi - kohde_rivi) == 2 &&
        (_lauta[lahto_rivi][lahto_linja] == wP || _lauta[lahto_rivi][lahto_linja] == bP)) {
        _kaksoisaskel_linjalla = kohde_linja;
    }
    else {
        _kaksoisaskel_linjalla = -1;
    }
    if (_lauta[lahto_rivi][lahto_linja] == wK || _lauta[lahto_rivi][lahto_linja] == bK) {
        if (pelaaja == VALKEA) {
            if (lahto_linja == 4 && kohde_linja == 6 && _valkea_lyhyt_linna_sallittu) {
                _lauta[7][5] = wR;
                _lauta[7][7] = NA;
            }
            else if (lahto_linja == 4 && kohde_linja == 2 && _valkea_pitka_linna_sallittu) {
                _lauta[7][3] = wR;
                _lauta[7][0] = NA;
            }
            _valkea_lyhyt_linna_sallittu = false;
            _valkea_pitka_linna_sallittu = false;
        }
        else if (pelaaja == MUSTA) {
            if (lahto_linja == 4 && kohde_linja == 6 && _musta_lyhyt_linna_sallittu) {
                _lauta[0][5] = bR;
                _lauta[0][7] = NA;
            }
            else if (lahto_linja == 4 && kohde_linja == 2 && _musta_pitka_linna_sallittu) {
                _lauta[0][3] = bR;
                _lauta[0][0] = NA;
            }
            _musta_lyhyt_linna_sallittu = false;
            _musta_pitka_linna_sallittu = false;
        }
    }
    if (_lauta[lahto_rivi][lahto_linja] == wR) {
        if (lahto_rivi == 7 && lahto_linja == 0) _valkea_pitka_linna_sallittu = false;
        if (lahto_rivi == 7 && lahto_linja == 7) _valkea_lyhyt_linna_sallittu = false;
    }
    if (_lauta[lahto_rivi][lahto_linja] == bR) {
        if (lahto_rivi == 0 && lahto_linja == 0) _musta_pitka_linna_sallittu = false;
        if (lahto_rivi == 0 && lahto_linja == 7) _musta_lyhyt_linna_sallittu = false;
    }
    _lauta[kohde_rivi][kohde_linja] = _lauta[lahto_rivi][lahto_linja];
    _lauta[lahto_rivi][lahto_linja] = NA;
    if (siirto._korotettava_nappula != NA) {
        _lauta[lahto_rivi][lahto_linja] = NA;
        _lauta[kohde_rivi][kohde_linja] = siirto._korotettava_nappula;
    }
    _siirtovuoro = (_siirtovuoro == VALKEA) ? MUSTA : VALKEA;
}

void Asema::kysy_siirto(int pelaaja, int& lahto_rivi, int& lahto_linja, int& kohde_rivi, int& kohde_linja) {
    std::string syote;
    while (true) {
        std::cout << "Pelaaja " << pelaaja << ", ANNA SIIRTO:  ";
        std::cin >> syote;
        Siirto siirto(syote);
        if (onko_laillinen_siirto(siirto, pelaaja)) {
            lahto_rivi = siirto._a_r;
            lahto_linja = siirto._a_l;
            kohde_rivi = siirto._l_r;
            kohde_linja = siirto._l_l;
            break;
        }
        else {
            std::cout << "Ei laillinen siirto! Yritä uudelleen." << std::endl;
        }
    }
}

bool Asema::on_vastustajan_nappula(int ruutu, int pelaaja) const {
    return (pelaaja == VALKEA && ruutu >= bR && ruutu <= bP) ||
        (pelaaja == MUSTA && ruutu >= wR && ruutu <= wP);
}

bool Asema::onko_laillinen_siirto(const Siirto& siirto, int pelaaja) const {
    vector<Siirto> sallitut_siirrot;
    sallitut_siirrot.reserve(100);
    int rivi = siirto._a_r;
    int linja = siirto._a_l;
    int nappula = _lauta[rivi][linja];

    if (pelaaja == VALKEA) {
        if (siirto._a_r == 7 && siirto._a_l == 4) {
            if (siirto._l_l == 6 && _valkea_lyhyt_linna_sallittu)
                return true;
            else if (siirto._l_l == 2 && _valkea_pitka_linna_sallittu)
                return true;
        }
    }
    else if (pelaaja == MUSTA) {
        if (siirto._a_r == 0 && siirto._a_l == 4) {
            if (siirto._l_l == 6 && _musta_lyhyt_linna_sallittu)
                return true;
            else if (siirto._l_l == 2 && _musta_pitka_linna_sallittu)
                return true;
        }
    }
    if ((pelaaja == VALKEA && !(nappula >= wR && nappula <= wP)) ||
        (pelaaja == MUSTA && !(nappula >= bR && nappula <= bP)))
        return false;

    if (nappula == wR || nappula == bR)
        anna_tornin_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    if (nappula == wB || nappula == bB)
        anna_lahetti_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    if (nappula == wN || nappula == bN)
        anna_ratsu_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    if (nappula == wQ || nappula == bQ)
        anna_daami_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    if (nappula == wK || nappula == bK)
        anna_kuningas_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);
    if (nappula == wP || nappula == bP)
        anna_sotilas_raakasiirrot(rivi, linja, pelaaja, sallitut_siirrot);

    for (const auto& s : sallitut_siirrot) {
        if (s._a_r == siirto._a_r && s._a_l == siirto._a_l &&
            s._l_r == siirto._l_r && s._l_l == siirto._l_l)
            return true;
    }
    return false;
}

void Asema::anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
    int nykyinen_rivi = rivi - 1;
    while (nykyinen_rivi >= 0) {
        if (_lauta[nykyinen_rivi][linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, linja));
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][linja], pelaaja)) {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, linja));
            break;
        }
        else break;
        nykyinen_rivi--;
    }
    nykyinen_rivi = rivi + 1;
    while (nykyinen_rivi <= 7) {
        if (_lauta[nykyinen_rivi][linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, linja));
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][linja], pelaaja)) {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, linja));
            break;
        }
        else break;
        nykyinen_rivi++;
    }
    int nykyinen_linja = linja - 1;
    while (nykyinen_linja >= 0) {
        if (_lauta[rivi][nykyinen_linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
        else if (on_vastustajan_nappula(_lauta[rivi][nykyinen_linja], pelaaja)) {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
            break;
        }
        else break;
        nykyinen_linja--;
    }
    nykyinen_linja = linja + 1;
    while (nykyinen_linja <= 7) {
        if (_lauta[rivi][nykyinen_linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
        else if (on_vastustajan_nappula(_lauta[rivi][nykyinen_linja], pelaaja)) {
            siirrot.push_back(Siirto(rivi, linja, rivi, nykyinen_linja));
            break;
        }
        else break;
        nykyinen_linja++;
    }
}

void Asema::anna_lahetti_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
    int nykyinen_rivi = rivi - 1, nykyinen_linja = linja - 1;
    while (nykyinen_rivi >= 0 && nykyinen_linja >= 0) {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][nykyinen_linja], pelaaja)) {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
            break;
        }
        else break;
        nykyinen_rivi--; nykyinen_linja--;
    }
    nykyinen_rivi = rivi + 1; nykyinen_linja = linja + 1;
    while (nykyinen_rivi <= 7 && nykyinen_linja <= 7) {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][nykyinen_linja], pelaaja)) {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
            break;
        }
        else break;
        nykyinen_rivi++; nykyinen_linja++;
    }
    nykyinen_rivi = rivi + 1; nykyinen_linja = linja - 1;
    while (nykyinen_rivi <= 7 && nykyinen_linja >= 0) {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][nykyinen_linja], pelaaja)) {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
            break;
        }
        else break;
        nykyinen_rivi++; nykyinen_linja--;
    }
    nykyinen_rivi = rivi - 1; nykyinen_linja = linja + 1;
    while (nykyinen_rivi >= 0 && nykyinen_linja <= 7) {
        if (_lauta[nykyinen_rivi][nykyinen_linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
        else if (on_vastustajan_nappula(_lauta[nykyinen_rivi][nykyinen_linja], pelaaja)) {
            siirrot.push_back(Siirto(rivi, linja, nykyinen_rivi, nykyinen_linja));
            break;
        }
        else break;
        nykyinen_rivi--; nykyinen_linja++;
    }
}

void Asema::anna_ratsu_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
    const int ratsun_siirrot[8][2] = {
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1},
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1}
    };
    for (int i = 0; i < 8; ++i) {
        int uusi_rivi = rivi + ratsun_siirrot[i][0];
        int uusi_linja = linja + ratsun_siirrot[i][1];
        if (uusi_rivi >= 0 && uusi_rivi < 8 && uusi_linja >= 0 && uusi_linja < 8) {
            if (_lauta[uusi_rivi][uusi_linja] == NA ||
                on_vastustajan_nappula(_lauta[uusi_rivi][uusi_linja], pelaaja))
                siirrot.push_back(Siirto(rivi, linja, uusi_rivi, uusi_linja));
        }
    }
}

void Asema::anna_daami_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
    anna_tornin_raakasiirrot(rivi, linja, pelaaja, siirrot);
    anna_lahetti_raakasiirrot(rivi, linja, pelaaja, siirrot);
}

void Asema::anna_kuningas_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
    const int kuningas_siirrot[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    for (int i = 0; i < 8; ++i) {
        int uusi_rivi = rivi + kuningas_siirrot[i][0];
        int uusi_linja = linja + kuningas_siirrot[i][1];
        if (uusi_rivi >= 0 && uusi_rivi < 8 && uusi_linja >= 0 && uusi_linja < 8)
            if (_lauta[uusi_rivi][uusi_linja] == NA ||
                on_vastustajan_nappula(_lauta[uusi_rivi][uusi_linja], pelaaja))
                siirrot.push_back(Siirto(rivi, linja, uusi_rivi, uusi_linja));
    }
}

void Asema::hae_linnoitukset(int pelaaja, int rivi, int linja, vector<Siirto>& siirrot) const {
    if (pelaaja == VALKEA && rivi == 7 && linja == 4) {
        if (_valkea_lyhyt_linna_sallittu && _lauta[7][5] == NA &&
            !onko_ruutu_uhattu(7, 4, MUSTA) && !onko_ruutu_uhattu(7, 5, MUSTA))
            siirrot.push_back(Siirto(7, 4, 7, 6));
        if (_valkea_pitka_linna_sallittu && _lauta[7][3] == NA &&
            !onko_ruutu_uhattu(7, 4, MUSTA) && !onko_ruutu_uhattu(7, 3, MUSTA))
            siirrot.push_back(Siirto(7, 4, 7, 2));
    }
    else if (pelaaja == MUSTA && rivi == 0 && linja == 4) {
        if (_musta_lyhyt_linna_sallittu && _lauta[0][5] == NA &&
            !onko_ruutu_uhattu(0, 4, VALKEA) && !onko_ruutu_uhattu(0, 5, VALKEA))
            siirrot.push_back(Siirto(0, 4, 0, 6));
        if (_musta_pitka_linna_sallittu && _lauta[0][3] == NA &&
            !onko_ruutu_uhattu(0, 4, VALKEA) && !onko_ruutu_uhattu(0, 3, VALKEA))
            siirrot.push_back(Siirto(0, 4, 0, 2));
    }
}

void Asema::anna_sotilas_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
    int suunta = (pelaaja == VALKEA) ? -1 : 1;
    int aloitusrivi = (pelaaja == VALKEA) ? 6 : 1;
    int ylennysrivi = (pelaaja == VALKEA) ? 0 : 7;
    if (_lauta[rivi + suunta][linja] == NA) {
        if (rivi + suunta == ylennysrivi) {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, false, (pelaaja == VALKEA ? wQ : bQ)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, false, (pelaaja == VALKEA ? wR : bR)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, false, (pelaaja == VALKEA ? wB : bB)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, false, (pelaaja == VALKEA ? wN : bN)));
        }
        else {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja));
        }
        if (rivi == aloitusrivi && _lauta[rivi + 2 * suunta][linja] == NA)
            siirrot.push_back(Siirto(rivi, linja, rivi + 2 * suunta, linja));
    }
    if (linja > 0 && on_vastustajan_nappula(_lauta[rivi + suunta][linja - 1], pelaaja)) {
        if (rivi + suunta == ylennysrivi) {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, false, (pelaaja == VALKEA ? wQ : bQ)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, false, (pelaaja == VALKEA ? wR : bR)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, false, (pelaaja == VALKEA ? wB : bB)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, false, (pelaaja == VALKEA ? wN : bN)));
        }
        else {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1));
        }
    }
    if (linja < 7 && on_vastustajan_nappula(_lauta[rivi + suunta][linja + 1], pelaaja)) {
        if (rivi + suunta == ylennysrivi) {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, false, (pelaaja == VALKEA ? wQ : bQ)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, false, (pelaaja == VALKEA ? wR : bR)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, false, (pelaaja == VALKEA ? wB : bB)));
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, false, (pelaaja == VALKEA ? wN : bN)));
        }
        else {
            siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1));
        }
    }
    if (_kaksoisaskel_linjalla != -1 && (linja == _kaksoisaskel_linjalla - 1 || linja == _kaksoisaskel_linjalla + 1)) {
        if ((pelaaja == VALKEA && rivi == 3) || (pelaaja == MUSTA && rivi == 4)) {
            int ohestalyonti_rivi = (pelaaja == VALKEA) ? 2 : 5;
            siirrot.push_back(Siirto(rivi, linja, ohestalyonti_rivi, _kaksoisaskel_linjalla, true));
        }
    }
}

void Asema::anna_kaikki_raakasiirrot(int pelaaja, vector<Siirto>& siirrot) const {
    for (int rivi = 0; rivi < 8; rivi++) {
        for (int sarake = 0; sarake < 8; sarake++) {
            int nappula = _lauta[rivi][sarake];
            if ((_siirtovuoro == VALKEA && nappula == wK) ||
                (_siirtovuoro == MUSTA && nappula == bK)) {
                hae_linnoitukset(pelaaja, rivi, sarake, siirrot);
            }
            if (pelaaja == VALKEA && nappula == wR)
                anna_tornin_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == VALKEA && nappula == wN)
                anna_ratsu_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == VALKEA && nappula == wB)
                anna_lahetti_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == VALKEA && nappula == wQ)
                anna_daami_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == VALKEA && nappula == wK)
                anna_kuningas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == VALKEA && nappula == wP)
                anna_sotilas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == MUSTA && nappula == bR)
                anna_tornin_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == MUSTA && nappula == bN)
                anna_ratsu_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == MUSTA && nappula == bB)
                anna_lahetti_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == MUSTA && nappula == bQ)
                anna_daami_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == MUSTA && nappula == bK)
                anna_kuningas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
            else if (pelaaja == MUSTA && nappula == bP)
                anna_sotilas_raakasiirrot(rivi, sarake, pelaaja, siirrot);
        }
    }
}

bool Asema::onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaja) const {
    vector<Siirto> raakasiirrot;
    raakasiirrot.reserve(100);
    anna_kaikki_raakasiirrot(uhkaava_pelaaja, raakasiirrot);
    for (const auto& rs : raakasiirrot)
        if (rs._l_r == rivi && rs._l_l == linja)
            return true;
    return false;
}

void Asema::anna_siirrot(vector<Siirto>& siirrot) const {
    vector<Siirto> raakasiirrot;
    raakasiirrot.reserve(100);
    anna_kaikki_raakasiirrot(_siirtovuoro, raakasiirrot);
    for (auto& rs : raakasiirrot) {
        Asema uusi_asema = *this;
        uusi_asema.tee_siirto(rs, _siirtovuoro);
        int k_rivi, k_linja;
        uusi_asema.etsi_kuningas((_siirtovuoro == VALKEA) ? wK : bK, k_rivi, k_linja);
        if (!uusi_asema.onko_ruutu_uhattu(k_rivi, k_linja, uusi_asema._siirtovuoro))
            siirrot.push_back(rs);
    }
}

float Asema::pisteyta_lopputulos(int syvyys) const {
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
