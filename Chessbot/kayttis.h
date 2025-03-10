#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <SFML/Graphics.hpp>
#include "asema.h"
#include "siirto.h"
#include "shakki.h"
#include <execution>

using namespace std;

// Shakkilaudan ruudun koko
const int tileSize = 100;

/**
 * @brief Tekstuurit kaikille nappuloille
 *
 * T�m� kartta tallentaa tekstuurit kaikille shakkipelin nappuloille.
 */
map<string, sf::Texture> textures;

/**
 * @brief Lataa kaikki shakkipelin nappulat
 *
 * T�m� funktio lataa kaikki shakkipelin nappuloiden kuvatiedostot.
 */
void lataaTekstuurit() {
    vector<string> pieces = {
        "black-bishop", "black-king", "black-knight", "black-pawn",
        "black-queen", "black-rook", "white-bishop", "white-king",
        "white-knight", "white-pawn", "white-queen", "white-rook"
    };

    for (const string& piece : pieces) {
        sf::Texture texture;
        if (!texture.loadFromFile("/pieces/" + piece + ".png")) {
            cerr << "Virhe ladattaessa tekstuuria: " << piece << endl;
        }
        textures[piece] = texture;
    }
}

/**
 * @brief N�ytt�� ylennysvalinta SFML-ikkunassa ja antaa k�ytt�j�n valinnan
 *
 * Funktio odottaa, ett� k�ytt�j� klikkaa jollain ylennysnappulalla ja asettaa sen
 * siirto-objektiin.
 *
 * @param window SFML-ikkuna, jossa n�ytet��n ylennysvalinta
 * @param asema Pelin nykyinen asema
 * @param move Siirto, johon ylennys tallennetaan
 */
void promotionDialog(sf::RenderWindow& window, Asema& asema, Siirto& move) {
    int chosenPromotion = NA;
    bool validSelection = false;

    // Luodaan promotion-valinnan sprite-oliot.
    sf::Sprite queenSprite(textures["white-queen"]);
    sf::Sprite rookSprite(textures["white-rook"]);
    sf::Sprite bishopSprite(textures["white-bishop"]);
    sf::Sprite knightSprite(textures["white-knight"]);

    // Aseta spritejen sijainnit (esim. ruudut keskelle ikkunaa)
    queenSprite.setPosition(300, 300);
    rookSprite.setPosition(450, 300);
    bishopSprite.setPosition(300, 450);
    knightSprite.setPosition(450, 450);

    while (!validSelection) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);
                if (queenSprite.getGlobalBounds().contains(mousePos)) {
                    chosenPromotion = (asema._siirtovuoro == VALKEA ? wQ : bQ);
                    validSelection = true;
                }
                else if (rookSprite.getGlobalBounds().contains(mousePos)) {
                    chosenPromotion = (asema._siirtovuoro == VALKEA ? wR : bR);
                    validSelection = true;
                }
                else if (bishopSprite.getGlobalBounds().contains(mousePos)) {
                    chosenPromotion = (asema._siirtovuoro == VALKEA ? wB : bB);
                    validSelection = true;
                }
                else if (knightSprite.getGlobalBounds().contains(mousePos)) {
                    chosenPromotion = (asema._siirtovuoro == VALKEA ? wN : bN);
                    validSelection = true;
                }
            }
        }

        // Piirret��n ylennysvalinta-overlay
        window.clear();
        sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        window.draw(queenSprite);
        window.draw(rookSprite);
        window.draw(bishopSprite);
        window.draw(knightSprite);
        window.display();
    }

    // Aseta k�ytt�j�n valitsema nappula ylennykseksi
    move._korotettava_nappula = chosenPromotion;
}

/**
 * @brief SFML-k�ytt�liittym�, joka piirt�� shakkilaudan ja k�sittelee hiiritapahtumat
 *
 * T�m� funktio luo SFML-ikkunan, jossa pelin shakkilauta ja nappulat piirret��n.
 * K�ytt�j� voi valita siirtoja klikkaamalla hiirell�.
 *
 * @param asema Pelin nykyinen asema
 */
void sfml_gui(Asema& asema) {
    sf::Font font;
    if (!font.loadFromFile("/font/AldotheApache.ttf")) {
        cerr << "Virhe ladattaessa fonttia!" << endl;
    }
    sf::Text text, text2;
    text.setFont(font);
    text2.setFont(font);
    text.setString("         A        B        C        D        E        F        G        H");
    text2.setString("\n 8 \n\n 7 \n\n 6 \n\n 5 \n\n 4 \n\n 3 \n\n 2 \n\n 1");
    text.setCharacterSize(45);
    text2.setCharacterSize(60);
    text.setFillColor(sf::Color::White);

    sf::RenderWindow window(sf::VideoMode(900, 900), "SFML Chessboard");
    lataaTekstuurit();
    map<int, string> pieceMap = {
        {wP, "white-pawn"}, {wR, "white-rook"}, {wN, "white-knight"},
        {wB, "white-bishop"}, {wQ, "white-queen"}, {wK, "white-king"},
        {bP, "black-pawn"}, {bR, "black-rook"}, {bN, "black-knight"},
        {bB, "black-bishop"}, {bQ, "black-queen"}, {bK, "black-king"}
    };

    vector<Siirto> mahdollisetSiirrot;
    mahdollisetSiirrot.reserve(100);
    int valittuRivi = -1, valittuLinja = -1;
    bool siirtoValittu = false;
    Siirto valittuSiirto;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = (event.mouseButton.x - 50) / tileSize;
                int y = (event.mouseButton.y - 50) / tileSize;
                if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                    if (!siirtoValittu) {
                        valittuRivi = y;
                        valittuLinja = x;
                        mahdollisetSiirrot.clear();
                        asema.anna_siirrot(mahdollisetSiirrot);
                        siirtoValittu = true;
                    }
                    else {
                        bool validMove = false;
                        for (const auto& s : mahdollisetSiirrot) {
                            if (s._a_r == valittuRivi && s._a_l == valittuLinja &&
                                s._l_r == y && s._l_l == x) {
                                valittuSiirto = s;
                                // Tarkistetaan, onko kyseess� ylennystilanne (sotilas p��ssyt viimeiselle riville)
                                if ((asema._siirtovuoro == VALKEA && y == 0 && asema._lauta[valittuRivi][valittuLinja] == wP) ||
                                    (asema._siirtovuoro == MUSTA && y == 7 && asema._lauta[valittuRivi][valittuLinja] == bP)) {
                                    promotionDialog(window, asema, valittuSiirto);
                                }
                                asema.tee_siirto(valittuSiirto, asema._siirtovuoro);
                                validMove = true;
                                break;
                            }
                        }
                        if (!validMove)
                            siirtoValittu = false;
                    }
                }
            }
        }

        window.clear();
        sf::Color darkBrown(139, 69, 19);
        sf::Color lightBrown(222, 184, 135);
        // Piirret��n shakkilauta
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(x * tileSize + 50, y * tileSize + 50);
                if ((x + y) % 2 == 0)
                    tile.setFillColor(lightBrown);
                else
                    tile.setFillColor(darkBrown);
                window.draw(tile);

                // Piirret��n nappulat
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
        // Piirret��n mahdolliset siirrot vihrein� ympyr�in�
        for (const auto& s : mahdollisetSiirrot) {
            if (s._a_r == valittuRivi && s._a_l == valittuLinja) {
                sf::CircleShape circle(15);
                circle.setFillColor(sf::Color(0, 255, 0, 150));
                circle.setPosition(s._l_l * tileSize + 50 + 35, s._l_r * tileSize + 50 + 35);
                window.draw(circle);
            }
        }
        window.draw(text);
        window.draw(text2);
        window.display();
    }
}

/**
 * @brief Tekstipohjainen k�ytt�liittym� komentorivill�
 *
 * T�m� funktio mahdollistaa tekstipohjaisen shakkipelin pelaamisen komentorivill�.
 * Peliss� k�ytt�j� sy�tt�� siirrot komentoriville.
 *
 * @param asema Pelin nykyinen asema
 */
void terminal_ui(Asema& asema) {
    vector<Siirto> siirrot;
    siirrot.reserve(100);
    while (true) {
        asema.tulosta();
        siirrot.clear();
        asema.anna_siirrot(siirrot);
        MinimaxArvo minimaxTulos = asema.minimaxAlphaBetaAsync(
            4,
            -std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::infinity(),
            true  // tai false, jos et halua rinnakkaista juuritasoa
        );

        std::cout << "Minimax Arvo: " << minimaxTulos._arvo << std::endl;
        std::cout << "Paras siirto: "
            << "Lahto: "
            << static_cast<char>('a' + minimaxTulos._siirto._a_l)
            << static_cast<char>('1' + (7 - minimaxTulos._siirto._a_r))
            << " -> Kohde: "
            << static_cast<char>('a' + minimaxTulos._siirto._l_l)
            << static_cast<char>('1' + (7 - minimaxTulos._siirto._l_r))
            << std::endl;
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
            if (laillinen_siirto)
                break;
            else
                cout << "Ei laillinen siirto! Yrit� uudelleen." << endl;
        }
        asema.tee_siirto(kayttajan_siirto, asema._siirtovuoro);
        asema.anna_siirrot(siirrot);
    }
}
