#pragma once
#include "xx_gamebase.h"

namespace xx {

    struct FrameBuffer {
        GLFrameBuffer fb;
        XY bakWndSiz{};
        XYu bakTexSiz{};
        std::array<uint32_t, 3> bakBlend{};

        // need ogl frame env
        FrameBuffer& Init() {
            assert(fb == (GLuint)-1);
            fb = MakeGLFrameBuffer();
            return *this;
        }

        template<typename Func>
        void DrawTo(Shared<GLTexture>& t, std::optional<RGBA8> const& c, Func&& func, Data* store = {}) {
            Begin(t, c);
            func();
            End(store);
        }

        template<typename Func>
        Shared<GLTexture> Draw(XYu const& wh, bool hasAlpha, std::optional<RGBA8> const& c, Func&& func, Data* store = {}) {
            auto t = MakeShared<GLTexture>();
            t->Make({wh.x, wh.y}, hasAlpha);
            DrawTo(t, c, std::forward<Func>(func), store);
            return t;
        }

    protected:
        void Begin(Shared<GLTexture>& t, std::optional<RGBA8> const& c = {}) {
            assert(t);
            auto& g = *GameBase::instance;
            g.ShaderEnd();
            bakWndSiz = g.windowSize;
            bakBlend = g.blend;
            bakTexSiz = t->size;
            g.SetWindowSize(t->size);
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
