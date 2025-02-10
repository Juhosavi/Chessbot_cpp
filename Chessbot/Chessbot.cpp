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
        if (!texture.loadFromFile("C:/GitHub/uusiChess/pieces/" + piece + ".png")) {
            cerr << "Virhe ladattaessa tekstuuria: " << piece << endl;
        }
        textures[piece] = texture;
    }
}

// SFML-graafinen käyttöliittymä
void sfml_gui(Asema& asema)
{
    sf::Font font;
    if (!font.loadFromFile("C:/Users/savin/source/repos/Chessbot/font/AldotheApache.ttf")) {
        cerr << "Virhe ladattaessa fonttia!" << endl;
    }
    sf::Text text;
    sf::Text text2;
    text.setFont(font);
    text2.setFont(font);
    text.setString("         A        B        C        D        E        F        G        H");
    text2.setString("\n 8 \n\n 7 \n\n 6 \n\n 5 \n\n 4 \n\n 3 \n\n 2 \n\n 1");
    sf::Color darkBrown(139, 69, 19);  // Tummanruskea
    sf::Color lightBrown(222, 184, 135);  // Vaaleanruskea

    text.setCharacterSize(45);
    text2.setCharacterSize(60);
    text.setFillColor(sf::Color::White);
    sf::RenderWindow window(sf::VideoMode(900, 900), "SFML Chessboard");
    lataaTekstuurit(); // Ladataan nappuloiden tekstuurit
    map<int, string> pieceMap = {
        {wP, "white-pawn"}, {wR, "white-rook"}, {wN, "white-knight"},
        {wB, "white-bishop"}, {wQ, "white-queen"}, {wK, "white-king"},
        {bP, "black-pawn"}, {bR, "black-rook"}, {bN, "black-knight"},
        {bB, "black-bishop"}, {bQ, "black-queen"}, {bK, "black-king"}
    };

    vector<Siirto> mahdollisetSiirrot;
    int valittuRivi = -1, valittuLinja = -1;
    bool siirtoValittu = false;
    Siirto valittuSiirto;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                int x = (event.mouseButton.x - 50) / tileSize;
                int y = (event.mouseButton.y - 50) / tileSize;
                if (x >= 0 && x < 8 && y >= 0 && y < 8)
                {
                    if (!siirtoValittu) {
                        valittuRivi = y;
                        valittuLinja = x;
                        mahdollisetSiirrot.clear();
                        asema.anna_siirrot(mahdollisetSiirrot);
                        siirtoValittu = true;
                    }
                    else {
                        for (const auto& siirto : mahdollisetSiirrot) {
                            if (siirto._a_r == valittuRivi && siirto._a_l == valittuLinja && siirto._l_r == y && siirto._l_l == x) {
                                valittuSiirto = siirto;
                                asema.tee_siirto(valittuSiirto, asema._siirtovuoro);
                                siirtoValittu = false;
                                break;
                            }
                        }
                    }
                }
            }
        }

        window.clear();

        // Piirretään shakkilauta
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(x * tileSize + 50, y * tileSize + 50);

                // Shakkilaudan ruutujen värit
                if ((x + y) % 2 == 0)
                    tile.setFillColor(darkBrown);
                else
                    tile.setFillColor(lightBrown);

                window.draw(tile);

                // Piirretään nappulat
                int piece = asema._lauta[y][x];
                if (pieceMap.find(piece) != pieceMap.end()) {
                    sf::Sprite sprite;
                    sprite.setTexture(textures[pieceMap[piece]]);
                    sprite.setOrigin(-35, -35);
                    sprite.setPosition(x * tileSize, y * tileSize);
                    window.draw(sprite);
                }
            }
        }

        // Piirretään mahdolliset siirrot
        for (const auto& siirto : mahdollisetSiirrot) {
            if (siirto._a_r == valittuRivi && siirto._a_l == valittuLinja) {
                sf::CircleShape circle(15);
                circle.setFillColor(sf::Color(0, 255, 0, 150));
                circle.setPosition(siirto._l_l * tileSize + 50 + 35, siirto._l_r * tileSize + 50 + 35);
                window.draw(circle);
            }
        }

        window.draw(text);
        window.draw(text2);
        window.display();
    }
}

// Tekstipohjainen shakkipeli komentorivillä
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
                cout << "Ei laillinen siirto! Yritä uudelleen." << endl;
            }
        }

        asema.tee_siirto(kayttajan_siirto, asema._siirtovuoro);
        asema.anna_siirrot(siirrot);
    }
}

int main() {
    Asema asema;

    // Luodaan kaksi säiettä: yksi SFML-grafiikalle ja toinen komentoriville
    thread t1(sfml_gui, ref(asema));
    thread t2(terminal_ui, ref(asema));

    // Odotetaan, että molemmat säikeet päättyvät
    t1.join();
    t2.join();

    return 0;
}
