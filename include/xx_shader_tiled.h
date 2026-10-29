#pragma once
#include "xx_frame.h"
#include "xx_shader.h"

namespace xx {

    struct Shader_TiledData {
        // pos: gl draw position by anchor 0,0
        // scale: for viewport size change
        XY pos{}, scale{};                                  // float * 4
        // tileSize: single tile size
        // mapSize: whole map/grid logic size( unaligned )
        XY tileSize{}, mapSize{};                           // float * 4

        void Fill(XY pos_, XY scale_ = 1.f, XY tileSize_ = 32, XY mapSize_ = {});
    };

    struct Shader_Tiled : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, uTex1{ -1 }, aVert{ -1 }, aPosScale{ -1 }, aTileSizeMapSize{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t bufLen{ 10000 };
        GLuint lastTextureId{}, lastMiniTextureId{};
        std::unique_ptr<Shader_TiledData[]> data = std::make_unique_for_overwrite<Shader_TiledData[]>(bufLen);
        int32_t count{};

        void Init();
        void Begin() override;
        void End() override;
        void Commit();
		Shader_TiledData* Alloc(GLuint texId_TiledSample_, GLuint texId_TiledData_);
		Shader_TiledData* Alloc(xx::Shared<xx::GLTexture> const& texTiledSample_, xx::Shared<xx::GLTiledTexture> const& texTiledData_);
    };

}
