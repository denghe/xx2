#pragma once
#include "xx_shader_quad.h"

namespace xx {

    struct Shader_QuadLight : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, uTex1{ -1 }, aVert{ -1 }, aPosAnchor{ -1 }, aScaleRadiansColorplus{ -1 }, aColor{ -1 }, aTexRect{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 20000 };
        GLuint lastTextureId{}, lastLightTextureId{};
        std::unique_ptr<Shader_QuadData[]> data = std::make_unique_for_overwrite<Shader_QuadData[]>(maxNums);
        int32_t count{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        Shader_QuadData* Alloc(GLuint texId, GLuint lightTexId, int32_t num_);
        void Draw(Shared<GLTexture> tex, Shared<GLTexture> lightTex, RGBA8 color = xx::RGBA8_White, float colorplus = 1.f);
    };

}
