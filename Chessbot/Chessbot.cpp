#include <iostream>
#include <thread>
#include "asema.h"
#include "siirto.h"
#include "shakki.h"
#include "kayttis.h"

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(0)));
    Asema asema;
    int interfaceChoice, colorChoice;

    cout << "Valitse käyttöliittymä:" << endl;
    cout << "  1. SFML" << endl;
    cout << "  2. Terminal" << endl;
    cout << "  3. Molemmat (SFML ja Terminal)" << endl;
    cout << "Valintasi: ";
    cin >> interfaceChoice;

    cout << "Valitse pelattava väri:" << endl;
    cout << "  1. Valkoinen (pelaat manuaalisesti)" << endl;
    cout << "  2. Musta (pelaat manuaalisesti)" << endl;
    cout << "Valintasi: ";
    cin >> colorChoice;

    int playerColor = (colorChoice == 1) ? VALKEA : MUSTA;

    // Pelaajan ohjaama puoli toimii manuaalisesti; toinen puoli on AI.
    if (interfaceChoice == 1) {
        sfml_gui(asema, playerColor);
    }
    else if (interfaceChoice == 2) {
        terminal_ui(asema, playerColor);
    }
    else if (interfaceChoice == 3) {
        thread t1(sfml_gui, ref(asema), playerColor);
        thread t2(terminal_ui, ref(asema), playerColor);
        t1.join();
        t2.join();
    }
    else {
        cout << "Virheellinen valinta!" << endl;
    }

    return 0;
}
