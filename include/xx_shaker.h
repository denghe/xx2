#pragma once
#include "xx_prims.h"

namespace xx {

	struct Shaker {
		float radius{}, speed{};	// speed: increase val per frame
		float endtime{};
		int32_t n{};
		XY offset{}, tarOffset{};

		void Shake(float radius_, float speed_, float endTime_);
		void Update(Rnd& rnd_, float time_);
	};

}
