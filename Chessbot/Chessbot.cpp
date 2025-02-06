#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <SFML/Graphics.hpp>
#include "asema.h"
#include "siirto.h"
#include "shakki.h"

using namespace std;

// Shakkilaudan ruudun koko
const int tileSize = 100;

// Tekstuurit kaikille nappuloille
map<string, sf::Texture> textures;

// Lataa kaikki shakkipelin nappulat
void lataaTekstuurit() {
    vector<string> pieces = {
        "black-bishop", "black-king", "black-knight", "black-pawn",
        "black-queen", "black-rook", "white-bishop", "white-king",
        "white-knight", "white-pawn", "white-queen", "white-rook"
    };

    for (const string& piece : pieces) {
        sf::Texture texture;
        if (!texture.loadFromFile("C:/Chessbot_cpp/pieces/" + piece + ".png")) {
            cerr << "Virhe ladattaessa tekstuuria: " << piece << endl;
        }
        textures[piece] = texture;
    }
}

// SFML-graafinen k‰yttˆliittym‰
void sfml_gui(Asema& asema)
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Chessboard");
    lataaTekstuurit(); // Ladataan nappuloiden tekstuurit

    map<int, string> pieceMap = {
        {wP, "white-pawn"}, {wR, "white-rook"}, {wN, "white-knight"},
        {wB, "white-bishop"}, {wQ, "white-queen"}, {wK, "white-king"},
        {bP, "black-pawn"}, {bR, "black-rook"}, {bN, "black-knight"},
        {bB, "black-bishop"}, {bQ, "black-queen"}, {bK, "black-king"}
    };

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Piirret‰‰n shakkilauta
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(x * tileSize, y * tileSize);

                // Shakkilaudan ruutujen v‰rit
                if ((x + y) % 2 == 0)
                    tile.setFillColor(sf::Color::Cyan);
                else
                    tile.setFillColor(sf::Color::Blue);

                window.draw(tile);

                // Piirret‰‰n nappulat
                int piece = asema._lauta[y][x];
                if (pieceMap.find(piece) != pieceMap.end()) {
                    sf::Sprite sprite;
                    sprite.setTexture(textures[pieceMap[piece]]);
                    sprite.setPosition(x * tileSize, y * tileSize);
                    window.draw(sprite);
                }
            }
        }

        window.display();
    }
}

// Tekstipohjainen shakkipeli komentorivill‰
void terminal_ui(Asema& asema) {
    vector<Siirto> siirrot;

    while (true) {
        asema.tulosta();
        siirrot.clear();
        asema.anna_siirrot(siirrot);

        cout << "Siirtoja: " << siirrot.size() << endl;

        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;
        Siirto kayttajan_siirto;

        while (true) {
            asema.kysy_siirto(asema._siirtovuoro, lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

            kayttajan_siirto = Siirto(lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

            bool laillinen_siirto = false;
            for (const auto& s : siirrot) {
                if (s == kayttajan_siirto) {
                    laillinen_siirto = true;
                    break;
                }
            }

            if (laillinen_siirto) {
                break;
            }
            else {
                cout << "Ei laillinen siirto! Yrit‰ uudelleen." << endl;
            }
        }

        asema.tee_siirto(kayttajan_siirto, asema._siirtovuoro);
        asema.anna_siirrot(siirrot);
    }
}

int main() {
    Asema asema;

    // Luodaan kaksi s‰iett‰: yksi SFML-grafiikalle ja toinen komentoriville
    thread t1(sfml_gui, ref(asema));
    thread t2(terminal_ui, ref(asema));

    // Odotetaan, ett‰ molemmat s‰ikeet p‰‰ttyv‰t
    t1.join();
    t2.join();

    return 0;
}
