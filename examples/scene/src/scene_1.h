#pragma once
#include "game.h"

struct Scene_1 : Scene {
	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize() override;
};
