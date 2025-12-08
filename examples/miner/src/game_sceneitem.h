#pragma once
#include "game.h"

struct Scene;
struct SceneItem {
	Scene* scene{};
	SceneItem* next{};	// for draw order sort by y
	int32_t indexAtList{-1};
	float y{};
	virtual bool Update() { return false; }	// return true mean release
	virtual void Draw() {}
	~SceneItem() {}
};
