#include <bits/stdc++.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

sf::RenderWindow win;
sf::Event event;
sf::RectangleShape p1, p2;

double coe[20][20];
std::vector<std::pair<double, double>> ls;
sf::VertexArray va(sf::LineStrip, 1000);

bool sel;
int mov = -1;
sf::Cursor c1, c2;

int main() {
    for (int i = 0; i < 20; ++i)
        coe[i][0] = coe[i][i] = 1;
    for (int i = 2; i < 20; ++i)
        for (int j = 1; j < i; ++j)
            coe[i][j] = coe[i - 1][j] + coe[i - 1][j - 1];

    win.create(sf::VideoMode(1270, 960), "Bezier", sf::Style::Close | sf::Style::Titlebar);
    win.setFramerateLimit(60);

    p1.setFillColor(sf::Color::Transparent);
    p1.setOutlineColor(sf::Color::White);
    p1.setOutlineThickness(1);
    p1.setSize(sf::Vector2f(11, 11));
    p2.setFillColor(sf::Color::White);
    p2.setSize(sf::Vector2f(5, 5));

    c1.loadFromSystem(sf::Cursor::Arrow);
    c2.loadFromSystem(sf::Cursor::Hand);

    while (win.isOpen()) {
        size_t now = ~0ull;
        auto pos = sf::Mouse::getPosition(win);
        for (size_t i = 0; !sel && i < ls.size(); ++i)
            if (hypot(ls[i].first - pos.x, ls[i].second - pos.y) < 8) {
                now = i;
                break;
            }
        if (~now || sel)
            win.setMouseCursor(c2);
        else
            win.setMouseCursor(c1);

        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                win.close();
                break;
            }
            switch (event.type) {
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Left && !sel && (~now)) {
                        sel = true;
                        mov = now;
                    }
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    if (event.mouseButton.button == sf::Mouse::Left && sel) {
                        sel = false;
                        mov = -1;
                        break;
                    }
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        if (~now)
                            ls.erase(ls.begin() + now);
                        else 
                            ls.emplace_back(event.mouseButton.x, event.mouseButton.y);
                    }
                    break;
                }
                case sf::Event::MouseMoved: {
                    if (sel)
                        ls[mov] = {pos.x, pos.y};
                    break;
                }
                default:
                    break;
            }
        }

        win.clear();

        if (ls.size() > 1) {
            sf::VertexArray tva(sf::LineStrip, ls.size());
            for (size_t i = 0; i < ls.size(); ++i)
                tva[i] = sf::Vertex(sf::Vector2f(ls[i].first, ls[i].second), sf::Color::Yellow);
            win.draw(tva);
        }

        for (size_t i = 0; i < ls.size(); ++i) {
            if (i == now) {
                p1.setOutlineColor(sf::Color::Red);
                p2.setFillColor(sf::Color::Red);
            }
            p1.setPosition(ls[i].first - 5, ls[i].second - 5);
            p2.setPosition(ls[i].first - 2, ls[i].second - 2);
            win.draw(p1);
            win.draw(p2);
            if (i == now) {
                p1.setOutlineColor(sf::Color::White);
                p2.setFillColor(sf::Color::White);
            }
        }

        if (ls.size() > 1) {
            for (size_t i = 0; i < 1000; ++i) {
                double x = 0, y = 0;
                for (size_t j = 0; j < ls.size(); ++j) {
                    double t1 = pow(i / 1000.0, j), t2 = pow(1 - i / 1000.0, ls.size() - j - 1);
                    x += t1 * t2 * coe[ls.size() - 1][j] * ls[j].first;
                    y += t1 * t2 * coe[ls.size() - 1][j] * ls[j].second;
                }
                va[i].position = sf::Vector2f(x, y);
            }
            win.draw(va);
        }

        win.display();
    }
    return 0;
}
