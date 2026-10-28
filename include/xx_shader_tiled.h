#pragma once
#include "xx_frame.h"
#include "xx_shader.h"

namespace xx {

    struct Shader_TiledData {
        XY pos{}, scale{};                  // float * 4
        XY tileSize{}, mapSize{};           // float * 4 ( pixel )
        XY tiling{}, offset{};              // float * 4
		XYi dataSize{};                     // i32 * 2 ( GLTiledTexture.unalignedSize )

        /*
        * example:
            pos = camera.ToGLPos({ 0, stage->mapSize.y });
            scale = camera.scale;
            tileSize = tileSize;
            mapSize = stage->gridSize * tileSize;
            tiling = 1;
            offset = 0;
        */
        void Fill(XY pos_, XY scale_, XY tileSize_, XY mapSize_, XY tiling_ = 1, XY offset_ = {}, XYi dataSize_ = {});
    };

    struct Shader_Tiled : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, uTex1{ -1 }, aVert{ -1 }, aPosScale{ -1 }, aTileSizeMapSize{ -1 }, aTilingOffset{ -1 }, aDataSize{ -1 };
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
		Shader_TiledData* Draw(GLuint texId_TiledSample_, GLuint texId_TiledData_);
		Shader_TiledData* Draw(xx::Shared<xx::GLTexture> const& texTiledSample_, xx::Shared<xx::GLTiledTexture> const& texTiledData_);
    };

}
