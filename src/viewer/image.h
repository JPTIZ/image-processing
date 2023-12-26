#pragma once

#include <filesystem>

#include <SFML/Graphics.hpp>

namespace tizip {

class Image {
public:
    static constexpr auto ColorDepth = 32;
    static constexpr auto BytesPerPixel = ColorDepth / 8;

    Image(int width, int height);
    Image(int width, int height, uint8_t const* data);

    auto data() const -> std::vector<uint8_t> {
        auto raw = _sf_image.getPixelsPtr();
        auto raw_size = width() * height() * BytesPerPixel;
        return std::vector<uint8_t>{raw, raw + raw_size};
    }

    auto width() const -> int {
        return _sf_image.getSize().x;
    }

    auto height() const -> int {
        return _sf_image.getSize().y;
    }

private:
    sf::Image _sf_image;
};

auto load_image(std::filesystem::path const& path) -> tizip::Image;

}
