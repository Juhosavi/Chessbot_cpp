#include <iostream>
#include "Asema.h"

int main()
{
    int rivi, linja;

    Asema asema;
 

    // Etsit‰‰n mustan tornin sijainti
    asema.etsi_kuningas(bR, rivi, linja);
    std::cout << "Tornin sijainti: " << rivi << ", " << linja << std::endl;

    // Tehd‰‰n siirto
    Siirto siirto("a2a6");
    asema.tee_siirto(siirto);

    // Tulostetaan pelilauta siirron j‰lkeen
    asema.tulosta();

    return 0;
}
