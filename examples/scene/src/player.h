#pragma once
#include "game.h"
#include "player_properties.h"

struct Scene_Play;
struct Player : PlayerProperties {
	Scene_Play* scene{};
	xx::Ref<xx::GLTexture> tex;
	XY pos{};
	float radius{};
	void Init(Scene_Play* scene_, XY pos_, float radius_);
	void Update();
	void Draw();
};
