#pragma once
#include "game.h"

struct Scene;
struct SceneItem {
	Scene* scene{};
	float y{};
	virtual void Update() {}
	virtual void Draw() {}
	~SceneItem() {}
};
