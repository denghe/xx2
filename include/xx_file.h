#pragma once
#include "xx_data.h"

namespace xx {

    Data ReadAllBytes(std::filesystem::path const& path);

    Data ReadAllBytes_sv(std::string_view path);    // utf8

    int WriteAllBytes(std::filesystem::path const& path, char const* buf, size_t len);

    bool IsAbsolutePathName(std::string_view s);
}
