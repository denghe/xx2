#pragma once
#include "game.h"

struct Scene_Test2 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	xx::Frame tp1p64, tp2p64, tp1a64;

	float time{}, timePool{}, timeScale{ 1 };

	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
