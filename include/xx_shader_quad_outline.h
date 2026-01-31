#pragma once
#include "xx_frame.h"
#include "xx_shader.h"

namespace xx {

    struct Shader_QuadOutlineData {
        XY pos{}, anchor{ 0.5, 0.5 };                           // float * 4
        XY scale{ 1, 1 }; float radians{}, colorplus{ 1 };      // float * 4
        RGBA8 color{ 255, 255, 255, 255 };                      // u8n * 4
        UVRect texRect{};                                       // u16 * 4 ( left-top: 0,0 )
        RGBA8 outlineColor{};                                   // u8 * 4
        XY outlineThickness{};                                  // float * 2

        void Fill(UVRect rect_, XY pos_, XY anchor_, XY scale_
            , float radians_, float colorplus_, RGBA8 color_
            , RGBA8 outlineColor_, XY outlineThickness_);
    };

    struct Shader_QuadOutline : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, aVert{ -1 }, aPosAnchor{ -1 }, aScaleRadiansColorplus{ -1 }, aColor{ -1 }, aTexRect{ -1 }, aOutlineColor{ -1 }, aOutlineThickness{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 20000 };
        GLuint lastTextureId{};
        std::unique_ptr<Shader_QuadOutlineData[]> data = std::make_unique_for_overwrite<Shader_QuadOutlineData[]>(maxNums);
        int32_t count{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        Shader_QuadOutlineData* Alloc(GLuint texId, int32_t num_);
        Shader_QuadOutlineData* Alloc(GLuint texId, UVRect const& uvRect, float outlineThickness, RGBA8 outlineColor);
        // todo
    };

}
