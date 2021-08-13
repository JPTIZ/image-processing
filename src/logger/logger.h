#pragma once

#include <iostream>
#include <string_view>

#include <fmt/color.h>

namespace tizip::logger {

enum class LogLevels {
    NoLog,
    Common,
    Warning,
    Error,
    Debug,
};

#ifdef NDEBUG
constexpr auto MaxLogLevel = LogLevels::Error;
#else
constexpr auto MaxLogLevel = LogLevels::Debug;
#endif

void log(std::string_view const& s) {
    if constexpr (MaxLogLevel >= LogLevels::Common) {
        fmt::print(fmt::fg(fmt::color::dim_gray), "[LOG] ");
        fmt::print("{}\n", s);
    }
}

void warn(std::string_view const& s) {
    if constexpr (MaxLogLevel >= LogLevels::Warning) {
        fmt::print(fmt::fg(fmt::color::yellow), "[WRN] ");
        fmt::print("{}\n", s);
    }
}

void error(std::string_view const& s) {
    if constexpr (MaxLogLevel >= LogLevels::Error) {
        fmt::print(fmt::fg(fmt::color::red), "[ERR] ");
        fmt::print("{}\n", s);
    }
}

void debug(std::string_view const& s) {
    if constexpr (MaxLogLevel >= LogLevels::Debug) {
        fmt::print(fmt::fg(fmt::color::dark_green), "[DBG] ");
        fmt::print("{}\n", s);
    }
}

}
