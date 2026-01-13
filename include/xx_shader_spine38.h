#pragma once
#include "xx_shader.h"

namespace xx {

    struct Shader_SpineData {
        XY pos, uv;
        RGBA8 color;
    };

    struct Shader_Spine : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, aPosUv{ -1 }, aColor{ -1 };
        GLVertexArrays va;
        GLBuffer vb;

        static constexpr int32_t maxNums{ maxVertNums * 4 };
        GLuint lastTextureId{};
        int32_t count{};
        std::unique_ptr<Shader_SpineData[]> data = std::make_unique<Shader_SpineData[]>(maxNums);

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        Shader_SpineData* Alloc(GLuint texId_, int32_t num_);
    };

}
