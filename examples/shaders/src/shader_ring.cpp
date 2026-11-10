#include "pch.h"
#include "shader_ring.h"
#include "xx_gamebase.h"

namespace xx {

	void Shader_Ring::Init() {

		v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;								// 2 / window_half_size for convert screen pos to -1 ~ 1
in vec2 aVert;									// circle vertex array iter
in vec4 aPosRadiusBorderWidth;
in vec4 aColor;

flat out vec4 vColor;
flat out float vBorderWidth;
out vec2 vUV;

void main() {
	vec2 pos = aPosRadiusBorderWidth.xy + aVert * aPosRadiusBorderWidth.z;
	gl_Position = vec4(pos * uCxy, 0, 1);
    vColor = aColor;
    vBorderWidth = aPosRadiusBorderWidth.w;
	vUV = aVert * 1.03f;						// * ???f for anti-aliasing
})"sv });

		f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision lowp float;
in vec2 vUV;
flat in vec4 vColor;
flat in float vBorderWidth;
out vec4 oColor;

void main() {
	float d = length(vUV);
	if (d > 1.f) {
		discard;
	} else if (d > 1.f - vBorderWidth) {
	    oColor.a = vColor.a;
	} else {
	    oColor.a = vColor.a * d * 0.6f;
	}
    oColor.xyz = vColor.xyz * oColor.a;			// premultiply alpha
})"sv });

		p = LinkGLProgram(v, f);

		uCxy = glGetUniformLocation(p, "uCxy");

		aVert = glGetAttribLocation(p, "aVert");
		aPosRadiusBorderWidth = glGetAttribLocation(p, "aPosRadiusBorderWidth");
		aColor = glGetAttribLocation(p, "aColor");
		CheckGLError();

		glGenVertexArrays(1, &va.id);
		glGenBuffers(1, &ib.id);
		glGenBuffers(1, &vb.id);

		glBindVertexArray(va);

		glBindBuffer(GL_ARRAY_BUFFER, ib);
		// verts add circle points
		List<XY> verts;
		verts.Emplace(0, 0);	// circle center
		static constexpr double angleStep = 2.0 * M_PI / numCircleMeshSegaments;
		for (int i = 0; i < numCircleMeshSegaments; ++i) {
			double angle = i * angleStep;
			verts.Emplace(std::cos(angle), std::sin(angle));
		}
		verts.Emplace(verts[1]);	// close circle
		assert(verts.len == numCircleMeshSegaments + 2);
		glBufferData(GL_ARRAY_BUFFER, verts.ByteLen(), verts.Buf(), GL_STATIC_DRAW);
		glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
		glEnableVertexAttribArray(aVert);

		glBindBuffer(GL_ARRAY_BUFFER, vb);

		glVertexAttribPointer(aPosRadiusBorderWidth, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_RingData), (GLvoid*)offsetof(Shader_RingData, pos));
		glVertexAttribDivisor(aPosRadiusBorderWidth, 1);
		glEnableVertexAttribArray(aPosRadiusBorderWidth);

		glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_RingData), (GLvoid*)offsetof(Shader_RingData, color));
		glVertexAttribDivisor(aColor, 1);
		glEnableVertexAttribArray(aColor);

		glBindVertexArray(0);
		CheckGLError();
	}

	void Shader_Ring::Begin() {
		assert(!GameBase::instance->shader);
		assert(count == 0);
		glUseProgram(p);
		glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
		glBindVertexArray(va);
	}

	void Shader_Ring::End() {
		assert(GameBase::instance->shader == this);
		if (count) {
			Commit();
		}
	}

	void Shader_Ring::Commit() {
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_RingData) * count, data.get(), GL_STREAM_DRAW);

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, numCircleMeshSegaments + 2, count);
		CheckGLError();

		GameBase::instance->drawVerts += count * (3 * numCircleMeshSegaments);
		GameBase::instance->drawCall += 1;

		count = 0;
	}

	Shader_RingData* Shader_Ring::Alloc(int32_t num_) {
		assert(GameBase::instance->shader == this);
		assert(num_ <= maxNums);
		if (count + num_ > maxNums) {
			Commit();
		}
		auto r = &data[count];
		count += num_;
		return r;
	}

	void Shader_Ring::Draw(XY pos_, float radius_, float borderWidth_, RGBA8 color_) {
		auto d = Alloc(1);
		d->pos = pos_;
		d->radius = radius_;
		d->borderWidth = borderWidth_;
		d->color = color_;
	}

}
