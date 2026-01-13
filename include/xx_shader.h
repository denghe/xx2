#pragma once
#include "xx_gl.h"

#ifndef __EMSCRIPTEN__
#   define XX_SHADER_CODE_FIRST_LINE "#version 330"
#else
#   define XX_SHADER_CODE_FIRST_LINE "#version 300 es"
#endif

namespace xx {

    // base class
    struct Shader {
        static constexpr size_t maxVertNums{ 65535 };	// 65535 for primitive restart index

        GLShader v, f;
        GLProgram p;
        GLint uCxy{ -1 };

        Shader() = default;
        Shader(Shader const&) = delete;
        Shader& operator=(Shader const&) = delete;
        virtual ~Shader() {}
        virtual void Begin() = 0;
        virtual void End() = 0;
    };

}
