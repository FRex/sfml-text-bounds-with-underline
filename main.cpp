#include <SFML/Graphics.hpp>
#include <sstream>
#include "Text2.hpp"

template <typename T>
static sf::RectangleShape shapeFromLocalBoundingBox(const T& txt)
{
    const auto bounds = txt.getLocalBounds();
    sf::RectangleShape ret;
    ret.setFillColor(sf::Color::Red);
    ret.setPosition(sf::Vector2f(bounds.left, bounds.top));
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
static bool g_underline = true;
static bool g_italize = false;
static bool g_bold = false;
static bool g_strike = true;
static int g_outlinesize = 0;

template <typename T>
static void doit(sf::RenderWindow& app, const sf::Font& font, sf::Transform transform)
{
    T txt(font, g_string, 100u);
    unsigned styles = 0u;

    if(g_underline)
        styles |= T::Underlined;

    if(g_italize)
        styles |= T::Italic;

    if(g_strike)
        styles |= T::StrikeThrough;

    if(g_bold)
        styles |= T::Bold;

    if(g_outlinesize)
    {
        txt.setOutlineThickness((float)g_outlinesize);
        txt.setOutlineColor(sf::Color::Black);
    }

    txt.setStyle(styles);
    app.draw(shapeFromLocalBoundingBoxAt00(txt), transform);
    app.draw(shapeFromLocalBoundingBox(txt), transform);
    app.draw(txt, transform);
}

static void handleKeyPressed(const sf::Event& eve)
{
    if(eve.key.code == sf::Keyboard::Enter)
        g_string += sf::String('\n');

    if(eve.key.code == sf::Keyboard::I && eve.key.control)
        g_italize = !g_italize;

    if(eve.key.code == sf::Keyboard::U && eve.key.control)
        g_underline = !g_underline;

    if(eve.key.code == sf::Keyboard::S && eve.key.control)
        g_strike = !g_strike;

    if(eve.key.code == sf::Keyboard::B && eve.key.control)
        g_bold = !g_bold;

    if(eve.key.code == sf::Keyboard::Up)
        ++g_outlinesize;

    if(eve.key.code == sf::Keyboard::Down)
        --g_outlinesize;
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
                g_string += sf::String((char32_t)eve.text.unicode);
        }

        if(eve.type == sf::Event::KeyPressed)
            handleKeyPressed(eve);

        if(eve.type == sf::Event::Resized)
            app.setView(sf::View(sf::FloatRect(sf::Vector2f(), sf::Vector2f(eve.size.width, eve.size.height))));
    } // while app poll event eve
}

static std::string makeInfoString()
{
    std::ostringstream ss;
    ss << std::boolalpha;

    ss << "Length: " << g_string.getSize() << std::endl;

    ss << "Content:";
    for(int i = 0; i < (int)g_string.getSize(); ++i)
        ss << ' ' << (unsigned)g_string[i];
    ss << std::endl;

    ss << "Styles (Ctrl + first letter of style name to toggle):" << std::endl;
    ss << "Bold: " << g_bold << std::endl;
    ss << "Underline: " << g_underline << std::endl;
    ss << "Italic: " << g_italize << std::endl;
    ss << "Strikethrough: " << g_strike << std::endl;

    ss << "Outline (Up/Down Arrows): " << g_outlinesize;
    if(g_outlinesize == 0)
        ss << " (off)";
    ss << std::endl;

    return ss.str();
}

int main(int argc, char ** argv)
{
    const char * fontname = "DejaVuSans.ttf";
    if(argc > 1)
        fontname = argv[1];

    sf::RenderWindow app(sf::VideoMode(sf::Vector2u(640u, 480u)), fontname);
    sf::Font font;
    if(!font.loadFromFile(fontname))
        return 1;

    if(argc > 2)
        g_string = argv[2];

    while(app.isOpen())
    {
        handleEvents(app);

        app.clear(sf::Color(0x3f3f3fff));
        app.draw(sf::Text(font, makeInfoString(), 16u));

        sf::Transformable transformable;
        transformable.move(sf::Vector2f(200.f, 150.f));
        doit<sf::Text>(app, font, transformable.getTransform());
        transformable.move(sf::Vector2f(0.f, 500.f));
        doit<sf::Text2>(app, font, transformable.getTransform());

        app.display();
    }
}
