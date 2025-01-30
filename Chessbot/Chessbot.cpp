#include <iostream>
#include "asema.h"
#include "siirto.h"

int main() {
    Asema asema;

    asema.tulosta();
    int nykyinen_pelaaja = VALKEA;

    while (true) 
    {
        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;

        // Kutsutaan funktiota kysym‰‰n siirtoa
        asema.kysy_siirto(nykyinen_pelaaja, lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

        // Tehd‰‰n siirto
        Siirto siirto(lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);
        asema.tee_siirto(siirto, nykyinen_pelaaja);

        asema.tulosta();

        // Vaihdetaan vuoroa
        nykyinen_pelaaja = (nykyinen_pelaaja == VALKEA) ? MUSTA : VALKEA;
        std::cout << (nykyinen_pelaaja == VALKEA ? "VALKOISEN" : "MUSTAN") << " VUORO,  " << std::endl;
    }

    return 0;
}