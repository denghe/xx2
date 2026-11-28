#pragma once
#include "xx_ptr.h"
#include "xx_prims.h"
#include "xx_gl.h"

namespace xx {
	
    // for bmfont
    struct FontFrame {
        GLuint texId;
        UVRect uvRect;
    };


    // base sprite frame
    struct TinyFrame {
        Shared<GLTexture> tex;
        UVRect uvRect{};

        TinyFrame() = default;
        TinyFrame(TinyFrame const&) = default;
        TinyFrame(TinyFrame&&) = default;
        TinyFrame& operator=(TinyFrame const&) = default;
        TinyFrame& operator=(TinyFrame&&) = default;

        operator GLuint () const;
        operator UVRect () const;
        operator bool() const;
        XY Size() const;
        void operator=(Shared<GLTexture> t);
        TinyFrame(Shared<GLTexture> t);
        TinyFrame(Shared<GLTexture> t, int32_t x, int32_t y, int32_t w, int32_t h);

        void Clear();
    };

    // Frame part 1
    struct FrameTex {
        Shared<GLTexture> tex;
    };

    // Frame part 2
    // for static constexpr declare & easy copy value to Frame
    struct FrameData {
    union {
        struct {
            UVRect uvRect;
            UVRect uvTrim;
            XY anchor;
            int32_t extrude;
            int32_t spineEventIndex;
        };
        std::array<uint64_t, 4> props{};            // for memcpy & simple initialization
    };
    };

    // sprite frame with anchor point
    struct Frame : FrameTex, FrameData {
        Frame() = default;
        Frame(Frame const&) = default;
        Frame(Frame&&) = default;
        Frame& operator=(Frame const&) = default;
        Frame& operator=(Frame&&) = default;

        operator TinyFrame& () const;
        operator TinyFrame* () const;
        operator GLuint () const;
        operator UVRect () const;
        operator XY const& () const;
        operator bool () const;
        XY Size() const;
        void operator=(Shared<GLTexture> t);

        Frame(Shared<GLTexture> t);
        Frame(Shared<GLTexture> t, int32_t x, int32_t y, int32_t w, int32_t h, XY a = 0.5f, int32_t e = 0);

        XY CalcTrimAnchor(XY a) const;
        void Fill(Shared<GLTexture> const& t_, FrameData const& d_);
        void Clear();
    };

}
