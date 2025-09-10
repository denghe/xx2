#include "pch.h"
#include "scene_play.h"
#include "player.h"

void Player::Init(Scene_Play* scene_, XY pos_, float radius_) {
	scene = scene_;
	tf = gg.embed.icon_heart;
	pos = pos_;
	radius = radius_;
}

void Player::Update() {
}

void Player::Draw() {
	auto& cam = scene->cam;
	auto scale = /*_1scale * */cam.scale * (radius / tf.textureRect.w);
	gg.Quad().Draw(*tf.tex, tf.textureRect, cam.ToGLPos(pos), 0.5f, scale, 0, 1, xx::RGBA8_Red);
}
