#pragma once
#include <xx_shader.h>

namespace xx {

    struct Shader_NumbersData {
		static constexpr XY cCharSize{ 10.f, 13.f };

        XY pos{}, scale{ 1, 1 };                // float * 4
        xx::RGBA8 color{ 255, 255, 255, 255 };  // u8 * 4
        uint32_t numbers[4];                    // u32 * 4      // last one byte is len

        int32_t Len() const {
            auto tar = (uint8_t*)&numbers;
            return (int32_t)tar[15];
        }

        int32_t FillTxt(std::string_view n_);
        int32_t FillTxt(int32_t n_, bool includeSignal_ = false);
        void FillBase(XY pos_, XY scale_, RGBA8 color_);
        int32_t Fill(XY pos_, XY scale_, RGBA8 color_, std::string_view n_);
        int32_t Fill(XY pos_, XY scale_, RGBA8 color_, int32_t n_, bool includeSignal_ = false);
    };

    struct Shader_Numbers : xx::Shader {
        using xx::Shader::Shader;
        GLint uTex0{ -1 }, aVert{ -1 }, aPosScale{ -1 }, aColor{ -1 }, aNumbers{ -1 };
        xx::GLVertexArrays va;
        xx::GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 30000 };
        std::unique_ptr<Shader_NumbersData[]> data = std::make_unique_for_overwrite<Shader_NumbersData[]>(maxNums);
        int32_t count{};

        void Init();
        void Begin() override;
        void End() override;
        void Commit();
        Shader_NumbersData* Alloc(int32_t num_ = 1);
        void Draw(Shader_NumbersData const& d_);
        void Draw(Shader_NumbersData const* buf_, int32_t len_);
    };

}
