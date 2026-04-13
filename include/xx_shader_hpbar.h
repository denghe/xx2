#pragma once
#include "xx_frame.h"
#include "xx_shader.h"

namespace xx {

    // fixed 2 pixel border width. value: 0 ~ 1 foreground left to right ratio
    struct Shader_HPBarData {
        XY pos{}, size{ 50, 8 };                        // float * 4
        xx::RGBA8 colorBorder{ 111,111,111,111 };       // u8 * 4
        xx::RGBA8 colorBackground{ 22,22,22,22 };       // u8 * 4
        xx::RGBA8 colorForeground{ 255,0,0,255 };       // u8 * 4
        float value{};                                  // float * 1

        void Fill(XY pos_, XY size_
            , xx::RGBA8 colorBorder_
            , xx::RGBA8 colorBackground_
            , xx::RGBA8 colorForeground_
            , float value_);
    };

    struct Shader_HPBar : xx::Shader {
        using xx::Shader::Shader;
        GLint aVert{ -1 }
            , aPosSize{ -1 }
            , aColorBorder{ -1 }
            , aColorBackground{ -1 }
            , aColorForeground{ -1 }
        , aValue{ -1 };
        xx::GLVertexArrays va;
        xx::GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 20000 };
        std::unique_ptr<Shader_HPBarData[]> data = std::make_unique_for_overwrite<Shader_HPBarData[]>(maxNums);
        int32_t count{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        Shader_HPBarData* Alloc();
    };

}
