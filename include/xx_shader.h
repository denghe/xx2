#pragma once
#include "xx_gl.h"

#ifndef __EMSCRIPTEN__
#   define XX_SHADER_CODE_FIRST_LINE "#version 330"
#else
#   define XX_SHADER_CODE_FIRST_LINE "#version 300 es"
#endif

namespace xx {
    struct GameBase;

    // base class
    struct Shader {
        static constexpr size_t maxVertNums{ 65535 };	// 65535 for primitive restart index
		static constexpr size_t maxIndexNums{ 65535 };	// 65535 for int16_t

        GLShader v, f;
        GLProgram p;
        GLint uCxy{ -1 };

        Shader() = default;
        Shader(Shader const&) = delete;
        Shader& operator=(Shader const&) = delete;
        virtual ~Shader() {}
        virtual void Begin() = 0;
        virtual void End() = 0;

        // need c++23
        // for easy Commit previous different shader
        auto& operator()(this auto&& self) {
		    auto g = GameBase::instance;
		    if (g->shader != &self) {
			    if (g->shader) {
				    g->shader->End();
				    g->shader = {};
			    }
			    self.Begin();
			    g->shader = &self;
		    }
		    return self;
	    }

    };

}
