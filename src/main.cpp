#include <iostream>

#include <logger.h>
#include <viewer/window.h>

int main(int argc, char **argv) {
    using tizip::logger::error;
    using tizip::logger::log;
    using tizip::logger::warn;

    if (argc != 1) {
        error(std::string{argv[0]} + " takes no arguments.");
        return 1;
    }

    log("Starting application...");

    viewer::Window("Image Processing Viewer", 800, 600).show();
}
