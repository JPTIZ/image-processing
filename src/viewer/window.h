#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace viewer {

class Window {
public:
    Window(std::string title, int width, int height);

    void show();
    void on_event(sf::Event const& event);

private:
    sf::RenderWindow _sf_window;
};

}
