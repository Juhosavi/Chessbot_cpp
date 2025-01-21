#include <iostream>
#include "Asema.h"
#include <vector>

int main()
{
    int rivi, linja;

    Asema asema;
    asema.tulosta();
    // Tehd‰‰n siirto
   
    std::string liikkuminen;
    while (liikkuminen != "0") 
    {
    std::cout << "mihin liikut: ";
    std::cin >> liikkuminen;
    Siirto siirto(liikkuminen);
    asema.tee_siirto(siirto);

    // Tulostetaan pelilauta siirron j‰lkeen
    asema.tulosta();

    }

    return 0;
}
