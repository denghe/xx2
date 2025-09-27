#pragma once
#include "game.h"

struct Scene_Test3 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;

	xx::Shared<xx::SpinePlayer> sp;
	spine::Bone* crosshair{};

	void Init();
	void Update() override;
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
