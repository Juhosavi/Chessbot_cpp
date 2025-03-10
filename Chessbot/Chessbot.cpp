/**
 * @file main.cpp
 * @brief P‰‰ohjelma shakki-sovellukselle.
 */

#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <SFML/Graphics.hpp>
#include "asema.h"
#include "siirto.h"
#include "shakki.h"
#include <execution>
#include "kayttis.h"

 /**
  * @brief P‰‰ohjelma, joka k‰ynnist‰‰ shakkisovelluksen.
  *
  * Luo kaksi erillist‰ s‰iett‰ graafiselle ja tekstipohjaiselle k‰yttˆliittym‰lle.
  * Molemmat s‰ikeet k‰ytt‰v‰t yhteist‰ Asema-oliota.
  *
  * @return Palauttaa 0 ohjelman suorituksen p‰‰tytty‰.
  */
int main() {
    Asema asema;  ///< Pelitilannetta kuvaava olio.


    ////Linnoitus asetelma testi
    //asema.tyhjenna();

    //asema._lauta[7][4] = wK;
    //asema._lauta[7][7] = wR;
    //asema._lauta[7][0] = wR;

    //asema._lauta[0][4] = bK;
    //asema._lauta[0][0] = bR;
    //asema._lauta[0][7] = bR;
    //asema._lauta[6][0] = wP;

    //asema._lauta[0][2] = bB; 
    //asema._lauta[0][5] = bB; 

    /// Luodaan s‰ikeet graafiselle ja tekstipohjaiselle k‰yttˆliittym‰lle.
    std::thread t1(sfml_gui, std::ref(asema));
    std::thread t2(terminal_ui, std::ref(asema));

    /// Odotetaan, ett‰ molemmat s‰ikeet p‰‰ttyv‰t ennen ohjelman sulkemista.
    t1.join();
    t2.join();

    return 0;
}
