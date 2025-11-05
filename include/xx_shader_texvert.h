#pragma once
#include "xx_shader_spine.h"

namespace xx {

    // tex data: float * 8
    // pos(2) + uv(2) + color(4)
    struct Shader_TexVertElement {
        float x, y, u, v, r, g, b, a;
    };

    struct Shader_TexVertData {
        XY pos{}, scale{};
        int32_t frameIndex{};
    };

    struct Shader_TexVert : Shader {
        using Shader::Shader;
        GLint uTex0{ -1 }, uTex1{ -1 }, aVertIndex{ -1 }, aPosScale{ -1 }, aFrameIndex{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t vertCap{ 8192 };
        static constexpr int32_t maxNums{ 100000 };
        GLuint lastTextureId{}, lastVertTextureId{}, numVerts{};
        std::unique_ptr<Shader_TexVertData[]> data = std::make_unique_for_overwrite<Shader_TexVertData[]>(maxNums);
        int32_t count{};

        void Init() {

            v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	                                            // screen center coordinate
uniform sampler2D uTex1;                                        // Shader_TexVertElement[][]

in int aVertIndex;                                              // vert index: 0, 1, 2, ...... 8191

in vec4 aPosScale;
in int aFrameIndex;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    vec2 pos = aPosScale.xy;
    vec2 scale = aPosScale.zw;
    int offset = aVertIndex * 2;
    vec4 xyuv = texelFetch(uTex1, ivec2(offset + 0, aFrameIndex), 0);
    vColor    = texelFetch(uTex1, ivec2(offset + 1, aFrameIndex), 0);
    vTexCoord = xyuv.zw;
    gl_Position = vec4((pos + xyuv.xy * scale) * uCxy, 0, 1);
})"sv });

            f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;
uniform sampler2D uTex0;

in vec2 vTexCoord;
in vec4 vColor;

out vec4 oColor;

void main() {
    oColor = vColor * texture(uTex0, vTexCoord / vec2(textureSize(uTex0, 0)));
})"sv });

            p = LinkGLProgram(v, f);

            uCxy = glGetUniformLocation(p, "uCxy");
            uTex0 = glGetUniformLocation(p, "uTex0");
            uTex1 = glGetUniformLocation(p, "uTex1");

            aVertIndex = glGetAttribLocation(p, "aVertIndex");
            aPosScale = glGetAttribLocation(p, "aPosScale");
            aFrameIndex = glGetAttribLocation(p, "aFrameIndex");
            CheckGLError();

            glGenVertexArrays(1, &va.id);
            glGenBuffers(1, &ib.id);
            glGenBuffers(1, &vb.id);

            glBindVertexArray(va);

            auto d = std::make_unique<int[]>(vertCap);
            for (int i = 0; i < vertCap; ++i) d[i] = i;
            glBindBuffer(GL_ARRAY_BUFFER, ib);
            glBufferData(GL_ARRAY_BUFFER, vertCap * 4, d.get(), GL_STATIC_DRAW);
            glVertexAttribIPointer(aVertIndex, 1, GL_INT, sizeof(int), 0);
            glEnableVertexAttribArray(aVertIndex);

            glBindBuffer(GL_ARRAY_BUFFER, vb);

            glVertexAttribPointer(aPosScale, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_TexVertData), (GLvoid*)offsetof(Shader_TexVertData, pos));
            glVertexAttribDivisor(aPosScale, 1);
            glEnableVertexAttribArray(aPosScale);

            glVertexAttribIPointer(aFrameIndex, 1, GL_INT, sizeof(Shader_TexVertData), (GLvoid*)offsetof(Shader_TexVertData, frameIndex));
            glVertexAttribDivisor(aFrameIndex, 1);
            glEnableVertexAttribArray(aFrameIndex);

            glBindVertexArray(0);
            CheckGLError();
        }

        virtual void Begin() override {
            assert(!GameBase::instance->shader);
            assert(lastTextureId == 0);
            assert(lastVertTextureId == 0);
            assert(count == 0);
            glUseProgram(p);
            glActiveTexture(GL_TEXTURE0 + 0);
            glActiveTexture(GL_TEXTURE0 + 1);
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
            glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_TexVertData) * count, data.get(), GL_STREAM_DRAW);

            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, lastTextureId);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, lastVertTextureId);

            glDrawArraysInstanced(GL_TRIANGLES, 0, numVerts, count);
            CheckGLError();

            GameBase::instance->drawVerts += count * numVerts;
            GameBase::instance->drawCall += 1;

            lastTextureId = 0;
            lastVertTextureId = 0;
            count = 0;
        }

        Shader_TexVertData* Alloc(Shared<GLTexture> tex_, Shared<GLVertTexture> const& vertTex_, int32_t num_) {
            assert(GameBase::instance->shader == this);
            assert(num_ <= maxNums);
            auto texId = tex_->id;
            auto vertTexId = vertTex_->id;
            if (count + num_ > maxNums
                || (lastTextureId && lastTextureId != texId) 
                || (lastVertTextureId && lastVertTextureId != vertTexId)) {
                Commit();
            }
            lastTextureId = texId;
            lastVertTextureId = vertTexId;
            numVerts = vertTex_->numVerts;
            assert(vertCap >= numVerts);
            auto r = &data[count];
            count += num_;
            return r;
        }

    };

}
