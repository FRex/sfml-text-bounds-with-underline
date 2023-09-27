#include <SFML/Graphics.hpp>

template <typename T>
static sf::RectangleShape shapeFromLocalBoundingBox(const T& txt)
{
    const auto bounds = txt.getLocalBounds();
    sf::RectangleShape ret;
    ret.setFillColor(sf::Color::Red);
    ret.setPosition(bounds.left, bounds.top);
    ret.setSize(sf::Vector2f(bounds.width, bounds.height));
    return ret;
}

int main()
{
    sf::RenderWindow app(sf::VideoMode(640u, 480u), "Test");

    sf::Font font;
    font.loadFromFile("DejaVuSans.ttf");

    sf::Text txt("Test", font, 200);
    txt.setStyle(sf::Text::Underlined);

    while(app.isOpen())
    {
        sf::Event eve;
        while(app.pollEvent(eve))
            if(eve.type == sf::Event::Closed)
                app.close();

        sf::Transformable transformable;
        transformable.move(100.f, 100.f);

        app.clear();
        app.draw(shapeFromLocalBoundingBox(txt), transformable.getTransform());
        app.draw(txt, transformable.getTransform());
        app.display();
    }
}
