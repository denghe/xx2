#pragma once
#include "game.h"

struct Scene_Test1 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };
	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
