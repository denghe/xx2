#pragma once
#include "xx_math.h"

namespace xx {

	struct Shaker {
		float radius{}, speed{};	// speed: increase val per frame
		float endtime{};
		int32_t n{};
		XY offset{}, tarOffset{};

		XX_INLINE void Shake(float radius_, float speed_, float endTime_) {
			radius = radius_;
			speed = speed_;
			endtime = endTime_;
		}

		XX_INLINE void Update(Rnd& rnd_, float time_) {
			XX_BEGIN(n);
		LabWait:
			while (time_ >= endtime) { XX_YIELD(n); }
		LabSetTarget:
			tarOffset = GetRndPosDoughnut(rnd_, radius, 0);
		LabMove:
			if (time_ >= endtime) {
				offset = {};
				goto LabWait;
			}
			XX_YIELD(n);
			auto d = tarOffset - offset;
			auto dd = d.x * d.x + d.y * d.y;
			if (dd <= speed * speed) {
				offset = tarOffset;
				goto LabSetTarget;
			}
			else {
				offset += d / std::sqrtf(dd) * speed;
				goto LabMove;
			}
			XX_END(n);
		}
	};

}
