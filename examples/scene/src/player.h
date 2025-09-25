#pragma once
#include "game.h"
#include "creature.h"

struct Scene_Play;
struct Player : Creature {
	int LoadData();	// return 0: success   -1: not found
	void SaveData();
	void Init(Scene_Play* scene_, XY pos_, float radius_);
	void Update() override;
	void Draw() override;
};
