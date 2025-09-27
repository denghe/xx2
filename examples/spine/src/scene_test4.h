#pragma once
#include "game.h"

struct SpineSpineBoy {
	xx::Shared<xx::SpinePlayer> sp;
	spine::Bone* crosshair{};
};

struct Scene_Test4 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	xx::List<SpineSpineBoy> sbs;

	void Init();
	void Update() override;
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
