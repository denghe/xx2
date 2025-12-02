#pragma once
#include "xx_gl.h"

namespace xx {

    struct FontFrame {
        GLuint texId;
        UVRect uvRect;
    };

    // sprite frame
    struct TinyFrame {
        Shared<GLTexture> tex;
        UVRect uvRect{};

        TinyFrame() = default;
        TinyFrame(TinyFrame const&) = default;
        TinyFrame(TinyFrame &&) = default;
        TinyFrame& operator=(TinyFrame const&) = default;
        TinyFrame& operator=(TinyFrame &&) = default;

        XX_INLINE operator GLuint () const {
            return tex->id;
        }

        XX_INLINE operator UVRect () const {
            return uvRect;
        }

        XX_INLINE XY Size() const {
            return { uvRect.w, uvRect.h };
        }

        TinyFrame(Shared<GLTexture> t) {
            operator=(std::move(t));
        }

        template<typename T>
        TinyFrame(Shared<GLTexture> t, T x, T y, T w, T h) {
            tex = std::move(t);
            uvRect = { (uint16_t)x, (uint16_t)y, (uint16_t)w, (uint16_t)h };
        }

        void operator=(Shared<GLTexture> t) {
            tex = std::move(t);
            uvRect = tex->Rect();
        }
    };

    // sprite frame with anchor point
    struct Frame {
        Shared<GLTexture> tex;
        UVRect uvRect{};
        XY anchor{};

        Frame() = default;
        Frame(Frame const&) = default;
        Frame(Frame&&) = default;
        Frame& operator=(Frame const&) = default;
        Frame& operator=(Frame&&) = default;

        XX_INLINE operator TinyFrame& () const {
            return *(TinyFrame*)this;
        }

        XX_INLINE operator GLuint () const {
            return tex->id;
        }

        XX_INLINE operator UVRect () const {
            return uvRect;
        }

        XX_INLINE XY Size() const {
            return { uvRect.w, uvRect.h };
        }

        Frame(Shared<GLTexture> t) {
            operator=(std::move(t));
        }

        template<typename T>
        Frame(Shared<GLTexture> t, T x, T y, T w, T h, XY anchor_ = 0.5f) {
            tex = std::move(t);
            uvRect = { (uint16_t)x, (uint16_t)y, (uint16_t)w, (uint16_t)h };
            anchor = anchor_;
        }

        void operator=(Shared<GLTexture> t) {
            tex = std::move(t);
            uvRect = tex->Rect();
            anchor = 0.5f;
        }

        XX_INLINE operator XY const& () const {
            return anchor;
        }
    };

    // texture packer element container
    struct TextureFrame {
        std::string key;
        // List<std::string> aliases;	        // unused
        XY anchor{};                            // unset / default: 0.5
        XY spriteOffset{};                      // content pos for original size
        XY spriteSize{};		                // content size( cut alpha )
        XY spriteSourceSize{};	                // original file size
        bool textureRotated{};
        List<uint16_t> triangles;
        List<float> vertices;
        List<float> verticesUV;
        Shared<GLTexture> tex;
        UVRect uvRect{};
    };

    //struct AnimFrame {
    //    Shared<Frame> frame;
    //    float durationSeconds;
    //};

    //using AnimFrames = List<AnimFrame>;

    //struct Anim {
    //    List<AnimFrame> animFrames;
    //    size_t cursor{};
    //    float timePool{};

    //    void Step() {
    //        if (++cursor == animFrames.size()) {
    //            cursor = 0;
    //        }
    //    }

    //    bool Update(float delta) {
    //        auto bak = cursor;
    //        timePool += delta;
    //    LabBegin:
    //        auto&& af = animFrames[cursor];
    //        if (timePool >= af.durationSeconds) {
    //            timePool -= af.durationSeconds;
    //            Step();
    //            goto LabBegin;
    //        }
    //        return bak != cursor;
    //    }

    //    AnimFrame& GetCurrentAnimFrame() const {
    //        return (AnimFrame&)animFrames[cursor];
    //    }
    //};

}
