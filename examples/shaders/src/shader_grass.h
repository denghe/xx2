#pragma once
#include "xx_shader.h"

namespace xx {

    struct Shader_GrassData {
        XY pos{}, anchor{ 0.5, 0.5 };                           // float * 4
        XY scale{ 1, 1 }; float radians{}, colorplus{ 1 };      // float * 4
        RGBA8 color{ 255, 255, 255, 255 };                      // u8n * 4
        UVRect texRect{};                                       // u16 * 4 ( left-top: 0,0 )

        void Fill(UVRect rect_, XY pos_ = {}, XY anchor_ = 0.5f, XY scale_ = 1.f
            , float radians_ = 0.f, float colorplus_ = 1.f, RGBA8 color_ = RGBA8_White);
    };

    struct Shader_Grass : Shader {
        using Shader::Shader;
        GLint aVert{ -1 }, aPosAnchor{ -1 }, aScaleRadiansColorplus{ -1 }, aColor{ -1 }, aTexRect{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 100 };
        std::unique_ptr<Shader_GrassData[]> data = std::make_unique_for_overwrite<Shader_GrassData[]>(maxNums);
        int32_t count{};

        // for easy use
        Shader_Grass& operator()();

        void Init();

        virtual void Begin() override;

        virtual void End() override;

        void Commit();

        Shader_GrassData* Alloc(int32_t num_);

        void Draw(UVRect rect_, XY pos_ = {}, XY anchor_ = 0.5f
            , XY scale_ = 1.f, float radians_ = 0.f, float colorplus_ = 1.f, RGBA8 color_ = RGBA8_White);
    };

}
