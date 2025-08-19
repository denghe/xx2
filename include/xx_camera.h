#pragma once
#include "xx_shaker.h"

namespace xx {

	struct Camera {
		float logicScale{ 1 }, baseScale{ 1 };
		float scale{ 1 }, _1_scale{ 1 };
		XY original{};	// logic center pos
		Shaker shaker;

		XX_INLINE Camera& SetOriginal(XY original_) {
			original = original_;
			return *this;
		}

		XX_INLINE Camera& SetBaseScale(float baseScale_) {
			baseScale = baseScale_;
			scale = logicScale * baseScale;
			_1_scale = 1.f / scale;
			return *this;
		}

		XX_INLINE Camera& SetLogicScale(float logicScale_) {
			logicScale = logicScale_;
			scale = logicScale * baseScale;
			_1_scale = 1.f / scale;
			return *this;
		}

		XX_INLINE Camera& SetScale(float baseScale_, float logicScale_) {
			baseScale = baseScale_;
			logicScale = logicScale_;
			scale = logicScale * baseScale;
			_1_scale = 1.f / scale;
			return *this;
		}

		XX_INLINE XY ToGLPos_Logic(XY logicPos) const {
			return (logicPos - original - shaker.offset).FlipY() * logicScale;
		}

		XX_INLINE XY ToGLPos(XY logicPos) const {
			return (logicPos - original - shaker.offset).FlipY() * scale;
		}

		XX_INLINE XY ToLogicPos(XY glPos) const {
			return { glPos.x * _1_scale + original.x + shaker.offset.x, -glPos.y * _1_scale + original.y + shaker.offset.y };
		}

		XX_INLINE void Update(xx::Rnd& rnd, int32_t time) {
			shaker.Update(rnd, time);
		}
	};
}
