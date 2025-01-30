#include <iostream>
#include <vector>
#include "asema.h"
#include "siirto.h"
using namespace std;


int main() {


    Asema asema;
    vector<Siirto> siirrot;

    while (true) 
    {
        asema.tulosta();
        siirrot.clear();
        asema.anna_kaikki_raakasiirrot(asema._siirtovuoro, siirrot);

        std::cout << "Siirtoja: " << siirrot.size() << std::endl;
        for (int i = 0; i < siirrot.size(); i++)
        {
            char alkuL = 'a' + siirrot[i]._a_l;
            char alkuR = '1' + (7 - siirrot[i]._a_r);
            char loppuL = 'a' + siirrot[i]._l_l;
            char loppuR = '1' + (7 - siirrot[i]._l_r);

            std::cout << i << ": " << alkuL << alkuR  << " --- > " << loppuL << loppuR << std::endl;
        }

    
        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;

        // Kutsutaan funktiota kysymään siirtoa
        asema.kysy_siirto(asema._siirtovuoro, lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

        // Tehdään siirto
        Siirto siirto(lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);
        asema.tee_siirto(siirto, asema._siirtovuoro);

    }

    return 0;
}