#pragma once
#include "xx_gl.h"

namespace xx {

    struct FontFrame {
        GLuint texId;
        UVRect textureRect;
    };

    // sprite frame
    struct TinyFrame {
        Ref<GLTexture> tex;
        UVRect textureRect{};

        TinyFrame() = default;
        TinyFrame(TinyFrame const&) = default;
        TinyFrame(TinyFrame &&) = default;
        TinyFrame& operator=(TinyFrame const&) = default;
        TinyFrame& operator=(TinyFrame &&) = default;

        TinyFrame(Ref<GLTexture> t) {
            tex = std::move(t);
            textureRect = tex->Rect();
        }

        TinyFrame(Ref<GLTexture> t, int x, int y, int w, int h) {
            tex = std::move(t);
            textureRect = { (uint16_t)x, (uint16_t)y, (uint16_t)w, (uint16_t)h };
        }

        void operator=(Ref<GLTexture> t) {
            tex = std::move(t);
            textureRect = tex->Rect();
        }
    };

    struct Frame : TinyFrame {
        XY anchor;                              // unset / default: 0.5
        XY spriteOffset{};                      // content pos for original size
    };

    // texture packer element container
    struct TextureFrame {
        std::string key;
        // List<std::string> aliases;	        // unused
        XY anchor;                              // unset / default: 0.5
        XY spriteOffset{};                      // content pos for original size
        XY spriteSize{};		                // content size( cut alpha )
        XY spriteSourceSize{};	                // original file size
        bool textureRotated{};
        List<uint16_t> triangles;
        List<float> vertices;
        List<float> verticesUV;
        Ref<GLTexture> tex;
        UVRect textureRect{};
    };

    //struct AnimFrame {
    //    Ref<Frame> frame;
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
