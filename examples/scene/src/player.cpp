#include "pch.h"
#include "scene_play.h"
#include "player.h"

void Player::Init(Scene_Play* scene_, XY pos_, float radius_) {
	InitCreature(scene_, pos_, radius_);
	// todo: load player data from sav?
	bag.Resize(6 * 10);

	// todo: fill some equip & bag item for test
	int32_t i{};
	bag[i++] = Equip::Make<Equip_amulet_1>(this);
	bag[i++] = Equip::Make<Equip_armor_1>(this);
	bag[i++] = Equip::Make<Equip_armor_2>(this);
	bag[i++] = Equip::Make<Equip_armor_3>(this);
	bag[i++] = Equip::Make<Equip_boots_1>(this);
	bag[i++] = Equip::Make<Equip_consumable_1>(this);
	bag[i++] = Equip::Make<Equip_currency_1>(this);
	bag[i++] = Equip::Make<Equip_currency_2>(this);
	bag[i++] = Equip::Make<Equip_helm_1>(this);
	bag[i++] = Equip::Make<Equip_material_1>(this);
	bag[i++] = Equip::Make<Equip_ring_1>(this);
	bag[i++] = Equip::Make<Equip_ring_2>(this);
	bag[i++] = Equip::Make<Equip_ring_3>(this);
	bag[i++] = Equip::Make<Equip_ring_4>(this);
	bag[i++] = Equip::Make<Equip_weapon1_1>(this);
	bag[i++] = Equip::Make<Equip_weapon1_2>(this);
	bag[i++] = Equip::Make<Equip_weapon1_3>(this);
	bag[i++] = Equip::Make<Equip_weapon2_1>(this);
	// ...
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
