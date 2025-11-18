#pragma once
#include "game.h"

struct Scene;
struct SceneItem {
	Scene* scene{};
	float y{};
	virtual bool Update() { return false; }	// return true mean release
	virtual void Draw() {}
	~SceneItem() {}
};
