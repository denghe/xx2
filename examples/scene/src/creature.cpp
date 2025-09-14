#include "pch.h"
#include "scene_play.h"
#include "creature.h"

void Creature::InitCreature(Scene_Play* scene_, XY pos_, float radius_) {
	scene = scene_;
	pos = pos_;
	scale = 1;
	anchor = { 0.5, 0 };
	radians = 0;
	radius = radius_;
}

void Creature::Idle_1() {
	static constexpr xx::FromTo<float> cScaleRange{ .5f, 1.f };
	static constexpr float cDuration{ 0.5f };
	static constexpr float cStepDelay{ cScaleRange.Sub() / (Game::cFps * cDuration) };
	XX_BEGIN(_1);
	scale = cScaleRange.to;
	while (true) {
		for (; scale.x > cScaleRange.from; scale -= cStepDelay) {
			XX_YIELD(_1);
		}
		for (; scale.x < cScaleRange.to; scale += cStepDelay) {
			XX_YIELD(_1);
		}
	}
	XX_END(_1);
}

void Creature::Idle_2() {
	static constexpr xx::FromTo<float> cScaleRange{ 1.f, 1.1f };
	static constexpr float cDuration{ 0.5f };
	static constexpr float cStepDelay{ cScaleRange.Sub() / (Game::cFps * cDuration) };
	XX_BEGIN(_2);
	scale = cScaleRange.from;
	while (true) {
		for (; scale.x < cScaleRange.to; scale.x += cStepDelay) {
			scale.y = 2.f - scale.x;
			XX_YIELD(_2);
		}
		for (; scale.x > cScaleRange.from; scale.x -= cStepDelay) {
			scale.y = 2.f - scale.x;
			XX_YIELD(_2);
		}
	}
	XX_END(_2);
}
