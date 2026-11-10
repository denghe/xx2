#pragma once
#include "xx_shader.h"

namespace xx {

    struct Shader_RingData {
        XY pos{};
        float radius{};
        float borderWidth{};                        // float * 4

        RGBA8 color{};                              // byte * 4
    };

    struct Shader_Ring : Shader {
        using Shader::Shader;
        GLint aVert{ -1 }, aPosRadiusBorderWidth{ -1 }, aColor{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t numCircleMeshSegaments{ 16 };
        static constexpr int32_t maxNums{ 100 };
        std::unique_ptr<Shader_RingData[]> data = std::make_unique_for_overwrite<Shader_RingData[]>(maxNums);
        int32_t count{};

        void Init();

        virtual void Begin() override;

        virtual void End() override;

        void Commit();

        Shader_RingData* Alloc(int32_t num_);

        void Draw(XY pos_ = {}, float radius_ = 100.f, float borderWidth_ = 5.f / 100.f, RGBA8 color_ = RGBA8_White);
    };

}
