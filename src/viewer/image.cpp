#include "image.h"

#include "errors.h"

#include <fmt/format.h>

namespace {

auto make_sf_image(int width, int height) -> sf::Image {
    auto image = sf::Image{};
    image.create(width, height);
    return image;
}

auto make_sf_image(int width, int height, uint8_t const* data) -> sf::Image {
    auto image = sf::Image{};
    image.create(width, height, data);
    return image;
}

}

auto tizip::load_image(std::filesystem::path const& path) -> tizip::Image {
    auto path_str = path.string();
    auto sf_image = sf::Image{};
    if (not sf_image.loadFromFile(path_str)) {
        errors::fail(fmt::format("Failed to load image {}", path_str));
    }
    auto [width, height] = sf_image.getSize();
    auto image = tizip::Image(width, height, sf_image.getPixelsPtr());
    return image;
}

namespace tizip {

Image::Image(int width, int height):
    _sf_image{make_sf_image(width, height)}
{}

Image::Image(int width, int height, uint8_t const* data):
    _sf_image{make_sf_image(width, height, data)}
{}

}
