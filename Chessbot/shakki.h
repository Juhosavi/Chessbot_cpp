#pragma once

/**
 * @file chess_constants.h
 * @brief M‰‰rittelee vakioarvot ja funktiot, jotka liittyv‰t shakkinappuloihin ja pelaajiin.
 */

 // Pelaajat.
 // @enum {VALKEA, MUSTA} Pelaajan v‰rit (VALKEA tai MUSTA)
enum { VALKEA, MUSTA };

/**
 * @enum Nappula
 * @brief Shakkinappuloiden vakioarvot, jotka kuvaavat peliss‰ k‰ytett‰vien nappuloiden tyyppi‰.
 *
 * @note NA tarkoittaa tyhj‰‰ ruutua.
 */
enum { wR, wN, wB, wQ, wK, wP, bR, bN, bB, bQ, bK, bP, NA };

/**
 * @brief Palauttaa annetun nappulan v‰rin.
 *
 * T‰m‰ funktio tarkistaa, onko nappula valkoinen vai musta.
 *
 * @param nappula Shakkinappula, jonka v‰ri halutaan palauttaa.
 * @return VALKEA, jos nappula on valkoinen, MUUTA, jos nappula on musta.
 */
int nappulan_vari(int nappula);

/**
 * @brief Palauttaa pelaajan vastustajan.
 *
 * T‰m‰ funktio palauttaa annetulle pelaajalle (VALKEA tai MUSTA) vastustajan v‰rin.
 *
 * @param pelaaja Pelaaja, jonka vastustaja halutaan tiet‰‰ (VALKEA tai MUSTA).
 * @return Vastustajan v‰ri (VALKEA tai MUSTA).
 */
int vastustaja(int pelaaja);
