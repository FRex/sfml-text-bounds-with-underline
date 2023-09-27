#include <SFML/Graphics.hpp>
#include <sstream>

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

template <typename T>
static sf::RectangleShape shapeFromLocalBoundingBoxAt00(const T& txt)
{
    const auto bounds = txt.getLocalBounds();
    sf::RectangleShape ret;
    ret.setFillColor(sf::Color::Blue);
    ret.setSize(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height));
    return ret;
}

static sf::String g_string;

static void handleKeyPressed(const sf::Event& eve)
{
    if(eve.key.code == sf::Keyboard::Return)
        g_string += '\n';
}

static void handleEvents(sf::RenderWindow& app)
{
    sf::Event eve;
    while(app.pollEvent(eve))
    {
        if(eve.type == sf::Event::Closed)
            app.close();

        if(eve.type == sf::Event::TextEntered)
        {
            if(eve.text.unicode == 0x8 && !g_string.isEmpty()) // backspace
                g_string.erase(g_string.getSize() - 1);

            if(eve.text.unicode >= ' ') // ignore non text ascii
                g_string += eve.text.unicode;
        }

        if(eve.type == sf::Event::KeyPressed)
            handleKeyPressed(eve);

        if(eve.type == sf::Event::Resized)
            app.setView(sf::View(sf::FloatRect(0.f, 0.f, eve.size.width, eve.size.height)));
    } // while app poll event eve
}

static std::string makeInfoString()
{
    std::ostringstream ss;

    ss << "Length: " << g_string.getSize() << std::endl;

    ss << "Content:";
    for(int i = 0; i < (int)g_string.getSize(); ++i)
        ss << ' ' << g_string[i];
    ss << std::endl;

    return ss.str();
}

int main()
{
    sf::RenderWindow app(sf::VideoMode(640u, 480u), "Test");

    sf::Font font;
    font.loadFromFile("DejaVuSans.ttf");

    sf::Text infotext("", font, 30u);

    sf::Text txt1("", font, 200u);
    txt1.setStyle(sf::Text::Underlined);

    while(app.isOpen())
    {
        sf::Event eve;
        handleEvents(app);

        sf::Transformable transformable;
        transformable.move(100.f, 100.f);

        app.clear();

        infotext.setString(makeInfoString());
        app.draw(infotext);

        txt1.setString(g_string);
        app.draw(shapeFromLocalBoundingBoxAt00(txt1), transformable.getTransform());
        app.draw(shapeFromLocalBoundingBox(txt1), transformable.getTransform());
        app.draw(txt1, transformable.getTransform());

        app.display();
    }
}
