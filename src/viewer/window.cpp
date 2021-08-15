#include "window.h"

#include <optional>
#include <iostream>

#include <logger.h>

#include "errors.h"

namespace {

sf::RenderWindow make_sf_window(std::string title, int width, int height) {
    auto video_mode = sf::VideoMode{
        static_cast<unsigned>(width),
        static_cast<unsigned>(height)
    };

    return sf::RenderWindow{video_mode, title};
}

auto setup_window(sf::RenderWindow& window) {
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.setActive();
    window.setFramerateLimit(60);

    auto [width, height] = window.getSize();

    window.setView(sf::View{sf::FloatRect{
        0.f, 0.f,
        static_cast<float>(width),
        static_cast<float>(height)
    }});
}

auto next_event(sf::RenderWindow& window) {
    auto event = sf::Event{};
    return window.pollEvent(event)
        ? std::optional{event}
        : std::nullopt;
}

auto make_font() {
    auto filename = std::filesystem::path("FiraSans-Book.ttf");
    auto font = sf::Font{};
    if (not font.loadFromFile(filename)) {
        tizip::errors::fail(fmt::format("Failed to load font {}", filename.string()));
    }
    return font;
}

auto make_sf_texture(tizip::Image const& image) -> sf::Texture {
    auto texture = sf::Texture{};
    texture.create(image.width(), image.height());
    texture.update(image.data().data());
    return texture;
}

}

namespace tizip::viewer {

Window::Window(std::string title, int width, int height):
    _sf_window{make_sf_window(title, width, height)},
    _font{make_font()},
    _info{"Hello!", _font},
    _image{0, 0}
{}


void Window::show() {
    auto& window = _sf_window;

    setup_window(window);

    _info.setFillColor(sf::Color{255, 0, 0});
    _info.setCharacterSize(32);

    while (window.isOpen()) {
        while (auto event = next_event(window)) {
            on_event(*event);
        }

        window.clear();
        window.draw(_sf_image);
        window.draw(_info);
        window.display();
    }
}


void Window::on_event(sf::Event const& e) {
    using tizip::logger::log;
    using tizip::logger::debug;

    auto callbacks = std::unordered_map<sf::Event::EventType, std::function<void(sf::Event const&)>>{
        {sf::Event::Closed, [this](auto const& _) {
            log("Closing application...");
            _sf_window.close();
            log("Closed");
        }},
        {sf::Event::KeyPressed, [this](auto const& e)  {
            debug(fmt::format("Pressed key {}", e.key.code));
        }},
    };

    if (callbacks.contains(e.type)) {
        auto callback = callbacks.at(e.type);
        callback(e);
    }
}

void Window::set_image(std::filesystem::path const& path) {
    auto image = load_image(path);
    _sf_texture = make_sf_texture(image);
    _sf_image.setTexture(_sf_texture);

    auto ratio = [](auto v1, auto v2) {
        auto v1_f = sf::Vector2<float>{v1};
        auto v2_f = sf::Vector2<float>{v2};
        return sf::Vector2{
            v1_f.x / v2_f.y,
            v1_f.y / v2_f.y,
        };
    };

    _sf_image.setScale(ratio(_sf_window.getSize(), _sf_texture.getSize()));
}

void Window::set_processing_function(std::string const& name, ProcessingFunction f) {
    _info.setString(name);
    process = f;
}

}
