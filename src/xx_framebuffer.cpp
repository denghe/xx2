#include "xx_framebuffer.h"
#include "xx_gamebase.h"

namespace xx {
	
    // need ogl frame env
    FrameBuffer& FrameBuffer::Init() {
        assert(fb == (GLuint)-1);
        fb = MakeGLFrameBuffer();
        return *this;
    }

    void FrameBuffer::Begin(Shared<GLTexture>& t, std::optional<RGBA8> const& c) {
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

    void FrameBuffer::End(Data* store) {
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

}
