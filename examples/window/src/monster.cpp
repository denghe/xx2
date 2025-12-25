#include "pch.h"
#include "monster.h"

void Monster::ChangeColor() {
	colorIndex++;
	if (colorIndex >= cColors.size()) {
		colorIndex = 0;
	}
}

void Monster::ChangeAnim() {
	animIndex++;
	if (animIndex >= cAnimCount) {
		animIndex = 0;
	}
}

void Monster::AnimScale() {
	XX_BEGIN(_1);
	for (scale = 1.f; scale.x > 0.75f; scale -= gg.delta) {
		XX_YIELD(_1);
	}
	for (scale = 0.75f; scale.x < 1.f; scale += gg.delta) {
		XX_YIELD(_1);
	}
	XX_YIELD_TO_BEGIN(_1);
	XX_END(_1);
}

void Monster::AnimBounce() {
	XX_BEGIN(_2);
	for (_2x = 0.0834f; _2x < 0.916f; _2x += gg.delta) {
		{
			auto r = xx::CalcBounce(_2x);
			scale = { r, 2.f - r };
		}
		XX_YIELD(_2);
	}
	XX_YIELD_TO_BEGIN(_2);
	XX_END(_2);
}

void Monster::Init(xx::Frame tex_) {
	frame = std::move(tex_);
	scale = 1.f;
}

void Monster::Update() {
	switch (animIndex) {
	case 0:
		AnimBounce();
		break;
	case 1:
		AnimScale();
		break;
	}
}

void Monster::Draw() {
	gg.Quad().DrawFrame(frame, gg.cam.ToGLPos({}), 20.f * scale * gg.cam.scale, 0, 1, cColors[colorIndex]);
}
