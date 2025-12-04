#pragma once
#include "game.h"

struct Scene_MainMenu : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	i18n::Languages bak;

	void Init();
	void MakeUI();
	void Update() override;
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
