#pragma once
#include "game.h"

struct SpineOwl {
	xx::Shared<xx::SpinePlayer> sp;
	spine::TrackEntry* left{};
	spine::TrackEntry* right{};
	spine::TrackEntry* up{};
	spine::TrackEntry* down{};
};

struct Scene_Test2 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };
	xx::List<SpineOwl> owls;

	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
