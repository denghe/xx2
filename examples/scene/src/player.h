#pragma once
#include "game.h"
#include "creature.h"

struct Scene_Play;
struct Player : Creature {
	void Init(Scene_Play* scene_, XY pos_, float radius_);
	void Update() override;
	void Draw() override;
};
