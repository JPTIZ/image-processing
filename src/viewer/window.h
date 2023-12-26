#pragma once

#include <functional>
#include <filesystem>
#include <string>

#include <SFML/Graphics.hpp>

#include "image.h"

namespace tizip::viewer {

class Window {
public:
    using ProcessingFunction = std::function<Image(Image const&)>;

    Window(std::string title, int width, int height);

    void show();
    void on_event(sf::Event const& event);
    void set_image(std::filesystem::path const&);
    void set_processing_function(std::string const& name, ProcessingFunction f);

private:
    sf::RenderWindow _sf_window;
    sf::Font _font;
    sf::Text _info;
    sf::Texture _sf_texture;
    sf::Sprite _sf_image;
    Image _image;
    ProcessingFunction process;
};

}
