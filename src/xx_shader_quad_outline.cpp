#include "xx_gamebase.h"

namespace xx {
	
    void Shader_QuadOutlineData::Fill(UVRect rect_, XY pos_, XY anchor_, XY scale_
        , float radians_, float colorplus_, RGBA8 color_
        , RGBA8 outlineColor_, XY outlineThickness_) {
        pos = pos_;
        anchor = anchor_;
        scale = scale_;
        radians = radians_;
        colorplus = colorplus_;
        color = color_;
        texRect = rect_;
        outlineColor = outlineColor_;
        outlineThickness = outlineThickness_;
    }

    void Shader_QuadOutline::Init() {

        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	// screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosAnchor;
in vec4 aScaleRadiansColorplus;
in vec4 aColor;
in vec4 aTexRect;
in vec4 aOutlineColor;
in vec2 aOutlineThickness;

out vec2 vTexCoord;
flat out float vColorplus;
flat out vec4 vColor;
flat out vec4 vOutlineColor;
flat out vec2 vOutlineThickness;

void main() {
vec2 pos = aPosAnchor.xy;
vec2 anchor = aPosAnchor.zw;
vec2 scale = vec2(aScaleRadiansColorplus.x * aTexRect.z, aScaleRadiansColorplus.y * aTexRect.w);
float radians = aScaleRadiansColorplus.z;
vec2 offset = vec2((aVert.x - anchor.x) * scale.x, (aVert.y - anchor.y) * scale.y);

float c = cos(-radians);
float s = sin(-radians);
vec2 v = pos + vec2(
    dot(offset, vec2(c, s)),
    dot(offset, vec2(-s, c))
);

gl_Position = vec4(v * uCxy, 0, 1);
vColor = aColor;
vColorplus = aScaleRadiansColorplus.w;
vTexCoord = vec2(aTexRect.x + aVert.x * aTexRect.z, aTexRect.y + aTexRect.w - aVert.y * aTexRect.w);
vOutlineColor = aOutlineColor;
vOutlineThickness = aOutlineThickness;
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
flat in float vColorplus;
flat in vec4 vColor;
flat in vec4 vOutlineColor;
flat in vec2 vOutlineThickness;

out vec4 oColor;

void main() {
vec2 texSize = vec2(textureSize(uTex0, 0));

vec2 p1 = vec2(vTexCoord.x - vOutlineThickness.x, vTexCoord.y + vOutlineThickness.y);
vec2 p2 = vec2(vTexCoord.x + vOutlineThickness.x, vTexCoord.y + vOutlineThickness.y);
vec2 p3 = vec2(vTexCoord.x + vOutlineThickness.x, vTexCoord.y - vOutlineThickness.y);
vec2 p4 = vec2(vTexCoord.x - vOutlineThickness.x, vTexCoord.y - vOutlineThickness.y);
float a1 = vColor.w * texture(uTex0, p1 / texSize).w;
float a2 = vColor.w * texture(uTex0, p2 / texSize).w;
float a3 = vColor.w * texture(uTex0, p3 / texSize).w;
float a4 = vColor.w * texture(uTex0, p4 / texSize).w;

// vec2 p5 = vec2(vTexCoord.x, vTexCoord.y + vOutlineThickness.y);
// vec2 p6 = vec2(vTexCoord.x, vTexCoord.y - vOutlineThickness.y);
// vec2 p7 = vec2(vTexCoord.x + vOutlineThickness.x, vTexCoord.y);
// vec2 p8 = vec2(vTexCoord.x - vOutlineThickness.x, vTexCoord.y);
// float a5 = vColor.w * texture(uTex0, p5 / texSize).w;
// float a6 = vColor.w * texture(uTex0, p6 / texSize).w;
// float a7 = vColor.w * texture(uTex0, p7 / texSize).w;
// float a8 = vColor.w * texture(uTex0, p8 / texSize).w;

vec4 c = vColor * texture(uTex0, vTexCoord / texSize);
c = clamp(vec4( (c.x + 0.00001f) * vColorplus, (c.y + 0.00001f) * vColorplus, (c.z + 0.00001f) * vColorplus, c.w ), 0.f, 1.f);
float a = clamp(a1 + a2 + a3 + a4
// + a5 + a6 + a7 + a8
, 0.f, 1.f) - c.w;
if (a > 0.5f) oColor = vOutlineColor;
else oColor = c;
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");

        aVert = glGetAttribLocation(p, "aVert");
        aPosAnchor = glGetAttribLocation(p, "aPosAnchor");
        aScaleRadiansColorplus = glGetAttribLocation(p, "aScaleRadiansColorplus");
        aColor = glGetAttribLocation(p, "aColor");
        aTexRect = glGetAttribLocation(p, "aTexRect");
        aOutlineColor = glGetAttribLocation(p, "aOutlineColor");
        aOutlineThickness = glGetAttribLocation(p, "aOutlineThickness");
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

        glVertexAttribPointer(aPosAnchor, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadOutlineData), 0);  // offsetof(Shader_QuadOutlineData, pos
        glVertexAttribDivisor(aPosAnchor, 1);
        glEnableVertexAttribArray(aPosAnchor);

        glVertexAttribPointer(aScaleRadiansColorplus, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, scale));
        glVertexAttribDivisor(aScaleRadiansColorplus, 1);
        glEnableVertexAttribArray(aScaleRadiansColorplus);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        glVertexAttribPointer(aTexRect, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, texRect));
        glVertexAttribDivisor(aTexRect, 1);
        glEnableVertexAttribArray(aTexRect);

        glVertexAttribPointer(aOutlineColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, outlineColor));
        glVertexAttribDivisor(aOutlineColor, 1);
        glEnableVertexAttribArray(aOutlineColor);

        glVertexAttribPointer(aOutlineThickness, 2, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, outlineThickness));
        glVertexAttribDivisor(aOutlineThickness, 1);
        glEnableVertexAttribArray(aOutlineThickness);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    void Shader_QuadOutline::Begin() {
        assert(!GameBase::instance->shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_QuadOutline::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_QuadOutline::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_QuadOutlineData) * count, data.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        lastTextureId = 0;
        count = 0;
    }

    Shader_QuadOutlineData* Shader_QuadOutline::Alloc(GLuint texId, int32_t num_) {
        assert(GameBase::instance->shader == this);
        assert(num_ <= maxNums);
        if (count + num_ > maxNums || (lastTextureId && lastTextureId != texId)) {
            Commit();
        }
        lastTextureId = texId;
        auto r = &data[count];
        count += num_;
        return r;
    }

    Shader_QuadOutlineData* Shader_QuadOutline::Alloc(GLuint texId, UVRect const& uvRect, float outlineThickness, RGBA8 outlineColor) {
        auto q = Alloc(texId, 1);
        q->pos = {};
        q->anchor = 0.5f;
        q->scale = 1;
        q->radians = 0;
        q->colorplus = 1.f;
        q->color = xx::RGBA8_White;
        q->texRect.x = uint16_t(uvRect.x - outlineThickness - 1.f);
        q->texRect.y = uint16_t(uvRect.y - outlineThickness - 1.f);
        q->texRect.w = uint16_t(uvRect.w + (outlineThickness + 1.f) * 2.f);
        q->texRect.h = uint16_t(uvRect.h + (outlineThickness + 1.f) * 2.f);
        q->outlineColor = outlineColor;
        q->outlineThickness = outlineThickness;
        return q;
    }

}
 