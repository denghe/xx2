#pragma once
#include "xx_data.h"

namespace xx {
    bool IsZstdCompressedData(void const* buf_, size_t len);

    bool IsZstdCompressedData(Span d);

    void ZstdDecompress(Span src, Data& dst);

    void TryZstdDecompress(Data& d);

    void ZstdCompress(Span src, Data& dst, int level = 3, bool doShrink = true);
}
