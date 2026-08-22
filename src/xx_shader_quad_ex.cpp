#include "xx_gamebase.h"

namespace xx {

    void Shader_QuadExData::Fill(UVRect rect_, XY pos_, XY anchor_, XY scale_
        , float radians_, float colorplus_, RGBA8 color_
        , XY tiling_, XY offset_) {
        pos = pos_;
        anchor = anchor_;
        scale = scale_;
        radians = radians_;
        colorplus = colorplus_;
        color = color_;
        texRect = rect_;
        tiling = tiling_;
        offset = offset_;
    }

    void Shader_QuadEx::Init() {

        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	// screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosAnchor;
in vec4 aScaleRadiansColorplus;
in vec4 aColor;
in vec4 aTexRect;
in vec4 aTilingOffset;

out vec2 vTexCoord;
flat out float vColorplus;
flat out vec4 vColor;
flat out vec4 vTexRect;

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
vTexCoord = (vec2(aVert.x * aTexRect.z, aTexRect.w - aVert.y * aTexRect.w) + aTilingOffset.zw) * aTilingOffset.xy;
vTexRect = aTexRect;
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
flat in float vColorplus;
flat in vec4 vColor;
flat in vec4 vTexRect;

out vec4 oColor;

void main() {
vec2 uv = vTexRect.xy + mod(vTexCoord, vTexRect.zw);
vec4 c = vColor * texture(uTex0, uv / vec2(textureSize(uTex0, 0)));
oColor = vec4( (c.x + 0.00001f) * vColorplus, (c.y + 0.00001f) * vColorplus, (c.z + 0.00001f) * vColorplus, c.w );
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");

        aVert = glGetAttribLocation(p, "aVert");
        aPosAnchor = glGetAttribLocation(p, "aPosAnchor");
        aScaleRadiansColorplus = glGetAttribLocation(p, "aScaleRadiansColorplus");
        aColor = glGetAttribLocation(p, "aColor");
        aTexRect = glGetAttribLocation(p, "aTexRect");
        aTilingOffset = glGetAttribLocation(p, "aTilingOffset");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glBindVertexArray(va);

        glGenBuffers(1, (GLuint*)&ib);
        static const XY verts[4] = { { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f } };
        glBindBuffer(GL_ARRAY_BUFFER, ib);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
        glEnableVertexAttribArray(aVert);

        glGenBuffers(1, (GLuint*)&vb);
        glBindBuffer(GL_ARRAY_BUFFER, vb);

        glVertexAttribPointer(aPosAnchor, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadExData), 0);  // offsetof(Shader_QuadExData, pos
        glVertexAttribDivisor(aPosAnchor, 1);
        glEnableVertexAttribArray(aPosAnchor);

        glVertexAttribPointer(aScaleRadiansColorplus, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadExData), (GLvoid*)offsetof(Shader_QuadExData, scale));
        glVertexAttribDivisor(aScaleRadiansColorplus, 1);
        glEnableVertexAttribArray(aScaleRadiansColorplus);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadExData), (GLvoid*)offsetof(Shader_QuadExData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        glVertexAttribPointer(aTexRect, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Shader_QuadExData), (GLvoid*)offsetof(Shader_QuadExData, texRect));
        glVertexAttribDivisor(aTexRect, 1);
        glEnableVertexAttribArray(aTexRect);

        glVertexAttribPointer(aTilingOffset, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadExData), (GLvoid*)offsetof(Shader_QuadExData, tiling));
        glVertexAttribDivisor(aTilingOffset, 1);
        glEnableVertexAttribArray(aTilingOffset);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    void Shader_QuadEx::Begin() {
        assert(!GameBase::instance->shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_QuadEx::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_QuadEx::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_QuadExData) * count, data.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        lastTextureId = 0;
        count = 0;
    }

    Shader_QuadExData* Shader_QuadEx::Alloc(GLuint texId, int32_t numQuads) {
        assert(GameBase::instance->shader == this);
        assert(numQuads <= maxNums);
        if (count + numQuads > maxNums || (lastTextureId && lastTextureId != texId)) {
            Commit();
        }
        lastTextureId = texId;
        auto r = &data[count];
        count += numQuads;
        return r;
    }

    void Shader_QuadEx::Draw(GLuint texId_, UVRect rect_, XY pos_, XY anchor_
        , XY scale_, XY tiling_, XY offset_, float radians_, float colorplus_, RGBA8 color_) {
        Alloc(texId_, 1)->Fill(rect_, pos_, anchor_, scale_, radians_, colorplus_, color_, tiling_, offset_);
    }

    void Shader_QuadEx::DrawTinyFrame(TinyFrame& tinyFrame_, XY pos_, XY anchor_
        , XY scale_, XY tiling_, XY offset_, float radians_, float colorplus_, RGBA8 color_) {
        Alloc(tinyFrame_, 1)->Fill(tinyFrame_, pos_, anchor_, scale_, radians_, colorplus_, color_, tiling_, offset_);
    }

    void Shader_QuadEx::DrawFrame(Frame& frame_, XY pos_, XY scale_, XY tiling_, XY offset_, float radians_, float colorplus_, RGBA8 color_) {
        Alloc(frame_.tex->id, 1)->Fill(frame_, pos_, frame_.anchor, scale_, radians_, colorplus_, color_, tiling_, offset_);
    }

}
 