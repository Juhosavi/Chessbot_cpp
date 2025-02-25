#include <iostream> // Sis‰llytet‰‰n iostream-kirjasto, joka mahdollistaa syˆtteen ja tulostuksen
#include <vector> // Sis‰llytet‰‰n vector-kirjasto, joka mahdollistaa vektorien k‰ytˆn
#include <thread> // Sis‰llytet‰‰n thread-kirjasto, joka mahdollistaa s‰ikeiden k‰ytˆn
#include <map> // Sis‰llytet‰‰n map-kirjasto, joka mahdollistaa assosiatiivisten konttien k‰ytˆn
#include <SFML/Graphics.hpp> // Sis‰llytet‰‰n SFML-grafiikkakirjasto
#include "asema.h" // Sis‰llytet‰‰n asema.h-tiedosto, joka sis‰lt‰‰ Asema-luokan m‰‰rittelyn
#include "siirto.h" // Sis‰llytet‰‰n siirto.h-tiedosto, joka sis‰lt‰‰ Siirto-luokan m‰‰rittelyn
#include "shakki.h" // Sis‰llytet‰‰n shakki.h-tiedosto, joka sis‰lt‰‰ shakkiin liittyvi‰ m‰‰rittelyj‰

using namespace std;

// Shakkilaudan ruudun koko
const int tileSize = 100;

// Tekstuurit kaikille nappuloille
map<string, sf::Texture> textures;

// Lataa kaikki shakkipelin nappulat
void lataaTekstuurit() {
    // Lista kaikista nappuloista
    vector<string> pieces = {
        "black-bishop", "black-king", "black-knight", "black-pawn",
        "black-queen", "black-rook", "white-bishop", "white-king",
        "white-knight", "white-pawn", "white-queen", "white-rook"
    };

    // Ladataan tekstuurit kaikille nappuloille
    for (const string& piece : pieces) {
        sf::Texture texture;
        if (!texture.loadFromFile("C:/Users/savin/source/repos/Chessbot/pieces/" + piece + ".png")) {
            cerr << "Virhe ladattaessa tekstuuria: " << piece << endl;
        }
        textures[piece] = texture;
    }
}

// SFML-graafinen k‰yttˆliittym‰
void sfml_gui(Asema& asema)
{
    sf::Font font;
    if (!font.loadFromFile("C:/Users/savin/source/repos/Chessbot/font/AldotheApache.ttf")) {
        cerr << "Virhe ladattaessa fonttia!" << endl;
    }
    sf::Text text; // Teksti-olio
    sf::Text text2; // Toinen teksti-olio
    text.setFont(font); // Asetetaan fontti tekstille
    text2.setFont(font); // Asetetaan fontti toiselle tekstille
    text.setString("         A        B        C        D        E        F        G        H"); // Asetetaan tekstin sis‰ltˆ
    text2.setString("\n 8 \n\n 7 \n\n 6 \n\n 5 \n\n 4 \n\n 3 \n\n 2 \n\n 1"); // Asetetaan toisen tekstin sis‰ltˆ
    sf::Color darkBrown(139, 69, 19);  // Tummanruskea v‰ri
    sf::Color lightBrown(222, 184, 135);  // Vaaleanruskea v‰ri

    text.setCharacterSize(45); // Asetetaan tekstin koko
    text2.setCharacterSize(60); // Asetetaan toisen tekstin koko
    text.setFillColor(sf::Color::White); // Asetetaan tekstin v‰ri
    sf::RenderWindow window(sf::VideoMode(900, 900), "SFML Chessboard"); // Luodaan ikkuna
    lataaTekstuurit(); // Ladataan nappuloiden tekstuurit
    map<int, string> pieceMap = { // Kartta, joka yhdist‰‰ nappulat niiden nimiin
        {wP, "white-pawn"}, {wR, "white-rook"}, {wN, "white-knight"},
        {wB, "white-bishop"}, {wQ, "white-queen"}, {wK, "white-king"},
        {bP, "black-pawn"}, {bR, "black-rook"}, {bN, "black-knight"},
        {bB, "black-bishop"}, {bQ, "black-queen"}, {bK, "black-king"}
    };

    vector<Siirto> mahdollisetSiirrot; // Vektori mahdollisille siirroille
    int valittuRivi = -1, valittuLinja = -1; // Valitun ruudun koordinaatit
    bool siirtoValittu = false; // Onko siirto valittu
    Siirto valittuSiirto; // Valittu siirto

    while (window.isOpen()) // P‰‰silmukka, joka pyˆrii niin kauan kuin ikkuna on auki
    {
        sf::Event event; // Tapahtuma-olio
        while (window.pollEvent(event)) // K‰yd‰‰n l‰pi kaikki tapahtumat
        {
            if (event.type == sf::Event::Closed) // Jos ikkuna suljetaan
                window.close(); // Suljetaan ikkuna
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) // Jos hiiren vasen nappi painetaan
            {
                int x = (event.mouseButton.x - 50) / tileSize; // Lasketaan x-koordinaatti
                int y = (event.mouseButton.y - 50) / tileSize; // Lasketaan y-koordinaatti
                if (x >= 0 && x < 8 && y >= 0 && y < 8) // Jos koordinaatit ovat laudan sis‰ll‰
                {
                    if (!siirtoValittu) { // Jos siirtoa ei ole valittu
                        valittuRivi = y; // Tallennetaan valitun ruudun rivi
                        valittuLinja = x; // Tallennetaan valitun ruudun linja
                        mahdollisetSiirrot.clear(); // Tyhjennet‰‰n mahdolliset siirrot
                        asema.anna_siirrot(mahdollisetSiirrot); // Haetaan mahdolliset siirrot
                        siirtoValittu = true; // Merkit‰‰n siirto valituksi
                    }
                    else { // Jos siirto on valittu
                        bool validMove = false; // Onko siirto laillinen
                        for (const auto& siirto : mahdollisetSiirrot) { // K‰yd‰‰n l‰pi mahdolliset siirrot
                            if (siirto._a_r == valittuRivi && siirto._a_l == valittuLinja && siirto._l_r == y && siirto._l_l == x) { // Jos siirto on laillinen
                                valittuSiirto = siirto; // Tallennetaan valittu siirto
                                asema.tee_siirto(valittuSiirto, asema._siirtovuoro); // Suoritetaan siirto
                                validMove = true; // Merkit‰‰n siirto lailliseksi
                                break; // Lopetetaan silmukka
                            }
                        }
                        if (!validMove) { // Jos siirto ei ollut laillinen
                            siirtoValittu = false; // Nollataan valinta
                        }
                    }
                }
            }
        }

        window.clear(); // Tyhjennet‰‰n ikkuna

        // Piirret‰‰n shakkilauta
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize)); // Luodaan ruutu
                tile.setPosition(x * tileSize + 50, y * tileSize + 50); // Asetetaan ruudun sijainti

                // Shakkilaudan ruutujen v‰rit
                if ((x + y) % 2 == 0)
                    tile.setFillColor(lightBrown); // Tummanruskea ruutu
                else
                    tile.setFillColor(darkBrown); // Vaaleanruskea ruutu

                window.draw(tile); // Piirret‰‰n ruutu

                // Piirret‰‰n nappulat
                int piece = asema._lauta[y][x]; // Haetaan nappula ruudusta
                if (pieceMap.find(piece) != pieceMap.end()) { // Jos nappula lˆytyy kartasta
                    sf::Sprite sprite; // Luodaan sprite
                    sprite.setTexture(textures[pieceMap[piece]]); // Asetetaan tekstuuri
                    sprite.setOrigin(-35, -35); // Asetetaan alkuper‰
                    sprite.setPosition(x * tileSize, y * tileSize); // Asetetaan sijainti
                    window.draw(sprite); // Piirret‰‰n sprite
                }
            }
        }

        // Piirret‰‰n mahdolliset siirrot
        for (const auto& siirto : mahdollisetSiirrot) {
            if (siirto._a_r == valittuRivi && siirto._a_l == valittuLinja) {
                sf::CircleShape circle(15); // Luodaan ympyr‰
                circle.setFillColor(sf::Color(0, 255, 0, 150)); // Asetetaan ympyr‰n v‰ri
                circle.setPosition(siirto._l_l * tileSize + 50 + 35, siirto._l_r * tileSize + 50 + 35); // Asetetaan ympyr‰n sijainti
                window.draw(circle); // Piirret‰‰n ympyr‰
            }
        }

        // Piirret‰‰n teksti, joka n‰ytt‰‰ shakkilaudan sarakkeiden kirjaimet (A-H)
        window.draw(text);

        // Piirret‰‰n toinen teksti, joka n‰ytt‰‰ shakkilaudan rivien numerot (1-8)
        window.draw(text2);

        // P‰ivitet‰‰n ikkuna ja n‰ytet‰‰n kaikki piirretyt elementit
        window.display();
    }
}

// Tekstipohjainen shakkipeli komentorivill‰
void terminal_ui(Asema& asema) {
    vector<Siirto> siirrot;
    while (true) {

        asema.tulosta();
        MinimaxArvo minimaxTulos = asema.minimax(3);
        std::cout << "Minimax Arvo: " << minimaxTulos._arvo << std::endl;
        std::cout << "Paras siirto: "
            << "Lahto: "
            << static_cast<char>('a' + minimaxTulos._siirto._a_l)
            << static_cast<char>('1' + (7 - minimaxTulos._siirto._a_r))
            << " -> Kohde: "
            << static_cast<char>('a' + minimaxTulos._siirto._l_l)
            << static_cast<char>('1' + (7 - minimaxTulos._siirto._l_r))
            << std::endl;

        siirrot.clear();
        asema.anna_siirrot(siirrot);
      

        cout << "Siirtoja: " << siirrot.size() << endl;

        int lahto_rivi, lahto_linja, kohde_rivi, kohde_linja;
        Siirto kayttajan_siirto;

        while (true) {
            asema.kysy_siirto(asema._siirtovuoro, lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

            kayttajan_siirto = Siirto(lahto_rivi, lahto_linja, kohde_rivi, kohde_linja);

            bool laillinen_siirto = false; // Muuttujan tyyppi lis‰tty ja alustettu
            for (const auto& s : siirrot) {
                if (s == kayttajan_siirto) {
                    laillinen_siirto = true; // Korjattu: = eik‰ ()
                    break;
                }
            }

            if (laillinen_siirto) { // Korjattu: if eik‰ else:
                break;
            }
            else {
                cout << "Ei laillinen siirto! Yrit‰ uudelleen." << endl;
            }
        }

        asema.tee_siirto(kayttajan_siirto, asema._siirtovuoro);
        // asema.anna_siirrot(siirrot); // T‰m‰n rivin voi poistaa, koska siirrot on jo haettu
    }
}

int main() {
    Asema asema; // Luodaan Asema-olio
    asema.tyhjenna();
    asema._lauta[5][6] = bK;
    asema._lauta[7][7] = wK;
    asema._lauta[6][5] = bQ;

    // Luodaan s‰ie SFML-grafiikalle
    //thread t1(sfml_gui, ref(asema));
    thread t2(terminal_ui, ref(asema));

    // Odotetaan, ett‰ s‰ie p‰‰ttyy
    //t1.join();
    t2.join();

    return 0;
}