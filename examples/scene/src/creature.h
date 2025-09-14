#pragma once
#include "game.h"
#include "creature_properties.h"

struct Scene_Play;
struct Creature : CreatureProperties {
	Scene_Play* scene{};
	XY pos{}, scale{}, anchor{};	// scale only for anim
	float radians{};
	float radius{};					// logic size
	xx::Shaker shaker;

	int32_t _1{};
	void Idle_1();

	int32_t _2{};
	void Idle_2();

	void InitCreature(Scene_Play* scene_, XY pos_, float radius_);
	virtual void Update() {}
	virtual void Draw() {}
	// ... more interface
};
