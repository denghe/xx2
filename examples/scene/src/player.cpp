#include "pch.h"
#include "scene_play.h"
#include "player.h"

int Player::LoadData() {
	if (auto sv = gg.db.Load("Player"); !sv.data()) {
		if (gg.db.NotFound()) return -1;
		return __LINE__;
	}
	else {
		xx::Data_r dr(sv);
		for (int i = 0; i < equips.size(); ++i) {
			if (auto r = Equip::ReadFrom(dr, this, equips[i]); r) return r;
		}
		for (int i = 0; i < bag.len; ++i) {
			if (auto r = Equip::ReadFrom(dr, this, bag[i]); r) return r;
		}
		if(dr.len != dr.offset) return __LINE__;
		return 0;
	}
}

void Player::SaveData() {
	xx::Data d;
	for (int i = 0; i < equips.size(); ++i) {
		if (auto& e = equips[i]; e) e->WriteTo(d);
		else d.Write((uint32_t)0);
	}
	for (int i = 0; i < bag.len; ++i) {
		if (auto& e = bag[i]; e) e->WriteTo(d);
		else d.Write((uint32_t)0);
	}
	auto r = gg.db.Save("Player", d);
	assert(!r);
}

void Player::Init(Scene_Play* scene_, XY pos_, float radius_) {
	InitCreature(scene_, pos_, radius_);
	bag.Resize(6 * 10);

	if (LoadData()) {
		// fill some equip & bag item for test
		int32_t i{};
		for (int j = 1; j < Equip::makers.len; ++j) {
			bag[i++] = Equip::Make(this, j);
		}
		SaveData();
	}
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
	auto s = scale * (c.scale * (radius / f.uvRect.w));
	auto p = pos + shaker.offset;
	gg.Quad().Draw(*f.tex, f.uvRect, c.ToGLPos(p), anchor, s, radians);
}

// todo: auto attack monster logic
