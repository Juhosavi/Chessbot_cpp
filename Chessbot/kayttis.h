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
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

// Shakkilaudan ruudun koko
const int tileSize = 100;

// Tekstuurit kaikille nappuloille
map<string, sf::Texture> textures;

// Lataa kaikki shakkipelin nappuloiden tekstuurit
void lataaTekstuurit() {
    vector<string> pieces = {
        "black-bishop", "black-king", "black-knight", "black-pawn",
        "black-queen", "black-rook", "white-bishop", "white-king",
        "white-knight", "white-pawn", "white-queen", "white-rook"
    };

    for (const string& piece : pieces) {
        sf::Texture texture;
        if (!texture.loadFromFile("C:/Users/savin/source/repos/Chessbot/pieces/" + piece + ".png")) {
            cerr << "Virhe ladattaessa tekstuuria: " << piece << endl;
        }
        textures[piece] = texture;
    }
}

// Näytä ylennysvalinta SFML-ikkunassa ja anna käyttäjän tehdä valinta
void promotionDialog(sf::RenderWindow& window, Asema& asema, Siirto& move) {
    int chosenPromotion = NA;
    bool validSelection = false;

    // Luodaan sprite-oliot ylennysvalinnalle.
    sf::Sprite queenSprite(textures["white-queen"]);
    sf::Sprite rookSprite(textures["white-rook"]);
    sf::Sprite bishopSprite(textures["white-bishop"]);
    sf::Sprite knightSprite(textures["white-knight"]);

    // Aseta spritejen sijainnit (keskelle ikkunaa)
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
        // Piirretään ylennysvalinta-overlay
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

    // Aseta käyttäjän valitsema nappula ylennykseksi
    move._korotettava_nappula = chosenPromotion;
}

// SFML-käyttöliittymä, jossa pelaajan vuorolla odotetaan hiiritapahtumia ja muulloin AI laskee siirtonsa.
void sfml_gui(Asema& asema, int playerColor) {
    sf::Font font;
    if (!font.loadFromFile("C:/Users/savin/source/repos/Chessbot/font/AldotheApache.ttf")) {
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

    // Muuttujat pelaajan siirron hallintaan
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

            // Jos on pelaajan vuoro (valitun värin mukaisesti), käsitellään hiiritapahtumat.
            if (asema._siirtovuoro == playerColor) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int x = (event.mouseButton.x - 50) / tileSize;
                    int y = (event.mouseButton.y - 50) / tileSize;
                    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                        if (!siirtoValittu) {
                            // Ensimmäisellä klikkauksella valitaan lähtöruutu
                            valittuRivi = y;
                            valittuLinja = x;
                            mahdollisetSiirrot.clear();
                            asema.anna_siirrot(mahdollisetSiirrot);
                            siirtoValittu = true;
                        }
                        else {
                            // Toisella klikkauksella etsitään siirto, joka lähtee valitusta ruudusta.
                            bool validMove = false;
                            for (const auto& s : mahdollisetSiirrot) {
                                if (s._a_r == valittuRivi && s._a_l == valittuLinja &&
                                    s._l_r == y && s._l_l == x) {
                                    valittuSiirto = s;
                                    // Tarkistetaan ylennystilanne (esim. sotilas pääsee viimeiselle riville)
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
        }

        // Piirretään shakkilauta ja nappulat
        window.clear();
        sf::Color darkBrown(139, 69, 19);
        sf::Color lightBrown(222, 184, 135);
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(x * tileSize + 50, y * tileSize + 50);
                tile.setFillColor(((x + y) % 2 == 0) ? lightBrown : darkBrown);
                window.draw(tile);

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
        // Piirretään vihreät pallot ainoastaan niille siirroille, jotka lähtevät valitusta ruudusta.
        if (siirtoValittu) {
            for (const auto& s : mahdollisetSiirrot) {
                if (s._a_r == valittuRivi && s._a_l == valittuLinja) {
                    sf::CircleShape circle(15);
                    circle.setFillColor(sf::Color(0, 255, 0, 150));
                    circle.setPosition(s._l_l * tileSize + 50 + 35, s._l_r * tileSize + 50 + 35);
                    window.draw(circle);
                }
            }
        }
        window.draw(text);
        window.draw(text2);
        window.display();

        // Jos ei ole pelaajan vuoro, AI laskee siirtonsa minimaxilla
        if (asema._siirtovuoro != playerColor) {
            vector<Siirto> legalMoves;
            asema.anna_siirrot(legalMoves);
            if (!legalMoves.empty()) {
                MinimaxArvo bestMove = asema.minimaxAlphaBetaAsync(
                    4,
                    -std::numeric_limits<float>::infinity(),
                    std::numeric_limits<float>::infinity(),
                    true
                );
                int fromRow = bestMove._siirto._a_r;
                int fromCol = bestMove._siirto._a_l;
                // Tarkistetaan ylennystilanne
                if ((asema._siirtovuoro == VALKEA && bestMove._siirto._l_r == 0 && asema._lauta[fromRow][fromCol] == wP) ||
                    (asema._siirtovuoro == MUSTA && bestMove._siirto._l_r == 7 && asema._lauta[fromRow][fromCol] == bP))
                {
                    bestMove._siirto._korotettava_nappula = (asema._siirtovuoro == VALKEA ? wQ : bQ);
                }
                asema.tee_siirto(bestMove._siirto, asema._siirtovuoro);
                sf::sleep(sf::seconds(0.5));
            }
        }
    }
}

// Terminaalikäyttöliittymä: Pelaaja syöttää siirtonsa manuaalisesti, kunnes on AI:n vuoro.
void terminal_ui(Asema& asema, int playerColor) {
    while (true) {
        asema.tulosta();
        vector<Siirto> legalMoves;
        asema.anna_siirrot(legalMoves);
        if (legalMoves.empty())
            break;

        if (asema._siirtovuoro == playerColor) {
            int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;
            Siirto kayttajan_siirto;
            while (true) {
                cout << "Syötä siirto muodossa: lahto_rivi lahto_linja kohde_rivi kohde_linja" << endl;
                asema.kysy_siirto(asema._siirtovuoro, lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);
                kayttajan_siirto = Siirto(lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);
                bool laillinen_siirto = false;
                for (const auto& s : legalMoves) {
                    if (s == kayttajan_siirto) {
                        laillinen_siirto = true;
                        break;
                    }
                }
                if (laillinen_siirto)
                    break;
                else
                    cout << "Ei laillinen siirto! Yritä uudelleen." << endl;
            }
            // Tarkistetaan ylennystilanne
            if ((asema._siirtovuoro == VALKEA && kohde_rivi == 0 && asema._lauta[lahto_rivi][lahto_linja] == wP) ||
                (asema._siirtovuoro == MUSTA && kohde_rivi == 7 && asema._lauta[lahto_rivi][lahto_linja] == bP))
            {
                kayttajan_siirto._korotettava_nappula = (asema._siirtovuoro == VALKEA ? wQ : bQ);
            }
            asema.tee_siirto(kayttajan_siirto, asema._siirtovuoro);
        }
        else {  // AI:n vuoro
            MinimaxArvo bestMove = asema.minimaxAlphaBetaAsync(
                4,
                -std::numeric_limits<float>::infinity(),
                std::numeric_limits<float>::infinity(),
                true
            );
            cout << "Minimax Arvo: " << bestMove._arvo << endl;
            cout << "Paras siirto: Lahto: "
                << static_cast<char>('a' + bestMove._siirto._a_l)
                << static_cast<char>('1' + (7 - bestMove._siirto._a_r))
                << " -> Kohde: "
                << static_cast<char>('a' + bestMove._siirto._l_l)
                << static_cast<char>('1' + (7 - bestMove._siirto._l_r))
                << endl;
            int fromRow = bestMove._siirto._a_r;
            int fromCol = bestMove._siirto._a_l;
            if ((asema._siirtovuoro == VALKEA && bestMove._siirto._l_r == 0 && asema._lauta[fromRow][fromCol] == wP) ||
                (asema._siirtovuoro == MUSTA && bestMove._siirto._l_r == 7 && asema._lauta[fromRow][fromCol] == bP))
            {
                bestMove._siirto._korotettava_nappula = (asema._siirtovuoro == VALKEA ? wQ : bQ);
            }
            asema.tee_siirto(bestMove._siirto, asema._siirtovuoro);
        }
    }
}
