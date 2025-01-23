#include <iostream>
#include "asema.h"
#include "siirto.h"

int main() {
    Asema asema;

    // Alustetaan pelilauta (voisit lisätä tähän oman tornin testitilanteen)
    asema.tyhjenna(); // Tyhjennä pelilauta
    asema._lauta[0][0] = wR; // Asetetaan valkoinen torni kohtaan a1

    std::cout << "Alkutilanne:" << std::endl;
    asema.tulosta();

    while (true) {
        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;

        // Pyydetään käyttäjältä siirto
        std::cout << "Anna tornin siirto (esim. a1a3): ";
        std::string syote;
        std::cin >> syote;

        // Muodostetaan Siirto-olio käyttäjän syötteestä
        Siirto siirto(syote);

        // Puretaan siirron tiedot
        lahto_rivi = siirto._a_r;
        lahto_linja = siirto._a_l;
        kohde_rivi = siirto._l_r;
        kohde_linja = siirto._l_l;

        // Tarkistetaan onko siirto laillinen
        if (asema.onko_laillinen_siirto(siirto, VALKEA)) {
            asema.tee_siirto(siirto, VALKEA); // Tehdään siirto
            std::cout << "Siirto suoritettu:" << std::endl;
            asema.tulosta();
        }
        else {
            std::cout << "Ei laillinen siirto! Yritä uudelleen." << std::endl;
        }
    }

    return 0;
}
