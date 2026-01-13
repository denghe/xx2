#pragma once
#include "xx_gl.h"
#include "xx_prims.h"

namespace xx {

	struct Scissor {
		XY bakWndSiz{};
		std::array<uint32_t, 3> bakBlend;

		template<typename Func>
		void DirectDrawTo(XY pos, XY wh, Func&& func) {
			DirectBegin(pos.x, pos.y, wh.x, wh.y);
			func();
			DirectEnd();
		}

		template<typename Func>
		void OffsetDrawTo(XY pos, XY wh, Func&& func) {
			OffsetBegin(pos.x, pos.y, wh.x, wh.y);
			func();
			OffsetEnd();
		}

	protected:
		void DirectBegin(float x, float y, float w, float h);
		void DirectEnd();
		void OffsetBegin(float x, float y, float w, float h);
		void OffsetEnd();
	};

}
