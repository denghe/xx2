#include "pch.h"
#include "scene_play.h"
#include "equip.h"

void Equip::Init(Scene_Play* scene_, xx::Ref<xx::GLTexture> tex_, XY pos_, float radius_) {
	scene = scene_;
	tex = std::move(tex_);
	pos = pos_;
	radius = radius_;
	_1scale = cAnimScaleRange.from;
}

void Equip::Update() {
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

void Equip::Draw() {
	auto& cam = scene->cam;
	auto scale = _1scale * cam.scale * (radius / tex->size.x);
	gg.Quad().Draw(*tex, tex->Rect(), cam.ToGLPos(pos), 0.5f, scale);
}
