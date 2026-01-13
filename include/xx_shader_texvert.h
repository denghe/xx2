#pragma once
#include "xx_shader_spine.h"

namespace xx {

    // tex data: float * 8
    // pos(2) + uv(2) + color(4)
    struct Shader_TexVertElement {
        float x, y, u, v, r, g, b, a;
    };

    struct Shader_TexVertData {
        XY pos{}, scale{};
        int32_t frameIndex{};
    };

    struct Shader_TexVert : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, uTex1{ -1 }, aVertIndex{ -1 }, aPosScale{ -1 }, aFrameIndex{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t vertCap{ 8192 };
        static constexpr int32_t maxNums{ 100000 };
        GLuint lastTextureId{}, lastVertTextureId{}, numVerts{};
        std::unique_ptr<Shader_TexVertData[]> data = std::make_unique_for_overwrite<Shader_TexVertData[]>(maxNums);
        int32_t count{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        Shader_TexVertData* Alloc(Shared<GLTexture> tex_, Shared<GLVertTexture> const& vertTex_, int32_t num_);
    };

}
