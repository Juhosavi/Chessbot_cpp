#include <iostream>
#include <vector>
#include "asema.h"
#include "siirto.h"
using namespace std;

int main() {
    Asema asema;
    vector<Siirto> siirrot;

    while (true) {
        asema.tulosta();
        siirrot.clear();
        asema.anna_siirrot(siirrot);

        std::cout << "Siirtoja: " << siirrot.size() << std::endl;
        for (int i = 0; i < siirrot.size(); i++) {
            char alkuL = 'a' + siirrot[i]._a_l;
            char alkuR = '1' + (7 - siirrot[i]._a_r);
            char loppuL = 'a' + siirrot[i]._l_l;
            char loppuR = '1' + (7 - siirrot[i]._l_r);

            std::cout << i << ": " << alkuL << alkuR << " --- > " << loppuL << loppuR << std::endl;
        }

        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;
        Siirto kayttajan_siirto;

        while (true) {
            // Kutsutaan funktiota kysymään siirtoa
            asema.kysy_siirto(asema._siirtovuoro, lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

            // Luo käyttäjän syötettä vastaava siirto-olio
            kayttajan_siirto = Siirto(lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

            // Tarkista, löytyykö laillisista siirroista käyttäjän siirtoa vastaavaa oliota
            bool laillinen_siirto = false;
            for (const auto& s : siirrot) {
                if (s == kayttajan_siirto) {
                    for (const auto& s : siirrot) {
                        if (s == kayttajan_siirto) {
                            laillinen_siirto = true;
                            break;
                        }
                    }
                    laillinen_siirto = true;
                    break;
                }
            }

            if (laillinen_siirto) {
                break; // Hyväksytään siirto ja poistutaan silmukasta
            }
            else {
                std::cout << "Ei laillinen siirto! Yritä uudelleen." << std::endl;
            }
        }

        // Tehdään siirto
        asema.tee_siirto(kayttajan_siirto, asema._siirtovuoro);
        asema.anna_siirrot(siirrot);
    }

    return 0;
}
