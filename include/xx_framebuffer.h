#pragma once
#include "xx_data.h"
#include "xx_ptr.h"
#include "xx_prims.h"
#include "xx_gl.h"

namespace xx {
	
    struct FrameBuffer {
        GLFrameBuffer fb;
        XY bakWndSiz{};
        XYu bakTexSiz{};
        std::array<uint32_t, 3> bakBlend{};

        // need ogl frame env
        FrameBuffer& Init();

        template<typename Func>
        void DrawTo(Shared<GLTexture>& t, std::optional<RGBA8> const& c, Func&& func, Data* store = {}) {
            Begin(t, c);
            func();
            End(store);
        }

        template<typename Func>
        Shared<GLTexture> Draw(XYu const& wh, bool hasAlpha, std::optional<RGBA8> const& c, Func&& func, Data* store = {}) {
            auto t = MakeShared<GLTexture>();
            t->Make({ wh.x, wh.y }, hasAlpha);
            DrawTo(t, c, std::forward<Func>(func), store);
            return t;
        }

    protected:
        void Begin(Shared<GLTexture>& t, std::optional<RGBA8> const& c = {});
        void End(Data* store = {});
    };

}
