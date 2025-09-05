#pragma once
#include "game.h"
#include "monster.h"

struct Scene_1 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };
	xx::List<xx::Shared<Monster>> monsters;
	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize() override;
};
