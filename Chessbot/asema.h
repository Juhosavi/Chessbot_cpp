#pragma once
#include "shakki.h"
#include "siirto.h"
#include <vector>
#include <iostream>
#include <limits>
#include <vector>
#include <map>

using namespace std;

class MinimaxArvo
{
public:
	MinimaxArvo(float arvo, Siirto siirto) :
		_arvo(arvo), _siirto(siirto)
	{}

	float	_arvo;
	Siirto	_siirto;
};


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
	void anna_lahetti_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const;
	void anna_ratsu_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const;
	void anna_sotilas_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const;
	void anna_daami_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const;
	void anna_kuningas_raakasiirrot(int rivi, int linja, int pelaaja, std::vector<Siirto>& siirrot) const;
	void anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot)const;
	void anna_siirrot(std::vector<Siirto>& siirrot) const;
	bool onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaja) const;
	void tulosta() const;
	void hae_linnoitukset(int pelaaja, int rivi, int linja, std::vector<Siirto>& siirrot) const;
	void etsi_kuningas(int nappula, int& rivi, int& linja) const;
	bool on_vastustajan_nappula(int ruutu, int pelaaja) const;
	bool onko_laillinen_siirto(const Siirto& siirto, int pelaaja) const;
	float pisteyta_lopputulos() const;
	void tyhjenna();
	bool undo();
//------------------------UUSI KOODI------------------------

	MinimaxArvo minimax(int syvyys)
	{
		// Generoidaan aseman siirrot.
		vector<Siirto> siirrot;
		anna_siirrot(siirrot);

		if (siirrot.size() == 0)
		{
			// Rekursion kantatapaus 1:
			// peli on päättynyt (ei yhtään laillista siirtoa).
			return MinimaxArvo(pisteyta_lopputulos(), Siirto());
		}

		if (syvyys == 0)
		{
			// Rekursion kantatapaus 2:
			// ollaan katkaisusyvyydessä.
			return MinimaxArvo(evaluoi(), Siirto());
		}

		// Siirtoja on jäljellä ja ei olla katkaisusyvyydessä,
		// joten kokeillaan yksitellen mahdollisia siirtoja,
		// ja kutsutaan minimax:a kullekin seuraaja-asemalle.
		// Otetaan paras minimax-arvo talteen (alustetaan
		// paras_arvo mahdollisimman huonoksi siirtovuoroisen
		// pelaajan kannalta).
		float paras_arvo = _siirtovuoro == VALKEA ?
			numeric_limits<float>::lowest() : numeric_limits<float>::max();
		Siirto paras_siirto;
		for (Siirto& s : siirrot)
		{
			Asema uusi = *this;
			uusi.tee_siirto(s, _siirtovuoro);

			// Rekursioasekel: kutsutaan minimax:ia seuraaja-asemalle.
			MinimaxArvo arvo = uusi.minimax(syvyys - 1);

			// Jos saatiin paras arvo, otetaan se talteen.
			if (_siirtovuoro == VALKEA && arvo._arvo > paras_arvo)
			{
				paras_arvo = arvo._arvo;
				paras_siirto = s;
			}
			else if (_siirtovuoro == MUSTA && arvo._arvo < paras_arvo)
			{
				paras_arvo = arvo._arvo;
				paras_siirto = s;
			}
		}

		// Palautetaan paras arvo.
		return MinimaxArvo(paras_arvo, paras_siirto);
	}



    //PSQT Taulukot
	const int pawnPSQT[64] = {
		  0,   0,   0,   0,   0,   0,   0,   0,
		 50,  50,  50,  50,  50,  50,  50,  50,
		 10,  10,  20,  30,  30,  20,  10,  10,
		  5,   5,  10,  25,  25,  10,   5,   5,
		  0,   0,   0,  20,  20,   0,   0,   0,
		  5,  -5, -10,   0,   0, -10,  -5,   5,
		  5,  10,  10, -20, -20,  10,  10,   5,
		  0,   0,   0,   0,   0,   0,   0,   0
	};

	const int knightPSQT[64] = {
		-50, -40, -30, -30, -30, -30, -40, -50,
		-40, -20,   0,   0,   0,   0, -20, -40,
		-30,   0,  10,  15,  15,  10,   0, -30,
		-30,   5,  15,  20,  20,  15,   5, -30,
		-30,   0,  15,  20,  20,  15,   0, -30,
		-30,   5,  10,  15,  15,  10,   5, -30,
		-40, -20,   0,   5,   5,   0, -20, -40,
		-50, -40, -30, -30, -30, -30, -40, -50
	};

	const int bishopPSQT[64] = {
		-20, -10, -10, -10, -10, -10, -10, -20,
		-10,   0,   0,   0,   0,   0,   0, -10,
		-10,   0,   5,  10,  10,   5,   0, -10,
		-10,   5,   5,  10,  10,   5,   5, -10,
		-10,   0,  10,  10,  10,  10,   0, -10,
		-10,  10,  10,  10,  10,  10,  10, -10,
		-10,   5,   0,   0,   0,   0,   5, -10,
		-20, -10, -10, -10, -10, -10, -10, -20
	};

	const int rookPSQT[64] = {
		 0,   0,   0,   0,   0,   0,   0,   0,
		 5,  10,  10,  10,  10,  10,  10,   5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		 0,   0,   0,   5,   5,   0,   0,   0
	};

	const int queenPSQT[64] = {
		-20, -10, -10,  -5,  -5, -10, -10, -20,
		-10,   0,   0,   0,   0,   0,   0, -10,
		-10,   0,   5,   5,   5,   5,   0, -10,
		 -5,   0,   5,   5,   5,   5,   0,  -5,
		  0,   0,   5,   5,   5,   5,   0,  -5,
		-10,   5,   5,   5,   5,   5,   0, -10,
		-10,   0,   5,   0,   0,   0,   0, -10,
		-20, -10, -10,  -5,  -5, -10, -10, -20
	};

	// Kuningas
	const int kingPSQT[64] = {
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-20, -30, -30, -40, -40, -30, -30, -20,
		-10, -20, -20, -20, -20, -20, -20, -10,
		 20,  20,   0,   0,   0,   0,  20,  20,
		 20,  30,  10,   0,   0,  10,  30,  20
	};

	// Palauttaa nappulalle ja ruudulle (rivi, linja) perustuvan PSQT-arvon.
	int getPieceSquareValue(int nappula, int rivi, int linja) const {
		int index = 0;
		// Valkoisten nappuloiden PSQT indeksointi: käännetään rivi
		if (nappula == wP || nappula == wN || nappula == wB ||
			nappula == wR || nappula == wQ || nappula == wK) {
			index = (7 - rivi) * 8 + linja;
		}
		else { // mustat nappulat
			index = rivi * 8 + linja;
		}

		switch (nappula) {
		case wP:
		case bP:
			return pawnPSQT[index];
		case wN:
		case bN:
			return knightPSQT[index];
		case wB:
		case bB:
			return bishopPSQT[index];
		case wR:
		case bR:
			return rookPSQT[index];
		case wQ:
		case bQ:
			return queenPSQT[index];
		case wK:
		case bK:
			return kingPSQT[index];
		default:
			return 0;
		}
	}
	// Laskee materiaalin perusteella annetun tilan.
	int nappulan_arvo(int nappula) const {
		switch (nappula) {
		case wP: return 100;
		case wN: return 320;
		case wB: return 330;
		case wR: return 500;
		case wQ: return 900;
		case wK: return 20000;
		case bP: return -100;
		case bN: return -320;
		case bB: return -330;
		case bR: return -500;
		case bQ: return -900;
		case bK: return -20000;
		default: return 0;
		}
	}



	float materiaali() const {
		float valkean_arvo = 0;
		float mustan_arvo = 0;
		for (int rivi = 0; rivi < 8; ++rivi) {
			for (int linja = 0; linja < 8; ++linja) {
				int nappula = _lauta[rivi][linja];
				if (nappula == NA) {
					continue;
				}
				if (on_vastustajan_nappula(nappula, VALKEA)) {
					mustan_arvo += nappulan_arvo(nappula);
				}
				else {
					valkean_arvo += nappulan_arvo(nappula);
				}
			}
		}
		return valkean_arvo - mustan_arvo;
	}
	float mobiliteetti() const {
		std::vector<Siirto> valkean_siirrot;
		std::vector<Siirto> mustan_siirrot;

		anna_kaikki_raakasiirrot(VALKEA, valkean_siirrot); 
		anna_kaikki_raakasiirrot(MUSTA, mustan_siirrot); 

		return (float)valkean_siirrot.size() - (float)mustan_siirrot.size();
	}

	
	float evaluoi() const {
		float score = 0.0f;
		for (int rivi = 0; rivi < 8; ++rivi) {
			for (int linja = 0; linja < 8; ++linja) {
				int nappula = _lauta[rivi][linja];
				if (nappula != NA) {
					int material = nappulan_arvo(nappula);
					int positional = getPieceSquareValue(nappula, rivi, linja);
					score += (material + positional);
				}
			}
		}
		// Halutessasi voit lisätä myös mobiliteetin vaikutuksen:
		// score += mobiliteetti();
		return score;
	}

	//Tämä liittyy undo-ominaisuuteen
	std::vector<Siirto> _siirtohistoria; // Tallentaa tehdyt siirrot
	std::vector<Asema> _asemaHistoria; // Tallentaa asemien tilat
	Asema& operator=(const Asema& toinen) 
	{
		if (this == &toinen) 
		{
			return *this; 
		}

		// Kopioidaan lauta
		for (int rivi = 0; rivi < 8; ++rivi) 
		{
			for (int linja = 0; linja < 8; ++linja) 
			{
				_lauta[rivi][linja] = toinen._lauta[rivi][linja];
			}
		}

		// Kopioidaan muut jäsenmuuttujat
		_siirtovuoro = toinen._siirtovuoro;
		_valkea_lyhyt_linna_sallittu = toinen._valkea_lyhyt_linna_sallittu;
		_valkea_pitka_linna_sallittu = toinen._valkea_pitka_linna_sallittu;
		_musta_lyhyt_linna_sallittu = toinen._musta_lyhyt_linna_sallittu;
		_musta_pitka_linna_sallittu = toinen._musta_pitka_linna_sallittu;
		_kaksoisaskel_linjalla = toinen._kaksoisaskel_linjalla;
		_siirtohistoria = toinen._siirtohistoria;
		_asemaHistoria = toinen._asemaHistoria;

		return *this;
	}
};
