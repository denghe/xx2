#include "pch.h"
#include "scene_1.h"
#include "monster.h"

void Monster::Init(Scene_1* scene_, xx::Ref<xx::GLTexture> tex_, XY pos_, float radius_) {
	scene = scene_;
	tex = std::move(tex_);
	pos = pos_;
	radius = radius_;
	_1scale = cAnimScaleRange.from;
}

void Monster::Update() {
	XX_BEGIN(_1);
	while (true) {
		for (; _1scale < cAnimScaleRange.to; _1scale += cAnimStepDelay) {
			XX_YIELD(_1);
		}
		for (; _1scale > cAnimScaleRange.from; _1scale -= cAnimStepDelay) {
			XX_YIELD(_1);
		}
	}
	XX_END(_1);
}

void Monster::Draw() {
	auto& cam = scene->cam;
	auto scale = _1scale * cam.scale * (radius / tex->Width());
	gg.Quad().Draw(*tex, tex->Rect(), cam.ToGLPos(pos), 0.5f, scale);
}
