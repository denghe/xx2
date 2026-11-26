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


    /****************************************************************************************/
    /****************************************************************************************/


    int32_t RectPackerEx::Pack(XY texSize_, XY padding_) {
        assert(fs.len);
        auto siz = texSize_.As<int32_t>();
        assert(siz.x > 0 && siz.y > 0);

        // create rects
        rects.Clear();
        rects.Reserve(fs.len);
        auto grow = (padding_ * 2).As<int32_t>();
        for (int32_t i = 0; i < fs.len; ++i) {
            auto& f = fs[i];
            auto& rect = rects.Emplace();
            rect.id = i;
            auto e = f->extrude * 2;
            rect.w = f->uvRect.w + grow.x + e;
            rect.h = f->uvRect.h + grow.y + e;
        }

        // place
        stbrp_context c;
        nodes.Resize(siz.x);
        stbrp_init_target(&c, siz.x, siz.y, nodes.buf, nodes.len);
        auto r = stbrp_pack_rects(&c, rects.buf, rects.len);
        if (r == 0) return __LINE__;

        // rects to tex
        Shared<GLTexture> t;
        t.Emplace()->Make(texSize_);
        FrameBuffer{}.Init().DrawTo(t, {}, [&] {
            XY basePos{ -texSize_.x / 2, -texSize_.y / 2 };
            for (auto& o : rects) {
                auto& f = *fs[o.id];
                o.x += padding_.x;
                o.y += padding_.y;
                auto pos = basePos + XY{ o.x, o.y };
                auto uv = f.uvRect;
                uv.x -= f.extrude;
                uv.y -= f.extrude;
                uv.w += f.extrude * 2;
                uv.h += f.extrude * 2;
                o.x += f.extrude;
                o.y += f.extrude;
                GameBase::instance->Quad().Draw(*f.tex, uv, pos, 0);
            }
        });

        // update target frames
        for (auto& o : rects) {
            auto& f = *fs[o.id];
            f.tex = t;
            f.uvRect.x = uint16_t(o.x);
            f.uvRect.y = uint16_t(texSize_.y - o.y - f.uvRect.h);
        }

        return 0;
    }

    int32_t RectPackerEx::AutoPack(int32_t minPackSize_, XY padding_) {
    LabRetry:
        if (auto r = Pack(minPackSize_); r) {
            minPackSize_ *= 2;
            assert(minPackSize_ <= 16384);
            goto LabRetry;
        }
        fs[0]->tex->TryGenerateMipmap();
        return 0;
    }

    GLTexture& RectPackerEx::Tex() {
        return *fs[0]->tex;
    }
}
