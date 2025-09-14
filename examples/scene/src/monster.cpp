#include "pch.h"
#include "scene_play.h"
#include "monster.h"

void Monster::Init(Scene_Play* scene_, XY pos_, float radius_) {
	InitCreature(scene_, pos_, radius_);
}

void Monster::Update() {
	shaker.Update(gg.rnd, scene->time);
	Idle_2();
}

void Monster::Draw() {
	auto& c = scene->cam;
	auto& f = gg.res.monster;
	auto s = scale * (c.scale * (radius / f.textureRect.w));
	auto p = pos + shaker.offset;
	gg.Quad().Draw(*f.tex, f.textureRect, c.ToGLPos(p), anchor, s, radians);
}
