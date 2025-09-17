#pragma once
#include "game.h"
#include "creature_properties.h"
#include "equip.h"

struct Scene_Play;
struct alignas(8) Creature {
	Scene_Play* scene{};
	XY pos{}, scale{}, anchor{};	// scale only for anim
	float radians{};
	float radius{};					// logic size
	xx::Shaker shaker;
	CreatureProperties props;
	std::array<xx::Shared<Equip>, (int32_t)EquipLocations::__EQUIPED_MAX__ + 1> equips;
	xx::List<xx::Shared<Equip>> bag;

	int32_t _1{};
	void Idle_1();

	int32_t _2{};
	void Idle_2();

	void InitCreature(Scene_Play* scene_, XY pos_, float radius_);
	virtual void Update() {}
	virtual void Draw() {}
	// ... more interface
};
