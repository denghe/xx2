#pragma once
#include "xx_shader.h"

namespace xx {

    struct Shader_RadiateGoldenRaysData {
        XY pos{};
        float time{}, radius{};
    };

    struct Shader_RadiateGoldenRays : Shader {
        using Shader::Shader;
        GLint aVert{ -1 }, aPosTimeRadius{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 100 };
        std::unique_ptr<Shader_RadiateGoldenRaysData[]> data = std::make_unique_for_overwrite<Shader_RadiateGoldenRaysData[]>(maxNums);
        int32_t count{};

        // for easy use
        Shader_RadiateGoldenRays& operator()();

        void Init();

        virtual void Begin() override;

        virtual void End() override;

        void Commit();

        Shader_RadiateGoldenRaysData* Alloc(int32_t num_);

        void Draw(XY pos_ = {}, float time_ = 0, float radius_ = 100);
    };

}
