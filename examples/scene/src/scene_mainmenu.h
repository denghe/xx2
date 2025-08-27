#pragma once
#include "game.h"

struct Scene_MainMenu : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	void Init();
	void Update() override;
	void Draw() override;
	void OnResize() override;
};
