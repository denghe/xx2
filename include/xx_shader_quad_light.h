#pragma once
#include "xx_shader_quad.h"

namespace xx {

    struct Shader_QuadLight : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, uTex1{ -1 }, aVert{ -1 }, aPosAnchor{ -1 }, aScaleRadiansColorplus{ -1 }, aColor{ -1 }, aTexRect{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 20000 };
        GLuint lastTextureId{}, lastLightTextureId{};
        std::unique_ptr<Shader_QuadData[]> data = std::make_unique_for_overwrite<Shader_QuadData[]>(maxNums);
        int32_t count{};

        void Init() {

            v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	// screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosAnchor;
in vec4 aScaleRadiansColorplus;
in vec4 aColor;
in vec4 aTexRect;

out vec2 vTexCoord;
flat out float vColorplus;
flat out vec4 vColor;

void main() {
    vec2 pos = aPosAnchor.xy;
    vec2 anchor = aPosAnchor.zw;
    vec2 scale = vec2(aScaleRadiansColorplus.x * aTexRect.z, aScaleRadiansColorplus.y * aTexRect.w);
    float radians = aScaleRadiansColorplus.z;
    vec2 offset = vec2((aVert.x - anchor.x) * scale.x, (aVert.y - anchor.y) * scale.y);

    float c = cos(radians);
    float s = sin(radians);
    vec2 v = pos + vec2(
       dot(offset, vec2(c, s)),
       dot(offset, vec2(-s, c))
    );

    gl_Position = vec4(v * uCxy, 0, 1);
    vColor = aColor;
    vColorplus = aScaleRadiansColorplus.w;
    vTexCoord = vec2(aTexRect.x + aVert.x * aTexRect.z, aTexRect.y + aTexRect.w - aVert.y * aTexRect.w);
})"sv });

            f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;
uniform sampler2D uTex1;

in vec2 vTexCoord;
flat in float vColorplus;
flat in vec4 vColor;

out vec4 oColor;

void main() {
    vec2 uv = vTexCoord / vec2(textureSize(uTex0, 0));
    vec4 c = vColor * texture(uTex0, uv);
    vec4 c1 = texture(uTex1, uv);
    oColor = vec4( (c.x + 0.00001f) * vColorplus * c1.x
                 , (c.y + 0.00001f) * vColorplus * c1.y
                 , (c.z + 0.00001f) * vColorplus * c1.z, c.w );
})"sv });

            p = LinkGLProgram(v, f);

            uCxy = glGetUniformLocation(p, "uCxy");
            uTex0 = glGetUniformLocation(p, "uTex0");
            uTex1 = glGetUniformLocation(p, "uTex1");

            aVert = glGetAttribLocation(p, "aVert");
            aPosAnchor = glGetAttribLocation(p, "aPosAnchor");
            aScaleRadiansColorplus = glGetAttribLocation(p, "aScaleRadiansColorplus");
            aColor = glGetAttribLocation(p, "aColor");
            aTexRect = glGetAttribLocation(p, "aTexRect");
            CheckGLError();

            glGenVertexArrays(1, &va.id);
            glGenBuffers(1, &ib.id);
            glGenBuffers(1, &vb.id);

            glBindVertexArray(va);

            static const XY verts[4] = { { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f } };
            glBindBuffer(GL_ARRAY_BUFFER, ib);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
            glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
            glEnableVertexAttribArray(aVert);

            glBindBuffer(GL_ARRAY_BUFFER, vb);

            glVertexAttribPointer(aPosAnchor, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, pos));
            glVertexAttribDivisor(aPosAnchor, 1);
            glEnableVertexAttribArray(aPosAnchor);

            glVertexAttribPointer(aScaleRadiansColorplus, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, scale));
            glVertexAttribDivisor(aScaleRadiansColorplus, 1);
            glEnableVertexAttribArray(aScaleRadiansColorplus);

            glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, color));
            glVertexAttribDivisor(aColor, 1);
            glEnableVertexAttribArray(aColor);

            glVertexAttribPointer(aTexRect, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, texRect));
            glVertexAttribDivisor(aTexRect, 1);
            glEnableVertexAttribArray(aTexRect);

            glBindVertexArray(0);
            CheckGLError();
        }

        virtual void Begin() override {
            assert(!GameBase::instance->shader);
            assert(lastTextureId == 0);
            assert(lastLightTextureId == 0);
            assert(count == 0);
            glUseProgram(p);
            glUniform1i(uTex0, 0);
            glUniform1i(uTex1, 1);
            glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
            glBindVertexArray(va);
        }

        virtual void End() override {
            assert(GameBase::instance->shader == this);
            if (count) {
                Commit();
            }
        }

        void Commit() {
            glBindBuffer(GL_ARRAY_BUFFER, vb);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_QuadData) * count, data.get(), GL_STREAM_DRAW);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, lastTextureId);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, lastLightTextureId);
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
            CheckGLError();

            GameBase::instance->drawVerts += count * 6;
            GameBase::instance->drawCall += 1;

            lastTextureId = 0;
            lastLightTextureId = 0;
            count = 0;
        }

        XX_INLINE Shader_QuadData* Alloc(GLuint texId, GLuint lightTexId, int32_t num_) {
            assert(GameBase::instance->shader == this);
            assert(num_ <= maxNums);
            if (count + num_ > maxNums
                || (lastTextureId && lastTextureId != texId)
                || (lastLightTextureId && lastLightTextureId != lightTexId)
                ) {
                Commit();
            }
            lastTextureId = texId;
            lastLightTextureId = lightTexId;
            auto r = &data[count];
            count += num_;
            return r;
        }

        XX_INLINE void Draw(Shared<GLTexture> tex, Shared<GLTexture> lightTex, RGBA8 color = xx::RGBA8_White, float colorplus = 1.f) {
            Alloc(*tex, *lightTex, 1)->Fill(*tex, 0, 0.5f, 1.f, 0, colorplus, color);
        }
    };

}
