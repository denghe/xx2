#include "pch.h"
#include "scene_play.h"
#include "player.h"

void Player::Init(Scene_Play* scene_, XY pos_, float radius_) {
	InitCreature(scene_, pos_, radius_);
}

void Player::Update() {
	if (gg.keyboard[GLFW_KEY_SPACE]) {
		shaker.Shake(radius * 0.1f, 800.f * gg.cDelta, scene->time + 0.5f);
	}

	shaker.Update(gg.rnd, scene->time);
	Idle_2();
}

void Player::Draw() {
	auto& c = scene->cam;
	auto& f = gg.res.player;
	auto s = scale * (c.scale * (radius / f.textureRect.w));
	auto p = pos + shaker.offset;
	gg.Quad().Draw(*f.tex, f.textureRect, c.ToGLPos(p), anchor, s, radians);
}

// todo: auto attack monster logic
