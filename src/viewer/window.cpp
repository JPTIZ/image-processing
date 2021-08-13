#include "window.h"

#include <optional>

#include <logger.h>

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
        -15.f, 0.f,
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

}

namespace viewer {

Window::Window(std::string title, int width, int height):
    _sf_window{make_sf_window(title, width, height)}
{}


void Window::show() {
    auto& window = _sf_window;

    setup_window(window);

    while (window.isOpen()) {
        while (auto event = next_event(window)) {
            on_event(*event);
        }

        window.clear();
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

}
