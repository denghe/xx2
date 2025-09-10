#include "pch.h"
#include "scene_play.h"
#include "monster.h"

void Monster::Init(Scene_Play* scene_, XY pos_, float radius_) {
	scene = scene_;
	tf = gg.embed.icon_heart;
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
	auto scale = _1scale * cam.scale * (radius / tf.textureRect.w);
	gg.Quad().Draw(*tf.tex, tf.textureRect, cam.ToGLPos(pos), 0.5f, scale);
}
