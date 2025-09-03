#pragma once
#include "game.h"

struct Scene_Settings : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Weak<xx::DropDownList> uiWindowModes, uiResolutions;
	xx::Weak<xx::Slider> uiAudioVolume, uiMusicVolume;
	void Init();
	void Update() override;
	void Draw() override;
	void OnResize() override;
};
