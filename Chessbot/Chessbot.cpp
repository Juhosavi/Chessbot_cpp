#include <iostream>
#include "asema.h"
#include "siirto.h"

int main() {
    Asema asema;
    asema.tyhjenna();

    //TESTI NAPPULAT TÄHÄN
    asema._lauta[6][4] = wP;
    asema._lauta[5][5] = wP; //valkoinen 

    asema._lauta[3][3] = bP; //musta 
    asema._lauta[1][5] = bP; //musta 

    asema.tulosta();
    int nykyinen_pelaaja = VALKEA;

    while (true) 
    {
        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;

        // Kutsutaan funktiota kysymään siirtoa
        asema.kysy_siirto(nykyinen_pelaaja, lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

        // Tehdään siirto
        Siirto siirto(lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);
        asema.tee_siirto(siirto, nykyinen_pelaaja);

        asema.tulosta();

        // Vaihdetaan vuoroa
        nykyinen_pelaaja = (nykyinen_pelaaja == VALKEA) ? MUSTA : VALKEA;
        std::cout << (nykyinen_pelaaja == VALKEA ? "VALKOISEN" : "MUSTAN") << " VUORO,  " << std::endl;
    }

    return 0;
}