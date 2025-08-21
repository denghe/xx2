#pragma once
#include "xx_list.h"
#include "xx_prims.h"
#include "xx_math.h"

namespace xx {

	struct SpaceGridCountRadius {
		int32_t count;
		float radius;
	};

	struct SpaceGridRingDiffuseData {
		int32_t cellSize;
		List<SpaceGridCountRadius> lens;
		List<XYi> idxs;

		void Init(int32_t gridNumRows, int32_t cellSize_) {
			cellSize = cellSize_;
			lens.Emplace(0, 0.f);
			idxs.Emplace();
			std::unordered_set<uint64_t> set;
			set.insert(0);
			for (float radius = (float)cellSize_; radius < cellSize_ * gridNumRows; radius += cellSize_) {
				auto lenBak = idxs.len;
				auto radians = std::asin(0.5f / radius) * 2;
				auto step = (int32_t)(M_PI * 2 / radians);
				auto inc = M_PI * 2 / step;
				for (int32_t i = 0; i < step; ++i) {
					auto a = inc * i;
					auto cos = std::cos(a);
					auto sin = std::sin(a);
					auto ix = (int32_t)(cos * radius) / cellSize_;
					auto iy = (int32_t)(sin * radius) / cellSize_;
					auto key = ((uint64_t)iy << 32) + (uint64_t)ix;
					if (set.insert(key).second) {
						idxs.Emplace(ix, iy);
					}
				}
				if (idxs.len > lenBak) {
					lens.Emplace(idxs.len, radius);
				}
			}
		}
	};

}
