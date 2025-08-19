#pragma once
#include "xx_prims.h"
#include "xx_rnd.h"

namespace xx {

	struct Shaker {
		float radius{}, speed{};	// speed: increase val per frame
		int32_t endtime{};
		int32_t n{};
		XY offset{}, tarOffset{};

		// example: 5, 300.f * Cfg::frameDelay, int32_t(0.5f * Cfg::fps), time
		XX_INLINE void Shake(float radius_, float speed_, int32_t duration, int32_t time) {
			radius = radius_;
			speed = speed_;
			endtime = time + duration;
		}

		XX_INLINE static XY GetRndPosDoughnut(xx::Rnd& rnd, float maxRadius, float safeRadius) {
			auto len = maxRadius - safeRadius;
			auto len_radius = len / maxRadius;
			auto safeRadius_radius = safeRadius / maxRadius;
			auto radius = std::sqrtf(rnd.Next<float>(0, len_radius) + safeRadius_radius) * maxRadius;
			auto radians = rnd.Next<float>(-M_PI, M_PI);
			return { std::cosf(radians) * radius, std::sinf(radians) * radius };
		}

		XX_INLINE void Update(xx::Rnd& rnd, int32_t time) {
			XX_BEGIN(n);
		LabWait:
			while (time >= endtime) { XX_YIELD(n); }	// sleepEx
		LabSetTarget:
			tarOffset = GetRndPosDoughnut(rnd, radius, 0);
		LabMove:
			if (time >= endtime) {
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
