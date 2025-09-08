#include "pch.h"
#include "scene_play.h"
#include "player.h"

void Player::Init(Scene_Play* scene_, XY pos_, float radius_) {
	scene = scene_;
	tex = gg.res.heart;
	pos = pos_;
	radius = radius_;
}

void Player::Update() {
}

void Player::Draw() {
	auto& cam = scene->cam;
	auto scale = /*_1scale * */cam.scale * (radius / tex->size.x);
	gg.Quad().Draw(*tex, tex->Rect(), cam.ToGLPos(pos), 0.5f, scale, 0, 1, xx::RGBA8_Red);
}
