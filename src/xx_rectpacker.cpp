#include "xx_rectpacker.h"
#include "xx_framebuffer.h"
#include "xx_gamebase.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

namespace xx {

    int32_t RectPacker::Pack(XY texSize_, XY padding_) {
        assert(tfs.len);
        auto siz = texSize_.As<int32_t>();
        assert(siz.x > 0 && siz.y > 0);

        rects.Clear();
        rects.Reserve(tfs.len);
        auto grow = (padding_ * 2).As<int32_t>();
        for (int32_t i = 0; i < tfs.len; ++i) {
            auto& tf = tfs[i];
            auto& rect = rects.Emplace();
            rect.id = i;
            rect.w = tf->uvRect.w + grow.x;
            rect.h = tf->uvRect.h + grow.y;
        }

        stbrp_context c;
        nodes.Resize(siz.x);
        stbrp_init_target(&c, siz.x, siz.y, nodes.buf, nodes.len);
        auto r = stbrp_pack_rects(&c, rects.buf, rects.len);
        if (r == 0) return __LINE__;

        Shared<GLTexture> t;
        t.Emplace()->Make(texSize_);
        FrameBuffer{}.Init().DrawTo(t, {}, [&] {
            XY basePos{ -texSize_.x / 2, -texSize_.y / 2 };
            for (auto& o : rects) {
                auto& tf = *tfs[o.id];
                o.x += padding_.x;
                o.y += padding_.y;
                auto pos = basePos + XY{ o.x, o.y };
                GameBase::instance->Quad().Draw(*tf.tex, tf.uvRect, pos, 0);
            }
            });
        for (auto& o : rects) {
            auto& tf = *tfs[o.id];
            tf.tex = t;
            tf.uvRect.x = uint16_t(o.x);
            tf.uvRect.y = uint16_t(texSize_.y - o.y - tf.uvRect.h);
        }

        return 0;
    }

    int32_t RectPacker::AutoPack(int32_t minPackSize_, XY padding_) {
    LabRetry:
        if (auto r = Pack(minPackSize_); r) {
            minPackSize_ *= 2;
            assert(minPackSize_ <= 16384);
            goto LabRetry;
        }
        tfs[0]->tex->TryGenerateMipmap();
        return 0;
    }

    GLTexture& RectPacker::Tex() {
        return *tfs[0]->tex;
    }
}
