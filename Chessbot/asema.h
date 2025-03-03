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
			numeric_limits<float>::min() : numeric_limits<float>::max();
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



	// Laskee materiaalitasapainon (valkean nappuloiden arvo - mustan nappuloiden arvo).
	// Nappuloiden arvot:
	//
	// sotilas		1
	// ratsu		3
	// lähetti		3
	// torni		5
	// daami		9
	//
	int nappulan_arvo(int nappula) const { 
		switch (nappula) {
		case wP: return 1;
		case wN: return 3;
		case wB: return 3;
		case wR: return 5;
		case wQ: return 9;
		case wK: return 100;
		case bP: return -1;
		case bN: return -3;
		case bB: return -3;
		case bR: return -5;
		case bQ: return -9;
		case bK: return -100;
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
		return 1.0f * materiaali();
	}

};
