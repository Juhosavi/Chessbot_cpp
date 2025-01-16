#pragma once
#include "shakki.h"


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


	// LÄKSY
	// Tyhjentää laudan.
	void tyhjenna();

	// LÄKSY
	// Tulosta lauta ascii-grafiikkana (mahdollisesti flagit
	// voisi myös tulostaa jotenkin siististi).
	void tulosta() const;

	// LÄKSY
	// etsii annetun kuninkaan (wK tai mK) rivin ja linjan
	void etsi_kuningas(int nappula, int& rivi, int& linja) const;
};
