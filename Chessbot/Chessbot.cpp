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


int main() {
    Asema asema;
    // Luodaan säikeet graafiselle ja tekstipohjaiselle käyttöliittymälle
    thread t1(sfml_gui, ref(asema));
    thread t2(terminal_ui, ref(asema));
    t1.join();
    t2.join();
    return 0;
}
