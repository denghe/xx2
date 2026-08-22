#include "xx_gamebase.h"

namespace xx {
	
    void Shader_HPBarData::Fill(XY pos_, XY size_
        , xx::RGBA8 colorBorder_
        , xx::RGBA8 colorBackground_
        , xx::RGBA8 colorForeground_
        , float value_) {
        pos = pos_;
        size = size_;
        colorBorder = colorBorder_;
        colorBackground = colorBackground_;
        colorForeground = colorForeground_;
        value = value_;
    }

    void Shader_HPBar::Init() {

        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	            // screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosSize;
in vec4 aColorBorder;
in vec4 aColorBackground;
in vec4 aColorForeground;
in float aValue;

out vec2 vTexCoord;
flat out vec2 vSize;
flat out vec4 vColorBorder;
flat out vec4 vColorBackground;
flat out vec4 vColorForeground;
flat out float vValue;

void main() {
    vSize = aPosSize.zw;
    vColorBorder = aColorBorder;
    vColorBackground = aColorBackground;
    vColorForeground = aColorForeground;
    vValue = aValue;

    vec2 pos = aPosSize.xy;
    vec2 siz = aPosSize.zw;
    gl_Position = vec4((pos + siz * aVert) * uCxy, 0.f, 1.f);
    vTexCoord = siz * aVert;
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;          // mediump draw border has issue

in vec2 vTexCoord;
flat in vec2 vSize;
flat in vec4 vColorBorder;
flat in vec4 vColorBackground;
flat in vec4 vColorForeground;
flat in float vValue;

out vec4 oColor;

void main() {
    if (vTexCoord.x < 2.f || vTexCoord.x >= vSize.x - 2.f
        || vTexCoord.y < 2.f || vTexCoord.y >= vSize.y - 2.f) {
        oColor = vColorBorder;
    } else if (vTexCoord.x / vSize.x <= vValue) {
        oColor = vColorForeground;
    } else {
        oColor = vColorBackground;
    }
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        aVert = glGetAttribLocation(p, "aVert");
        aPosSize = glGetAttribLocation(p, "aPosSize");
        aColorBorder = glGetAttribLocation(p, "aColorBorder");
        aColorBackground = glGetAttribLocation(p, "aColorBackground");
        aColorForeground = glGetAttribLocation(p, "aColorForeground");
        aValue = glGetAttribLocation(p, "aValue");
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

        glVertexAttribPointer(aPosSize, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_HPBarData), 0);
        glVertexAttribDivisor(aPosSize, 1);
        glEnableVertexAttribArray(aPosSize);

        glVertexAttribPointer(aColorBorder, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_HPBarData), (GLvoid*)offsetof(Shader_HPBarData, colorBorder));
        glVertexAttribDivisor(aColorBorder, 1);
        glEnableVertexAttribArray(aColorBorder);

        glVertexAttribPointer(aColorBackground, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_HPBarData), (GLvoid*)offsetof(Shader_HPBarData, colorBackground));
        glVertexAttribDivisor(aColorBackground, 1);
        glEnableVertexAttribArray(aColorBackground);

        glVertexAttribPointer(aColorForeground, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_HPBarData), (GLvoid*)offsetof(Shader_HPBarData, colorForeground));
        glVertexAttribDivisor(aColorForeground, 1);
        glEnableVertexAttribArray(aColorForeground);

        glVertexAttribPointer(aValue, 1, GL_FLOAT, GL_FALSE, sizeof(Shader_HPBarData), (GLvoid*)offsetof(Shader_HPBarData, value));
        glVertexAttribDivisor(aValue, 1);
        glEnableVertexAttribArray(aValue);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    void Shader_HPBar::Begin() {
        assert(!GameBase::instance->shader);
        glUseProgram(p);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_HPBar::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_HPBar::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_HPBarData) * count, data.get(), GL_STREAM_DRAW);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        count = 0;
    }

    Shader_HPBarData* Shader_HPBar::Alloc() {
        assert(GameBase::instance->shader == this);
        if (count + 1 > maxNums) {
            Commit();
        }
        auto r = &data[count];
        count += 1;
        return r;
    }

}
 