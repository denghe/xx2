#include "xx_gamebase.h"

namespace xx {
	
    void Shader_Line::Init() {
        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;
uniform vec2 uCxy;	// screen center coordinate

in vec2 aPos;
in vec4 aColor;

out vec4 vColor;

void main() {
	gl_Position = vec4(aPos * uCxy, 0, 1);
	vColor = aColor;
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;

in vec4 vColor;

out vec4 oColor;

void main() {
	oColor = vColor;
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");

        aPos = glGetAttribLocation(p, "aPos");
        aColor = glGetAttribLocation(p, "aColor");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glGenBuffers(1, &vb.id);
        glGenBuffers(1, &ib.id);

        glBindVertexArray(va);

        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, sizeof(Shader_LineData), 0);
        glEnableVertexAttribArray(aPos);
        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_LineData), (GLvoid*)offsetof(Shader_LineData, r));
        glEnableVertexAttribArray(aColor);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

        glBindVertexArray(0);
        CheckGLError();
    }

    void Shader_Line::Begin() {
        assert(!GameBase::instance->shader);
        assert(pointsCount == 0 && indexsCount == 0);
        glUseProgram(p);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_Line::End() {
        assert(GameBase::instance->shader == this);
        if (indexsCount) {
            assert(pointsCount);
            Commit();
        }
    }

    void Shader_Line::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_LineData) * pointsCount, points.get(), GL_STREAM_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indexsCount, indexs.get(), GL_STREAM_DRAW);

        glDrawElements(GL_LINE_STRIP, indexsCount, GL_UNSIGNED_SHORT, 0);

		GameBase::instance->drawVerts += indexsCount;
        GameBase::instance->drawCall += 1;

        pointsCount = 0;
        indexsCount = 0;
    }

    Shader_LineData* Shader_Line::Alloc(int32_t num_) {
        assert(num_ <= maxVertNums);
        auto c = pointsCount + num_;
        if (c > maxVertNums / 2) {
            Commit();
            c = num_;
        }
        auto rtv = &points[pointsCount];
        for (size_t i = pointsCount; i < c; ++i) {
            indexs[indexsCount++] = i;
        }
        indexs[indexsCount++] = 65535;	// primitive restart
        assert(indexsCount <= maxIndexNums);
        pointsCount = c;
        return rtv;
    }

    void Shader_Line::Fill(Shader_LineData const* buf_, int32_t num_) {
        memcpy(Alloc(num_), buf_, sizeof(Shader_LineData) * num_);
    }

    void Shader_Line::DrawCircle(XY centerPos_, float radius_, std::optional<float> radians_, int32_t segments_, xx::RGBA8 color_) {
        auto len = segments_ + 1;
        float a{};
        if (radians_.has_value()) {
            ++len;
			a = -*radians_;
        }
		auto points = Alloc(len);
        auto coef = 2.0f * (float)M_PI / segments_;
        for (int32_t i = 0; i <= segments_; ++i) {
			auto& p = points[i];
            auto rads = i * coef + a;
            p.x = radius_ * cosf(rads) + centerPos_.x;
            p.y = radius_ * sinf(rads) + centerPos_.y;
            (uint32_t&)p.r = (uint32_t&)color_;
        }
        if (radians_.has_value()) {
            auto& p = points[len - 1];
            p.x = centerPos_.x;
            p.y = centerPos_.y;
            (uint32_t&)p.r = (uint32_t&)color_;
        }
    }

}
 