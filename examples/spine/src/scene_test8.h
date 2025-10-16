#pragma once
#include "game.h"

struct Scene_Test8 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	xx::Ref<xx::GLTexture> texBG;

	void Init();
	void Update() override;
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
