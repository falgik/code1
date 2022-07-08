#include <math.h>
#include "SFML/Graphics.hpp"
#include "Button.h"

using namespace sf;



class Prog
{
public:  

    void init(Vector2u wndsize, Font& font)
    {
        btns.emplace_back(Button(wndsize.x - 250, 0, 250, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"Маштаб +"));
        btns.emplace_back(Button(wndsize.x - 250, 40, 250, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"Маштаб -"));
        btns.emplace_back(Button(wndsize.x - 250, 80, 250, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"метод прямокутників"));
        btns.emplace_back(Button(wndsize.x - 250, 120, 250, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"метод трапецій"));
        btns.emplace_back(Button(wndsize.x - 250, 160, 125, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"N--"));
        btns.emplace_back(Button(wndsize.x - 125, 160, 125, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"N++"));
        btns.emplace_back(Button(wndsize.x - 250, 200, 125, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"a -= 0.1"));
        btns.emplace_back(Button(wndsize.x - 125, 200, 125, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"a += 0.1"));
        btns.emplace_back(Button(wndsize.x - 250, 240, 125, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"b -= 0.1"));
        btns.emplace_back(Button(wndsize.x - 125, 240, 125, 40, Color(49, 53, 231), Color(52, 85, 232), Color(52, 98, 232), font, L"b += 0.1"));
        btns.emplace_back(Button(wndsize.x - 250, 280, 250, 40, Color(52, 85, 232), Color(52, 85, 232), Color(52, 85, 232), font, L""));
        btns.emplace_back(Button(wndsize.x - 250, 320, 250, 40, Color(52, 85, 232), Color(52, 85, 232), Color(52, 85, 232), font, L"S = 0"));

        numbers.setCharacterSize(12);
        numbers.setFont(font);
        numbers.setFillColor(Color(49, 193, 231));
        wsize = (Vector2f)wndsize;

        a = -3;
        b = 3;
        n = 100;

        scalex = wsize.x / n;
        scaley = wsize.x / (b - a);
        steep = (b - a) / n;
        scale = 1;

        method = Method::NONE;

        coordLines.resize(4);
        coordLines.setPrimitiveType(Lines);
        for (int i = 0; i < 4; coordLines[i++].color = Color::Blue);
        coordLines[0].position = { 0, wsize.y / 2 };
        coordLines[1].position = { wsize.x , wsize.y / 2 };
        coordLines[2].position = { scaley * -a, 0 };
        coordLines[3].position = { scaley * -a, wsize.y };

        plot.setPrimitiveType(LinesStrip);
        rect.setFillColor(Color::Transparent);
        rect.setOutlineColor(Color(49, 255, 128));
        rect.setOutlineThickness(1);
    }

    void update(Vector2f pos, bool click)
    {
        for (int i = 0; i < btns.size(); i++)
            btns[i].update(pos, click);

        for (int i = 0; i < btns.size(); i++)
        {
            btns[i].update(pos, click);
            if (btns[i].isClick())
            {
                switch (i)
                {
                case 0:
                    scale += 0.1;
                    break;

                case 1:
                    scale -= 0.1;
                    break;

                case 2:
                    method = Method::RECT;
                    break;

                case 3:
                    method = Method::TRAP;
                    break;

                case 4:
                    if (--n < 1)
                        n = 1;
                    break;
                case 5:
                    n++;
                    break;
                case 6:
                    a -= 0.1;
                    break;
                case 7:
                    a += 0.1;
                    break;
                case 8:
                    b -= 0.1;
                    break;
                case 9:
                    b += 0.1;
                    break;
                }

                scale = std::max(scale, 0.3f);
                scale = std::min(scale, 3.f);

                scalex = wsize.x / n * scale;
                scaley = wsize.x / (b - a) * scale;
                steep = (b - a) / n;
            }
        }



        if (Keyboard::isKeyPressed(Keyboard::W))
            offset.y += moveTimer.getElapsedTime().asMilliseconds() / 2;
        else if (Keyboard::isKeyPressed(Keyboard::S))
            offset.y -= moveTimer.getElapsedTime().asMilliseconds() / 2;
        else if (Keyboard::isKeyPressed(Keyboard::D))
            offset.x -= moveTimer.getElapsedTime().asMilliseconds() / 2;
        else if (Keyboard::isKeyPressed(Keyboard::A))
            offset.x += moveTimer.getElapsedTime().asMilliseconds() / 2;

        moveTimer.restart();

        coordLines[0].position = { 0, wsize.y / 2 + offset.y };
        coordLines[1].position = { wsize.x , wsize.y / 2 + offset.y };
        coordLines[2].position = { scaley * -a + offset.x, 0 };
        coordLines[3].position = { scaley * -a + offset.x, wsize.y };


        plot.resize(n + 1);
        float val = a;
        for (int i = 0; i < n + 1; i++, val += steep)
        {
            plot[i].position.x = i * scalex;
            plot[i].position.y = wsize.y / 2 - (func(val) * scaley);
            plot[i].color = Color::Red;
            plot[i].position += offset;
        }

        float res = 0;

        switch (method)
        {
        case Method::TRAP:
        {
            float val = a;

            for (int i = 0; i < n; i++, val += steep)
            {
                res += steep * (abs(func(val)) + abs(func(val + steep))) / 2;
            }

            break;
        }

        case Method::RECT:
        {
            float val = a;

            for (int i = 0; i < n; i++, val += steep)
            {
                res += steep * abs(func(val));
            }

            break;
        }

        }

        static std::string s, s1, s2;

        s = std::to_string(a);
        if (a < 0)
            s.resize(5);
        else
            s.resize(4);


        s1 = std::to_string(b);
        if (b < 0)
            s1.resize(5);
        else
            s1.resize(4);
        s2 = std::to_string((int)n);

        btns[btns.size() - 2].setString("a = " + s + ", b = " + s1);

        btns[btns.size() - 1].setString("N = " + s2 + ", S = " + std::to_string(res));

    }

    void draw(RenderWindow& window)
    {
        window.draw(coordLines);
        window.draw(plot);



        switch (method)
        {
        case Method::TRAP:
        {
            float val = a;
            static Vector2f p1, p2, p3;
            for (int i = 0; i < n; i++, val += steep)
            {
                p1 = { i * scalex, wsize.y / 2 - (func(val) * scaley) };

                p2 = { (i + 1) * scalex, wsize.y / 2 - (func(val + steep) * scaley) };

                p3 = { (i + 1) * scalex, wsize.y / 2 };

                drawTrap(p1, p2, p3, window);
            }
        }
        break;

        case Method::RECT:
        {
            float val = a;
            for (int i = 0; i < n; i++, val += steep)
            {

                rect.setPosition(i * scalex, wsize.y / 2 - (func(val) * scaley));
                rect.setSize(Vector2f(scalex, func(val) * scaley));
                rect.move(offset);
                window.draw(rect);
            }
        }
        break;

        }

        for (int i = 0; i < 100; i++)
        {
            numbers.setPosition(coordLines[2].position.x + i * scaley, coordLines[0].position.y);
            numbers.setString(std::to_string(i));
            window.draw(numbers);
        }

        for (int i = -1; i > -100; i--)
        {
            numbers.setPosition(coordLines[2].position.x + i * scaley, coordLines[0].position.y);
            numbers.setString(std::to_string(i));
            window.draw(numbers);
        }

        for (int i = 1; i < 50; i++)
        {
            numbers.setPosition(coordLines[2].position.x, coordLines[0].position.y - i * scaley);
            numbers.setString(std::to_string(i));
            window.draw(numbers);
        }

        for (int i = 1; i < 50; i++)
        {
            numbers.setPosition(coordLines[2].position.x, coordLines[0].position.y + i * scaley);
            numbers.setString(std::to_string(-i));
            window.draw(numbers);
        }


        for (int i = 0; i < btns.size(); i++)
            btns[i].draw(window);
    }

private:

    enum class Method {
        NONE,
        TRAP,
        RECT
    };

    float func(float x) // функція для графіка
    {
      return sin(2 * x) + cos(3 * x);
      // return pow(x, 3) + 3 * pow(x, 2) + 6 * x - 1;
    }

    void drawTrap(Vector2f p1, Vector2f p2, Vector2f p3, RenderWindow& window)
    {
        VertexArray tr(LineStrip, 5);

        for (int i = 0; i < 5; i++)
        {
            tr[i].color = Color(49, 255, 128);
        }

        tr[0].position = p1 + offset;
        tr[1].position = p2 + offset;
        tr[2].position = p3 + offset;
        tr[3].position = Vector2f(p1.x, p3.y) + offset;
        tr[4].position = p1 + offset;

        window.draw(tr);
    }

    VertexArray coordLines;
    VertexArray plot;
    Vector2f wsize;
    Vector2f offset;
    float a, b, n, scalex, scaley, scale, steep;
    Text numbers;
    Clock moveTimer;
    RectangleShape rect;
    std::vector<Button> btns;
    Method method;
};


Prog prog;

int main()
{
    Font font;
    font.loadFromFile("consolab.ttf");

    RenderWindow window(VideoMode(1366, 768), L"лаба_1", Style::Default);
    window.setFramerateLimit(60);
    Event ev;


    prog.init(window.getSize(), font);

    while (window.isOpen())
    {
        while (window.pollEvent(ev))
        {
            switch (ev.type)
            {
            case Event::Closed:
                window.close();
                break;
            }
        }

        window.clear();

        prog.update(Vector2f(Mouse::getPosition(window)), Mouse::isButtonPressed(Mouse::Left));
        prog.draw(window);

        window.display();
    }

    return 0;
}