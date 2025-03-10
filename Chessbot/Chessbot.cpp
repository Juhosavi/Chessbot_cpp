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
    // Luodaan s�ikeet graafiselle ja tekstipohjaiselle k�ytt�liittym�lle
    thread t1(sfml_gui, ref(asema));
    thread t2(terminal_ui, ref(asema));
    t1.join();
    t2.join();
    return 0;
}
