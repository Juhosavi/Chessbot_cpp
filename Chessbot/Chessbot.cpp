#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 650), "SFML works!");
    sf::RectangleShape shape(sf::Vector2f(100, 100));
    sf::RectangleShape shape2(sf::Vector2f(100, 100));
    sf::RectangleShape shape3(sf::Vector2f(100, 100));
    shape.setFillColor(sf::Color::White);
    shape2.setFillColor(sf::Color::Black);
    shape3.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        shape2.setPosition(100, 0);
        window.draw(shape2);
        shape3.setPosition(200, 0);
        window.draw(shape3);
        window.display();
    }

    return 0;
}