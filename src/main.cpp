#include <logger.h>
#include <viewer/window.h>

int main(int argc, char **argv)
try {
    using tizip::viewer::Window;
    using tizip::logger::error;
    using tizip::logger::log;
    using tizip::logger::warn;

    if (argc != 2) {
        error(fmt::format("Usage: {} <image path>.", argv[0]));
        return 1;
    }

    log("Starting application...");

    auto window = Window("Image Processing Viewer", 800, 600);
    window.set_image(argv[1]);
    window.show();
} catch (std::runtime_error const& err) {
    tizip::logger::error(fmt::format("Application failed with message:\n    {}", err.what()));
}
