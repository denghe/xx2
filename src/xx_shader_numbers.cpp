#include "xx_gamebase.h"

namespace xx {
	
    int32_t Shader_NumbersData::FillTxt(std::string_view n_) {
        assert(n_.size());
        auto buf = n_.data();
        auto len = n_.size();
        auto tar = (uint8_t*)&numbers;
        for (size_t i = 0; i < len; ++i) {
            tar[i] = buf[i] - '0';
        }
        tar[15] = (uint8_t)len;
		return (int32_t)len;
    }

    int32_t Shader_NumbersData::FillTxt(int32_t n_, bool includeSignal_) {
        std::string s;
        if (includeSignal_) {
            if (n_ >= 0) {
                s.push_back((char)58);
                Append(s, (uint32_t)n_);
            }
            else {
                s.push_back((char)59);
                Append(s, (uint32_t)-n_);
            }
        }
        else {
            Append(s, std::abs(n_));
        }
        return FillTxt(s);
    }

    void Shader_NumbersData::FillBase(XY pos_, XY scale_, RGBA8 color_) {
        pos = pos_;
        scale = scale_;
        color = color_;
    }

    int32_t Shader_NumbersData::Fill(XY pos_, XY scale_, RGBA8 color_, std::string_view n_) {
        FillBase(pos_, scale_, color_);
        return FillTxt(n_);
    }

    int32_t Shader_NumbersData::Fill(XY pos_, XY scale_, RGBA8 color_, int32_t n_, bool includeSignal_) {
        FillBase(pos_, scale_, color_);
        return FillTxt(n_, includeSignal_);
    }


    void Shader_Numbers::Init() {

        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	            // screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosScale;
in vec4 aColor;
in ivec4 aNumbers;

out vec2 vTexCoord;
out vec4 vColor;
flat out ivec4 vNumbers;

void main() {
    vec2 pos = aPosScale.xy;
    vec2 scale = aPosScale.zw;
    vec2 offset = aVert * scale;
    float len = float(aNumbers.w >> 24);
    vec2 size = vec2(9.f * len, 13.f);
    gl_Position = vec4((pos + size * offset) * uCxy, 0.f, 1.f);
    vColor = aColor;
    vTexCoord = vec2(9.f * len * aVert.x, 13.f - 13.f * aVert.y);
    vNumbers = aNumbers;
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
in vec4 vColor;
flat in ivec4 vNumbers;

out vec4 oColor;

void main() {
    uint i = uint(vTexCoord.x * 0.1111111f);        // / 9
    uint rIdx = (i >> 2);                           // / 4
    uint cIdx = i - (rIdx << 2);                    // * 4
    uint n4 = uint(vNumbers[rIdx]);
    uint n = (n4 >> (cIdx << 3)) & 0xffu;           // * 8
    vec2 uv = vec2(10.f * float(n) + vTexCoord.x - float(i) * 9.f, vTexCoord.y);
    //vec2 uv = vTexCoord;
    oColor = vColor * texture(uTex0, uv / vec2(120.f, 13.f));
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");

        aVert = glGetAttribLocation(p, "aVert");
        aPosScale = glGetAttribLocation(p, "aPosScale");
        aColor = glGetAttribLocation(p, "aColor");
        aNumbers = glGetAttribLocation(p, "aNumbers");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glBindVertexArray(va);

        glGenBuffers(1, &ib.id);
        static const XY verts[4] = { { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f } };
        glBindBuffer(GL_ARRAY_BUFFER, ib);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
        glEnableVertexAttribArray(aVert);

        glGenBuffers(1, &vb.id);
        glBindBuffer(GL_ARRAY_BUFFER, vb);

        glVertexAttribPointer(aPosScale, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_NumbersData), 0);
        glVertexAttribDivisor(aPosScale, 1);
        glEnableVertexAttribArray(aPosScale);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_NumbersData), (GLvoid*)offsetof(Shader_NumbersData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        // "I" Pointer for int
        glVertexAttribIPointer(aNumbers, 4, GL_INT, sizeof(Shader_NumbersData), (GLvoid*)offsetof(Shader_NumbersData, numbers));
        glVertexAttribDivisor(aNumbers, 1);
        glEnableVertexAttribArray(aNumbers);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    void Shader_Numbers::Begin() {
        assert(!GameBase::instance->shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_Numbers::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_Numbers::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_NumbersData) * count, data.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, GameBase::instance->embed.png_numbers);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        count = 0;
    }

    Shader_NumbersData* Shader_Numbers::Alloc(int32_t num_) {
        assert(GameBase::instance->shader == this);
        if (count + num_ > maxNums) {
            Commit();
        }
        auto r = &data[count];
        count += num_;
        return r;
    }

    void Shader_Numbers::Draw(Shader_NumbersData const* buf_, int32_t len_) {
        auto ptr = Alloc(len_);
        memcpy(ptr, buf_, sizeof(Shader_NumbersData) * len_);
    }

    void Shader_Numbers::Draw(Shader_NumbersData const& d_) {
        Draw(&d_, 1);
    }

}
 