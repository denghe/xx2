#pragma once
#include "game.h"

struct Scene_Settings : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Weak<xx::DropDownList> ddlWindowModes, ddlResolutions;
	void Init();
	void Update() override;
	void Draw() override;
	void OnResize() override;
};
