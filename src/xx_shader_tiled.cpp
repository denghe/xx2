#include "xx_gamebase.h"

namespace xx {

    void Shader_TiledData::Fill(XY pos_, XY scale_, XY tileSize_, XY mapSize_) {
        pos = pos_;
        scale = scale_;
        tileSize = tileSize_;
        if (mapSize_.x > 0 || mapSize_.y > 0) {
            mapSize = mapSize_;
        }
    }

    void Shader_Tiled::Init() {

            v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	            // screen center coordinate

in vec2 aVert;	                // fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosScale;
in vec4 aTileSizeMapSize;

out vec2 vOutUV;                // value range: 0 ~ mapSize * tileSize
flat out vec4 vTileSizeOutSize;

void main() {

    vec2 pos = aPosScale.xy;
    vec2 scale = aPosScale.zw;
    vec2 tileSize = aTileSizeMapSize.xy;
    vec2 mapSize = aTileSizeMapSize.zw;
    vec2 outSize = mapSize * tileSize;
    vec2 outOffset = outSize * -0.5f;

    vOutUV = vec2(outSize.x * aVert.x, outSize.y - outSize.y * aVert.y);
    gl_Position = vec4((pos + vOutUV + outOffset) * scale * uCxy, 0.f, 1.f);
    vTileSizeOutSize = vec4(tileSize, outSize);
})"sv });

            f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;        // tiled samples
uniform usampler2D uTex1;       // tild data uint8+8[]    .x: tiled index  .y: 0~255 map to 0~1 color multiplier

in vec2 vOutUV;
flat in vec4 vTileSizeOutSize;

out vec4 oColor;

void main() {
    vec2 tileSize = vTileSizeOutSize.xy;
    vec2 outSize = vTileSizeOutSize.zw;
    ivec2 mapIndex = ivec2(vOutUV / tileSize);
    uvec4 data = texelFetch(uTex1, mapIndex, 0);
    float colorMultiplier = float(data.y) / 255.f;
    //oColor = vec4( float(data.x) / 9.f, 0, 0, 1 );
    float tileIndex = float(data.x);
    vec2 tileUV = vOutUV - vec2(mapIndex) * tileSize;
    vec2 uv = vec2(tileUV.x + tileIndex * tileSize.x, tileSize.y - mod(tileUV.y, tileSize.y));
    vec4 c = texture(uTex0, uv / vec2(textureSize(uTex0, 0)));
    oColor = vec4( c.x * colorMultiplier, c.y * colorMultiplier, c.z * colorMultiplier, c.w * colorMultiplier );
})"sv });

            p = LinkGLProgram(v, f);

            uCxy = glGetUniformLocation(p, "uCxy");
            uTex0 = glGetUniformLocation(p, "uTex0");
            uTex1 = glGetUniformLocation(p, "uTex1");

            aVert = glGetAttribLocation(p, "aVert");
            aPosScale = glGetAttribLocation(p, "aPosScale");
            aTileSizeMapSize = glGetAttribLocation(p, "aTileSizeMapSize");
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

    Shader_TiledData* Shader_Tiled::Alloc(GLuint texId_TiledSample_, GLuint texId_TiledData_) {
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

    Shader_TiledData* Shader_Tiled::Alloc(xx::Shared<xx::GLTexture> const& texTiledSample_, xx::Shared<xx::GLTiledTexture> const& texTiledData_) {
        auto r = Alloc(texTiledSample_->id, texTiledData_->id);
        r->mapSize = { texTiledData_->mapSize.x, texTiledData_->mapSize.y };
        return r;
    }
}
