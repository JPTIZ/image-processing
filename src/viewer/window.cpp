#include "window.h"

#include <optional>

#include <logger.h>

#include "errors.h"

namespace {

template <typename Container, typename K>
auto value_or(
    Container const& c,
    K const& key,
    typename Container::mapped_type default_value
) -> typename Container::mapped_type const {
    try {
        return c.at(key);
    } catch (std::out_of_range const& e) {
        return default_value;
    }
}

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


void Window::on_event([[maybe_unused]] sf::Event const& e) {
    using tizip::logger::log;
    using tizip::logger::debug;

    static auto current_function = 0;

    auto processing_functions = std::vector<std::tuple<std::string, ProcessingFunction>>{
        {"No Processing", [](auto i) { return i; }},
        {"Grayscale", [](Image const& img) {
            auto original = img.data();
            auto grayscaled = std::vector<uint8_t>(original.size());
            for (auto i = 0; i < std::ssize(grayscaled); i += 4) {
                auto avg = int{original[i]};
                for (auto sp = 1; sp < 3; ++sp) { // RGB, no A
                    avg += original[i + sp];
                }
                avg = avg / 3;
                for (auto sp = 0; sp < 3; ++sp) {
                    grayscaled[i + sp] = avg;
                }
                grayscaled[i + 3] = original[i + 3];
            }
            return Image(img.width(), img.height(), grayscaled.data());
        }},
    };

    auto select_processing_function = [=, this](auto index) {
        current_function = index % processing_functions.size();
        auto [name, f] = processing_functions[current_function];
        set_processing_function(name, f);
        _sf_texture = make_sf_texture(process(_image));
        _sf_image.setTexture(_sf_texture);
    };

    auto previous_processing_function = [=]() {
        select_processing_function(current_function - 1);
    };

    auto next_processing_function = [=]() {
        select_processing_function(current_function + 1);
    };

    using EventCallback = std::function<void(sf::Event const&)>;

    auto noop = EventCallback{[](auto&&...) {}};
    auto callbacks = std::unordered_map<sf::Event::EventType, EventCallback>{
        {sf::Event::Closed, [this](auto const&) {
            log("Closing application...");
            _sf_window.close();
            log("Closed");
        }},
        {sf::Event::KeyPressed, [=](auto const& e)  {
            debug(fmt::format("Pressed key {}", static_cast<int>(e.key.code)));

            using KeyPressedCallback = std::function<void()>;
            auto key_callbacks = std::unordered_map<sf::Keyboard::Key, KeyPressedCallback>{
                { sf::Keyboard::Left, previous_processing_function },
                { sf::Keyboard::Right, next_processing_function },
            };

            auto noop = KeyPressedCallback{[](auto&&...) {}};
            auto callback = value_or(key_callbacks, e.key.code, noop);
            callback();
        }},
    };

    auto callback = value_or(callbacks, e.type, noop);
    callback(e);
}

void Window::set_image(std::filesystem::path const& path) {
    _image = load_image(path);
    _sf_texture = make_sf_texture(_image);
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
