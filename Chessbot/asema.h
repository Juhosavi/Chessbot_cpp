#pragma once
#include "shakki.h"
#include "siirto.h"
#include <vector>


// Pelin tilan kuvaaminen ja siihen liittyvät operaatiot.
class Asema
{
public:

	// Laudan nappulat. Indeksointi [rivi][linja], esim.
	//
	// [0][0] : vasen ylänurkka ("a8")
	// [7][0] : vasen alanurkka ("a1")
	// [7][7] : oikea alanurkka ("h1")
	//
	int _lauta[8][8] = {
		{ bR, bN, bB, bQ, bK, bB, bN, bR },
		{ bP, bP, bP, bP, bP, bP, bP, bP },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ wP, wP, wP, wP, wP, wP, wP, wP },
		{ wR, wN, wB, wQ, wK, wB, wN, wR }
	};

	// Kumman pelaajan siirtovuoro?
	int _siirtovuoro = VALKEA;

	// Kirjanpito siitä, onko kuningas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestalyöntiin tarvittava info.
	int _kaksoisaskel_linjalla = -1;

	// Tekee annetun siirron laudalla. Voidaan olettaa, että
	// siirto on laillinen.
	void tee_siirto(const Siirto& siirto, int pelaaja);

	void kysy_siirto(int pelaaja, int& lahto_rivi, int& lahto_linja, int& kohde_rivi, int& kohde_linja);

	

	void anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const;

	// LÄKSY
	// Tyhjentää laudan.
	void tyhjenna();

	// LÄKSY
	// Tulosta lauta ascii-grafiikkana (mahdollisesti flagit
	// voisi myös tulostaa jotenkin siististi).
	void tulosta() const;
	bool onko_laillinen_siirto(const Siirto& siirto, int pelaaja) const;

	// LÄKSY
	// etsii annetun kuninkaan (wK tai mK) rivin ja linjan
	void etsi_kuningas(int nappula, int& rivi, int& linja) const;

	bool on_vastustajan_nappula(int ruutu, int pelaaja) const;
};
