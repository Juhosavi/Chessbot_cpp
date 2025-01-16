#pragma once
#include "shakki.h"


// Pelin tilan kuvaaminen ja siihen liittyv�t operaatiot.
class Asema
{
public:

	// Laudan nappulat. Indeksointi [rivi][linja], esim.
	//
	// [0][0] : vasen yl�nurkka ("a8")
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

	// Kirjanpito siit�, onko kuningas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestaly�ntiin tarvittava info.
	int _kaksoisaskel_linjalla = -1;


	// L�KSY
	// Tyhjent�� laudan.
	void tyhjenna();

	// L�KSY
	// Tulosta lauta ascii-grafiikkana (mahdollisesti flagit
	// voisi my�s tulostaa jotenkin siististi).
	void tulosta() const;

	// L�KSY
	// etsii annetun kuninkaan (wK tai mK) rivin ja linjan
	void etsi_kuningas(int nappula, int& rivi, int& linja) const;
};
