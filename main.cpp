#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow app(sf::VideoMode(640u, 480u), "Test");

    sf::Font font;
    font.loadFromFile("DejaVuSans.ttf");

    sf::Text txt("Test", font, 200);
    txt.setStyle(sf::Text::Underlined);
    const auto bounds = txt.getLocalBounds();

    sf::RectangleShape sha;
    sha.setFillColor(sf::Color::Red);
    sha.setPosition(bounds.left, bounds.top);
    sha.setSize(sf::Vector2f(bounds.width, bounds.height));

    txt.move(100.f, 100.f);
    sha.move(100.f, 100.f);

    while(app.isOpen())
    {
        sf::Event eve;
        while(app.pollEvent(eve))
            if(eve.type == sf::Event::Closed)
                app.close();

        app.clear();
        app.draw(sha);
        app.draw(txt);
        app.display();
    }
}
