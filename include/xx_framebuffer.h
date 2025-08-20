#pragma once
#include "xx_gamebase.h"

namespace xx {

    struct FrameBuffer {
        GLFrameBuffer fb;
        XY bakWndSiz{};
        XYu bakTexSiz{};
        std::array<uint32_t, 3> bakBlend{};

        // need ogl frame env
        explicit FrameBuffer(bool autoInit = false) {
            if (autoInit) {
                Init();
            }
        }

        // need ogl frame env
        FrameBuffer& Init() {
            assert(!fb);   // excessive initializations ?
            fb = MakeGLFrameBuffer();
            return *this;
        }

        inline static Ref<GLTexture> MakeTexture(XYu const& wh, bool hasAlpha = true) {
            return MakeRef<GLTexture>(GLTexture::Create(wh.x, wh.y, hasAlpha));
        }

        template<typename Func>
        void DrawTo(Ref<GLTexture>& t, std::optional<RGBA8> const& c, Func&& func, Data* store = {}) {
            Begin(t, c);
            func();
            End(store);
        }

        template<typename Func>
        Ref<GLTexture> Draw(XYu const& wh, bool hasAlpha, std::optional<RGBA8> const& c, Func&& func, Data* store = {}) {
            auto t = MakeTexture(wh, hasAlpha);
            DrawTo(t, c, std::forward<Func>(func), store);
            return t;
        }

    protected:
        void Begin(Ref<GLTexture>& t, std::optional<RGBA8> const& c = {}) {
            assert(t);
            auto& g = *GameBase::instance;
            g.ShaderEnd();
            bakWndSiz = g.windowSize;
            bakBlend = g.blend;
            bakTexSiz = { t->Width(), t->Height() };
            g.SetWindowSize(t->Size());
            g.flipY = -1;
            BindGLFrameBufferTexture(fb, *t);
            g.GLViewport();
            if (c.has_value()) {
                g.GLClear(c.value());
            }
            g.GLBlendFunc(g.blendDefault);
        }

        void End(Data* store = {}) {
            auto& g = *GameBase::instance;
            g.ShaderEnd();
            if (store) {
                GLFrameBufferSaveTo(*store, 0, 0, bakTexSiz.x, bakTexSiz.y);
            }
            UnbindGLFrameBuffer();
            g.SetWindowSize(bakWndSiz);
            g.flipY = 1;
            g.GLViewport();
            g.GLBlendFunc(bakBlend);
        }
    };

}
