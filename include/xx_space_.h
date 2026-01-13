#pragma once
#include "xx_prims.h"

namespace xx {

	struct SpaceGridCountRadius {
		int32_t count;
		float radius;
	};

	struct SpaceGridRingDiffuseData {
		int32_t cellSize;
		List<SpaceGridCountRadius> lens;
		List<XYi> idxs;

		void Init(int32_t gridNumRows, int32_t cellSize_);
	};

}
