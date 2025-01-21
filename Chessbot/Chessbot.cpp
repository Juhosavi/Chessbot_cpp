#include <iostream>
#include "Asema.h"
#include <vector>

int main()
{
    Asema asema;
    

    std::vector<Siirto> siirrot;

    // Testaa tornin siirrot tietystä sijainnista
    int rivi = 5, linja = 5; // Voit asettaa tornin lähtösijainnin testiin
    asema.anna_tornin_raakasiirrot(rivi, linja, VALKEA, siirrot);
    asema.tulosta();

    // Tulosta lasketut tornin siirrot
    std::cout << "Tornin mahdolliset siirrot (" << rivi << ", " << linja << "):" << std::endl;
    for (const auto& siirto : siirrot)
    {
        std::cout << "Lähtö: (" << siirto._a_r << ", " << siirto._a_l << ") "
            << "-> Kohde: (" << siirto._l_r << ", " << siirto._l_l << ")" << std::endl;
    }

    return 0;
}
