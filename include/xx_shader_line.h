#pragma once
#include "xx_shader.h"

namespace xx {
	// 1 point data ( for draw line strip )
	struct Shader_LineData : XY, RGBA8 {};

	// for draw line strip
	struct Shader_Line : Shader {
		static const size_t index = 2;	// index at sm->shaders

		GLint aPos = -1, aColor = -1;
		GLVertexArrays va;
		GLBuffer vb, ib;

		std::unique_ptr<Shader_LineData[]> points = std::make_unique_for_overwrite<Shader_LineData[]>(maxIndexNums);
		size_t pointsCount = 0;
		std::unique_ptr<uint16_t[]> indexs = std::make_unique_for_overwrite<uint16_t[]>(maxIndexNums);
		size_t indexsCount = 0;

		void Init();
		void Begin() override;
		void End() override;

		void Commit();
		Shader_LineData* Alloc(int32_t num_);
		void Fill(Shader_LineData const* buf_, int32_t num_);
		void DrawCircle(XY centerPos_, float radius_, std::optional<float> radians_ = {}, int32_t segments_ = 20, xx::RGBA8 color_ = xx::RGBA8_White);
	};

}
