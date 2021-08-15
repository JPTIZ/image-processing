#pragma once

#include <logger.h>

namespace tizip::errors {

inline auto fail(std::string const& msg) {
    tizip::logger::error(msg);
    throw std::runtime_error(msg);
}

}
