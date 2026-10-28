#include "xx_gamebase.h"

namespace xx {

    void Shader_TiledData::Fill(XY pos_, XY scale_, XY tileSize_, XY mapSize_, XY tiling_, XY offset_, XYi dataSize_) {
        pos = pos_;
        scale = scale_;
        tileSize = tileSize_;
        mapSize = mapSize_;
        tiling = tiling_;
        offset = offset_;
        if (dataSize_.x || dataSize_.y) {
            dataSize = dataSize_;
        }
    }

    void Shader_Tiled::Init() {

            v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	            // screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosScale;
in vec4 aTileSizeMapSize;
in vec4 aTilingOffset;
in vec2 aDataSize;

out vec2 vTexCoord;
flat out vec4 vTileSizeOffset;
flat out ivec2 vDataSize;

void main() {
    gl_Position = vec4((aPosScale.xy + aTileSizeMapSize.zw * aVert * aPosScale.zw) * uCxy, 0.f, 1.f);
    vTexCoord = vec2(aTileSizeMapSize.z * aVert.x * aTilingOffset.x, aTileSizeMapSize.w - aTileSizeMapSize.w * aVert.y * aTilingOffset.y);
    vTileSizeOffset = vec4( aTileSizeMapSize.xy, aTilingOffset.zw );
    vDataSize = ivec2(aDataSize);
})"sv });

            f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;        // tiled samples
uniform sampler2D uTex1;        // tild data array    .xy: uv  .z: colorPlus  .w: todo

in vec2 vTexCoord;
flat in vec4 vTileSizeOffset;
flat in ivec2 vDataSize;

out vec4 oColor;

void main() {
    vec2 tileTexSize = vec2(textureSize(uTex0, 0));
    vec2 tileSize = vTileSizeOffset.xy;
    vec2 offset = vTileSizeOffset.zw;
    vec2 p = vTexCoord + offset;
    ivec2 n = ivec2(p / tileSize);
    ivec2 miniIdx = n - (n / vDataSize) * vDataSize;
    vec4 uvc = texelFetch(uTex1, miniIdx, 0);
    vec2 uv = uvc.xy;// * tileSize;
    vec2 uvOffset = p - tileSize * vec2(miniIdx);
    vec4 c = texture(uTex0, (uv + uvOffset) / tileTexSize);
    oColor = vec4( c.x * uvc.z, c.y * uvc.z, c.z * uvc.z, c.w );
})"sv });

            p = LinkGLProgram(v, f);

            uCxy = glGetUniformLocation(p, "uCxy");
            uTex0 = glGetUniformLocation(p, "uTex0");
            uTex1 = glGetUniformLocation(p, "uTex1");

            aVert = glGetAttribLocation(p, "aVert");
            aPosScale = glGetAttribLocation(p, "aPosScale");
            aTileSizeMapSize = glGetAttribLocation(p, "aTileSizeMapSize");
            aTilingOffset = glGetAttribLocation(p, "aTilingOffset");
            aDataSize = glGetAttribLocation(p, "aDataSize");
            CheckGLError();

            glGenVertexArrays(1, &va.id);
            glBindVertexArray(va);

            glGenBuffers(1, &ib.id);
            static const XY verts[4] = { { 0, 0 },{ 0, 1.f },{ 1.f, 0 },{ 1.f, 1.f } };
            glBindBuffer(GL_ARRAY_BUFFER, ib);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
            glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
            glEnableVertexAttribArray(aVert);

            glGenBuffers(1, &vb.id);
            glBindBuffer(GL_ARRAY_BUFFER, vb);

            glVertexAttribPointer(aPosScale, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_TiledData), 0);
            glVertexAttribDivisor(aPosScale, 1);
            glEnableVertexAttribArray(aPosScale);

            glVertexAttribPointer(aTileSizeMapSize, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_TiledData), (GLvoid*)offsetof(Shader_TiledData, tileSize));
            glVertexAttribDivisor(aTileSizeMapSize, 1);
            glEnableVertexAttribArray(aTileSizeMapSize);

            glVertexAttribPointer(aTilingOffset, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_TiledData), (GLvoid*)offsetof(Shader_TiledData, tiling));
            glVertexAttribDivisor(aTilingOffset, 1);
            glEnableVertexAttribArray(aTilingOffset);

            glVertexAttribPointer(aDataSize, 2, GL_FLOAT, GL_FALSE, sizeof(Shader_TiledData), (GLvoid*)offsetof(Shader_TiledData, dataSize));
            glVertexAttribDivisor(aDataSize, 1);
            glEnableVertexAttribArray(aDataSize);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            CheckGLError();
        }

    void Shader_Tiled::Begin() {
        assert(!GameBase::instance->shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0 + 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glUniform1i(uTex0, 0);
        glUniform1i(uTex1, 1);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_Tiled::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_Tiled::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_TiledData) * count, data.get(), GL_STREAM_DRAW);

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, lastMiniTextureId);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        lastTextureId = lastMiniTextureId = 0;
        count = 0;
    }

    Shader_TiledData* Shader_Tiled::Draw(GLuint texId_TiledSample_, GLuint texId_TiledData_) {
        assert(GameBase::instance->shader == this);
        if (count + 1 > bufLen
            || (lastTextureId && lastTextureId != texId_TiledSample_)
            || (lastMiniTextureId && lastMiniTextureId != texId_TiledData_)) {
            Commit();
        }
        lastTextureId = texId_TiledSample_;
        lastMiniTextureId = texId_TiledData_;
        auto r = &data[count];
        count += 1;
        return r;
    }

    Shader_TiledData* Shader_Tiled::Draw(xx::Shared<xx::GLTexture> const& texTiledSample_, xx::Shared<xx::GLTiledTexture> const& texTiledData_) {
        auto r = Draw(texTiledSample_->id, texTiledData_->id);
        r->dataSize = { texTiledData_->unalignedSize.x, texTiledData_->unalignedSize.y };
        return r;
    }
}
