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

        void operator=(Ref<GLTexture> t) {
            tex = std::move(t);
            textureRect = tex->Rect();
        }
    };

    // texture packer element container
    struct Frame : TinyFrame {
        std::string key;
        // std::vector<std::string> aliases;	// unused
        std::optional<XY> anchor;
        XY spriteOffset{};                      // content pos for original size
        XY spriteSize{};		                // content size( cut alpha )
        XY spriteSourceSize{};	                // original file size
        bool textureRotated{};
        size_t ud{};                            // user data
        std::vector<uint16_t> triangles;
        std::vector<float> vertices;
        std::vector<float> verticesUV;
    };

    //struct AnimFrame {
    //    Ref<Frame> frame;
    //    float durationSeconds;
    //};

    //using AnimFrames = std::vector<AnimFrame>;

    //struct Anim {
    //    std::vector<AnimFrame> animFrames;
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
