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

		static const int32_t maxNums = maxVertNums * 1.5;
		std::unique_ptr<Shader_LineData[]> points = std::make_unique_for_overwrite<Shader_LineData[]>(maxVertNums);
		size_t pointsCount = 0;
		std::unique_ptr<uint16_t[]> indexs = std::make_unique_for_overwrite<uint16_t[]>(maxIndexNums);
		size_t indexsCount = 0;

		void Init();
		void Begin() override;
		void End() override;

		void Commit();
		Shader_LineData* Alloc(int32_t num_);
		void Fill(Shader_LineData const* buf_, int32_t num_);
	};

}
