#pragma once
#include "xx_list.h"
#include "xx_file.h"

namespace xx {
    int LoadOpusMemory(xx::List<float>& tmpContainer, void* buf, size_t len);
    int LoadOpusFilePath(xx::List<float>& tmpContainer, std::filesystem::path opusFilePath);
    int LoadOpusFileName(xx::List<float>& tmpContainer, std::string_view opusFilePath);
}
