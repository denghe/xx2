#pragma once
#include <xx_shader.h>

namespace xx {

    struct Shader_NumbersData {
        XY pos{}, scale{ 1, 1 };                // float * 4
        xx::RGBA8 color{ 255, 255, 255, 255 };  // u8 * 4
        uint32_t numbers[4];                    // u32 * 4      // last one byte is len

        int32_t Fill(std::string_view n_);
        int32_t Fill(int32_t n_, bool includeSignal_ = false);
        int32_t Fill(XY pos_, XY scale_, RGBA8 color_, std::string_view n_);
        int32_t Fill(XY pos_, XY scale_, RGBA8 color_, int32_t n_, bool includeSignal_ = false);
    };

    struct Shader_Numbers : xx::Shader {
        using xx::Shader::Shader;
        GLint uTex0{ -1 }, aVert{ -1 }, aPosScale{ -1 }, aColor{ -1 }, aNumbers{ -1 };
        xx::GLVertexArrays va;
        xx::GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 20000 };
        std::unique_ptr<Shader_NumbersData[]> data = std::make_unique_for_overwrite<Shader_NumbersData[]>(maxNums);
        int32_t count{};

        void Init();
        void Begin() override;
        void End() override;
        void Commit();
        Shader_NumbersData* Alloc(int32_t num_);
        void Draw(Shader_NumbersData const& d_);
    };

}
