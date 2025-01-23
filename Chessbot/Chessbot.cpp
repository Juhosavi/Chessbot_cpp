#include <iostream>
#include "asema.h"
#include "siirto.h"

int main() {
    Asema asema;

    // Alustetaan pelilauta (voisit lis�t� t�h�n oman tornin testitilanteen)
    asema.tyhjenna(); // Tyhjenn� pelilauta
    asema._lauta[0][0] = wR; // Asetetaan valkoinen torni kohtaan a1

    std::cout << "Alkutilanne:" << std::endl;
    asema.tulosta();

    while (true) {
        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;

        // Pyydet��n k�ytt�j�lt� siirto
        std::cout << "Anna tornin siirto (esim. a1a3): ";
        std::string syote;
        std::cin >> syote;

        // Muodostetaan Siirto-olio k�ytt�j�n sy�tteest�
        Siirto siirto(syote);

        // Puretaan siirron tiedot
        lahto_rivi = siirto._a_r;
        lahto_linja = siirto._a_l;
        kohde_rivi = siirto._l_r;
        kohde_linja = siirto._l_l;

        // Tarkistetaan onko siirto laillinen
        if (asema.onko_laillinen_siirto(siirto, VALKEA)) {
            asema.tee_siirto(siirto, VALKEA); // Tehd��n siirto
            std::cout << "Siirto suoritettu:" << std::endl;
            asema.tulosta();
        }
        else {
            std::cout << "Ei laillinen siirto! Yrit� uudelleen." << std::endl;
        }
    }

    return 0;
}
