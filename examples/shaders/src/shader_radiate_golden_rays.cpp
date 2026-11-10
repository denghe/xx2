#include "pch.h"
#include "shader_radiate_golden_rays.h"
#include "xx_gamebase.h"

namespace xx {

	void Shader_RadiateGoldenRays::Init() {

		v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	// screen center coordinate
in vec2 aVert;
in vec4 aPosTimeRadius;
in vec4 aCenterColor;
in vec4 aRayColor;
in vec4 aRcRsRfCgr;	// aRayCount RaySharpness RadiusFade CenterGlowRadius
in vec3 aRSFsFs;	// aRotationSpeed FlickerSpeed FlickerStrength
flat out vec4 vCenterColor;
flat out vec4 vRayColor;
flat out vec4 vRcRsRfCgr;
flat out vec4 vRSFsFs;
out vec2 vUV;

void main() {
	vCenterColor = aCenterColor;
	vRayColor = aRayColor;
	vRcRsRfCgr = aRcRsRfCgr;
	vRSFsFs = vec4(aRSFsFs, aPosTimeRadius.z);
	vec2 pos = aPosTimeRadius.xy + aVert * aPosTimeRadius.w;
    gl_Position = vec4(pos * uCxy, 0, 1);
	vUV = aVert;
})"sv });


		f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision lowp float;
in vec2 vUV;
flat in vec4 vCenterColor;
flat in vec4 vRayColor;
flat in vec4 vRcRsRfCgr;
flat in vec4 vRSFsFs;
out vec4 oColor;

void main() {
	float ray_count = vRcRsRfCgr.x;
	float ray_sharpness = vRcRsRfCgr.y;
	float radius_fade = vRcRsRfCgr.z;
	float center_glow_radius = vRcRsRfCgr.w;
	float rotation_speed = vRSFsFs.x;
	float flicker_speed = vRSFsFs.y;
	float flicker_strength = vRSFsFs.z;
	float time = vRSFsFs.w;

	float dist = length(vUV);
	float angle = atan(vUV.y, vUV.x);

	// Clockwise rotation (subtracting time from angle produces clockwise motion)
	angle += time * rotation_speed;

	// Radial fade: bright at the center, fades out gradually toward the edges
	float radial = 1.0 - smoothstep(0.0, radius_fade, dist);
	radial = radial * radial;

	// Angular ray pattern: bright at the center of each ray, darker toward the edges
	float slice_angle = 6.28318530718 / ray_count; // 2PI / 射线数量
	float in_slice = mod(angle + 3.14159265359, slice_angle);
	float dist_from_ray_center = abs(in_slice - slice_angle * 0.5);
	float ray = 1.0 - smoothstep(0.0, slice_angle * 0.35, dist_from_ray_center);
	ray = pow(ray, ray_sharpness);

	// Per-ray flicker: brightness varies with a sine wave based on ray index
	float ray_index = floor((angle + 3.14159265359) / slice_angle);
	float flicker = 0.5 + 0.5 * sin(time * flicker_speed + ray_index * 1.618);
	flicker = mix(1.0, flicker, flicker_strength);
	ray *= flicker;

	// Center glow (light bloom behind the element)
	float glow = 1.0 - smoothstep(0.0, center_glow_radius, dist);

	// Final alpha: combines rays + center glow with radial fade applied
	float alpha = (ray * 0.75 + glow * 0.5) * radial;
	alpha = clamp(alpha, 0.0, 1.0);

	// Color mix: premutiple alpha
	vec3 col = mix(vRayColor.rgb, vCenterColor.rgb, glow);
	oColor = vec4(col * alpha, vRayColor.a * alpha);
})"sv });

		p = LinkGLProgram(v, f);

		uCxy = glGetUniformLocation(p, "uCxy");

		aVert = glGetAttribLocation(p, "aVert");
		aPosTimeRadius = glGetAttribLocation(p, "aPosTimeRadius");
		aCenterColor = glGetAttribLocation(p, "aCenterColor");
		aRayColor = glGetAttribLocation(p, "aRayColor");
		aRayCountRaySharpnessRadiusFadeCenterGlowRadius = glGetAttribLocation(p, "aRcRsRfCgr");
		aRotationSpeedFlickerSpeedFlickerStrength = glGetAttribLocation(p, "aRSFsFs");

		CheckGLError();

		glGenVertexArrays(1, &va.id);
		glGenBuffers(1, &ib.id);
		glGenBuffers(1, &vb.id);

		glBindVertexArray(va);

		glBindBuffer(GL_ARRAY_BUFFER, ib);
#if 0
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
#else
		static const XY verts[4] = { { -1, -1 },{ -1, 1 },{ 1, -1 },{ 1, 1 } };
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
#endif
		glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
		glEnableVertexAttribArray(aVert);

		glBindBuffer(GL_ARRAY_BUFFER, vb);

		glVertexAttribPointer(aPosTimeRadius, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_RadiateGoldenRaysData), (GLvoid*)offsetof(Shader_RadiateGoldenRaysData, pos));
		glVertexAttribDivisor(aPosTimeRadius, 1);
		glEnableVertexAttribArray(aPosTimeRadius);

		glVertexAttribPointer(aCenterColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_RadiateGoldenRaysData), (GLvoid*)offsetof(Shader_RadiateGoldenRaysData, centerColor));
		glVertexAttribDivisor(aCenterColor, 1);
		glEnableVertexAttribArray(aCenterColor);

		glVertexAttribPointer(aRayColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_RadiateGoldenRaysData), (GLvoid*)offsetof(Shader_RadiateGoldenRaysData, rayColor));
		glVertexAttribDivisor(aRayColor, 1);
		glEnableVertexAttribArray(aRayColor);

		glVertexAttribPointer(aRayCountRaySharpnessRadiusFadeCenterGlowRadius, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_RadiateGoldenRaysData), (GLvoid*)offsetof(Shader_RadiateGoldenRaysData, rayCount));
		glVertexAttribDivisor(aRayCountRaySharpnessRadiusFadeCenterGlowRadius, 1);
		glEnableVertexAttribArray(aRayCountRaySharpnessRadiusFadeCenterGlowRadius);

		glVertexAttribPointer(aRotationSpeedFlickerSpeedFlickerStrength, 3, GL_FLOAT, GL_FALSE, sizeof(Shader_RadiateGoldenRaysData), (GLvoid*)offsetof(Shader_RadiateGoldenRaysData, rotationSpeed));
		glVertexAttribDivisor(aRotationSpeedFlickerSpeedFlickerStrength, 1);
		glEnableVertexAttribArray(aRotationSpeedFlickerSpeedFlickerStrength);

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

		//glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, numCircleMeshSegaments + 2, count);
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

	void Shader_RadiateGoldenRays::Draw(XY pos_, float time_, float radius_
		, RGBA8 centerColor_, RGBA8 rayColor_
		, float rayCount_, float raySharpness_, float radiusFade_, float centerGlowRadius_
		, float rotationSpeed_, float flickerSpeed_, float flickerStrength_
	) {
		auto d = Alloc(1);
		d->pos = pos_;
		d->time = time_;
		d->radius = radius_;
		d->centerColor = centerColor_;
		d->rayColor = rayColor_;
		d->rayCount = rayCount_;
		d->raySharpness = raySharpness_;
		d->radiusFade = radiusFade_;
		d->centerGlowRadius = centerGlowRadius_;
		d->rotationSpeed = rotationSpeed_;
		d->flickerSpeed = flickerSpeed_;
		d->flickerStrength = flickerStrength_;
	}

}
