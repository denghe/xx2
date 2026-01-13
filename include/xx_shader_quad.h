#pragma once
#include "xx_frame.h"
#include "xx_shader.h"

namespace xx {

    struct Shader_QuadData {
        XY pos{}, anchor{ 0.5, 0.5 };                           // float * 4
        XY scale{ 1, 1 }; float radians{}, colorplus{ 1 };      // float * 4
        RGBA8 color{ 255, 255, 255, 255 };                      // u8n * 4
        UVRect texRect{};                                       // u16 * 4 ( left-top: 0,0 )

        void Fill(UVRect rect_, XY pos_ = {}, XY anchor_ = 0.5f, XY scale_ = 1.f, float radians_ = 0.f, float colorplus_ = 1.f, RGBA8 color_ = RGBA8_White);
    };

    struct Shader_Quad : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, aVert{ -1 }, aPosAnchor{ -1 }, aScaleRadiansColorplus{ -1 }, aColor{ -1 }, aTexRect{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 200000 };
        GLuint lastTextureId{};
        std::unique_ptr<Shader_QuadData[]> data = std::make_unique_for_overwrite<Shader_QuadData[]>(maxNums);
        int32_t count{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        Shader_QuadData* Alloc(GLuint texId_, int32_t num_);
        void Draw(GLuint texId_, UVRect rect_, XY pos_ = {}, XY anchor_ = 0.5f, XY scale_ = 1.f, float radians_ = 0.f, float colorplus_ = 1.f, RGBA8 color_ = RGBA8_White);
        void DrawTinyFrame(TinyFrame& tinyFrame_, XY pos_ = {}, XY anchor_ = 0.5f, XY scale_ = 1.f, float radians_ = 0.f, float colorplus_ = 1.f, RGBA8 color_ = RGBA8_White);
        void DrawFrame(Frame& frame_, XY pos_ = {}, XY scale_ = 1.f, float radians_ = 0.f, float colorplus_ = 1.f, RGBA8 color_ = RGBA8_White);
    };

}
