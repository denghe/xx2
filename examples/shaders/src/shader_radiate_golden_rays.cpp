#include "pch.h"
#include "shader_radiate_golden_rays.h"
#include "xx_gamebase.h"

namespace xx {

	void Shader_RadiateGoldenRays::Init() {

		v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	// screen center coordinate
in vec2 aVert;
in vec4 aPosTimeRadius;
out vec2 vUV;
flat out float vTime;

void main() {
	vec2 pos = aPosTimeRadius.xy + aVert * aPosTimeRadius.w;
    gl_Position = vec4(pos * uCxy, 0, 1);
	vUV = aVert;
	vTime = aPosTimeRadius.z;
})"sv });


		f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision lowp float;
in vec2 vUV;
flat in float vTime;
out vec4 oColor;

// 中心颜色（接近白色）
const vec4 center_color = vec4(1.0, 1.0, 0.98, 1.0);
// 射线颜色（金色/黄色）
const vec4 ray_color = vec4(1.0, 0.88, 0.45, 0.92);
// 放射线条数量
const float ray_count = 12.0;
// 线条锐度（数值越大线条越细、轮廓越锐利）
const float ray_sharpness = 2.0;
// 径向衰减范围：从中心到该距离线条慢慢淡出，1=一直延伸到边缘
const float radius_fade = 1.1;
// 中心辉光半径（中心亮斑范围大小）
const float center_glow_radius = 0.5;
// 顺时针旋转速度（弧度/秒）
const float rotation_speed = 0.1;
// 闪烁速度（越大闪得越快）
const float flicker_speed = 2.0;
// 闪烁强度（0=不闪烁，1=线条明暗变化幅度最大）
const float flicker_strength = 1.0;

void main() {

	float dist = length(vUV);
	float angle = atan(vUV.y, vUV.x);

	// 顺时针旋转：角度减去时间*转速，实现顺时针转动
	angle += vTime * rotation_speed;

	// 径向衰减：中心亮，向外逐渐淡出
	float radial = 1.0 - smoothstep(0.0, radius_fade, dist);
	radial = radial * radial;

	// 角度分片，生成放射线条
	float slice_angle = 6.28318530718 / ray_count; // 2PI / 射线数量
	float in_slice = mod(angle + 3.14159265359, slice_angle);
	float dist_from_ray_center = abs(in_slice - slice_angle * 0.5);
	float ray = 1.0 - smoothstep(0.0, slice_angle * 0.35, dist_from_ray_center);
	ray = pow(ray, ray_sharpness);

	// 单独线条闪烁：每一根射线用不同相位正弦，错开闪烁节奏
	float ray_index = floor((angle + 3.14159265359) / slice_angle);
	float flicker = 0.5 + 0.5 * sin(vTime * flicker_speed + ray_index * 1.618);
	flicker = mix(1.0, flicker, flicker_strength);
	ray *= flicker;

	// 中心辉光（物品中心的漫反射亮光）
	float glow = 1.0 - smoothstep(0.0, center_glow_radius, dist);

	// 最终透明度：射线 + 中心辉光，乘径向衰减
	float alpha = (ray * 0.75 + glow * 0.5) * radial;
	alpha = clamp(alpha, 0.0, 1.0);

	// 颜色混合：辉光越强越偏向中心白色，否则偏向射线金色
	vec3 col = mix(ray_color.rgb, center_color.rgb, glow);
	oColor = vec4(col * alpha, alpha * ray_color.a);
})"sv });

		p = LinkGLProgram(v, f);

		uCxy = glGetUniformLocation(p, "uCxy");

		aVert = glGetAttribLocation(p, "aVert");
		aPosTimeRadius = glGetAttribLocation(p, "aPosTimeRadius");
		CheckGLError();

		glGenVertexArrays(1, &va.id);
		glGenBuffers(1, &ib.id);
		glGenBuffers(1, &vb.id);

		glBindVertexArray(va);

		static const XY verts[4] = { { -1, -1 },{ -1, 1 },{ 1, -1 },{ 1, 1 } };
		glBindBuffer(GL_ARRAY_BUFFER, ib);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
		glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
		glEnableVertexAttribArray(aVert);

		glBindBuffer(GL_ARRAY_BUFFER, vb);

		glVertexAttribPointer(aPosTimeRadius, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_RadiateGoldenRaysData), (GLvoid*)offsetof(Shader_RadiateGoldenRaysData, pos));
		glVertexAttribDivisor(aPosTimeRadius, 1);
		glEnableVertexAttribArray(aPosTimeRadius);

		glBindVertexArray(0);
		CheckGLError();
	}

	void Shader_RadiateGoldenRays::Begin() {
		assert(!GameBase::instance->shader);
		assert(count == 0);
		glUseProgram(p);
		glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
		glBindVertexArray(va);
	}

	void Shader_RadiateGoldenRays::End() {
		assert(GameBase::instance->shader == this);
		if (count) {
			Commit();
		}
	}

	void Shader_RadiateGoldenRays::Commit() {
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_RadiateGoldenRaysData) * count, data.get(), GL_STREAM_DRAW);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
		CheckGLError();

		GameBase::instance->drawVerts += count * 6;
		GameBase::instance->drawCall += 1;

		count = 0;
	}

	Shader_RadiateGoldenRaysData* Shader_RadiateGoldenRays::Alloc(int32_t num_) {
		assert(GameBase::instance->shader == this);
		assert(num_ <= maxNums);
		if (count + num_ > maxNums) {
			Commit();
		}
		auto r = &data[count];
		count += num_;
		return r;
	}

	void Shader_RadiateGoldenRays::Draw(XY pos_, float time_, float radius_) {
		auto data = Alloc(1);
		data->pos = pos_;
		data->time = time_;
		data->radius = radius_;
	}

}
