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
	// [0][0] : vasen yl‰nurkka ("a8")
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

	// Kirjanpito siit‰, onko kuningas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestalyˆntiin tarvittava info.
	int _kaksoisaskel_linjalla = -1;

	// Tekee annetun siirron laudalla. Voidaan olettaa, ett‰
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
	float pisteyta_lopputulos(int syvyys) const;
	void tyhjenna();

//------------------------UUSI KOODI------------------------

	MinimaxArvo minimax(int syvyys)
	{
		vector<Siirto> siirrot;
		anna_siirrot(siirrot);

		if (siirrot.empty())
		{
			// Lopputilanteessa k‰ytet‰‰n pisteyt‰_lopputulos()-funktiota,
			// ja syvyys-parametri v‰litet‰‰n.
			return MinimaxArvo(pisteyta_lopputulos(syvyys), Siirto());
		}

		if (syvyys == 0)
		{
			return MinimaxArvo(evaluoi(), Siirto());
		}

		// K‰ytet‰‰n vektorimuuttujaa parhaiden siirtojen ker‰‰miseen.
		float paras_arvo = (_siirtovuoro == VALKEA) ?
			numeric_limits<float>::lowest() : numeric_limits<float>::max();
		vector<Siirto> parhaatSiirrot;
		const float epsilon = 0.01f;  // Pieni toleranssi

		for (Siirto& s : siirrot)
		{
			Asema uusi = *this;
			uusi.tee_siirto(s, _siirtovuoro);
			MinimaxArvo arvo = uusi.minimax(syvyys - 1);

			if (_siirtovuoro == VALKEA)
			{
				if (arvo._arvo > paras_arvo + epsilon)
				{
					paras_arvo = arvo._arvo;
					parhaatSiirrot.clear();
					parhaatSiirrot.push_back(s);
				}
				else if (fabs(arvo._arvo - paras_arvo) < epsilon)
				{
					parhaatSiirrot.push_back(s);
				}
			}
			else  // MUSTA
			{
				if (arvo._arvo < paras_arvo - epsilon)
				{
					paras_arvo = arvo._arvo;
					parhaatSiirrot.clear();
					parhaatSiirrot.push_back(s);
				}
				else if (fabs(arvo._arvo - paras_arvo) < epsilon)
				{
					parhaatSiirrot.push_back(s);
				}
			}
		}

		// Jos useita siirtoja on yht‰ hyvi‰, valitaan niiden joukosta satunnaisesti
		int randomIndex = rand() % parhaatSiirrot.size();
		return MinimaxArvo(paras_arvo, parhaatSiirrot[randomIndex]);
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
		 0,   10,   0,   5,   5,   0,   0,   0
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
		int index = (7 - rivi) * 8 + linja; // Peilataan valkoisille

		if (nappula == bP || nappula == bN || nappula == bB ||
			nappula == bR || nappula == bQ || nappula == bK) {
			index = rivi * 8 + linja; // Peilataan mustille
		}

		switch (nappula) {
		case wP:
		case bP:
			return pawnPSQT[index] * (nappula == wP ? 1 : -1);
		case wN:
		case bN:
			return knightPSQT[index] * (nappula == wN ? 1 : -1);
		case wB:
		case bB:
			return bishopPSQT[index] * (nappula == wB ? 1 : -1);
		case wR:
		case bR:
			return rookPSQT[index] * (nappula == wR ? 1 : -1);
		case wQ:
		case bQ:
			return queenPSQT[index] * (nappula == wQ ? 1 : -1);
		case wK:
		case bK:
			return kingPSQT[index] * (nappula == wK ? 1 : -1);
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
		// Halutessasi voit lis‰t‰ myˆs mobiliteetin vaikutuksen:
		// score += mobiliteetti();
		return score;
	}

};
