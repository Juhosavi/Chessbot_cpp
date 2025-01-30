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


        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;

        // Kutsutaan funktiota kysymään siirtoa
        asema.kysy_siirto(asema._siirtovuoro, lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

        // Tehdään siirto
        Siirto siirto(lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);
        asema.tee_siirto(siirto, asema._siirtovuoro);

    }

    return 0;
}