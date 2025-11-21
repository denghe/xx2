#pragma once
#include "game.h"

struct Scene;
struct Map {
	Scene* scene{};
	virtual void Init(Scene* scene_);
	virtual void Update();
	virtual void Draw();
};
