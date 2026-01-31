#pragma once
#include "xx_frame.h"
#include "xx_shader.h"

namespace xx {

    struct Shader_QuadExData {
        XY pos{}, anchor{ 0.5, 0.5 };                           // float * 4
        XY scale{ 1, 1 }; float radians{}, colorplus{ 1 };      // float * 4
        RGBA8 color{ 255, 255, 255, 255 };                      // u8n * 4
        UVRect texRect{};                                       // u16 * 4 ( left-top: 0,0 )
        XY tiling{}, offset{};                                  // float * 4

        void Fill(UVRect rect_, XY pos_, XY anchor_, XY scale_
            , float radians_, float colorplus_, RGBA8 color_
            , XY tiling_, XY offset_);
    };

    struct Shader_QuadEx : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, aVert{ -1 }, aPosAnchor{ -1 }, aScaleRadiansColorplus{ -1 }, aColor{ -1 }, aTexRect{ -1 }, aTilingOffset{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 200000 };
        GLuint lastTextureId{};
        std::unique_ptr<Shader_QuadExData[]> data = std::make_unique_for_overwrite<Shader_QuadExData[]>(maxNums);
        int32_t count{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        Shader_QuadExData* Alloc(GLuint texId, int32_t numQuads = 1);
    };

}
