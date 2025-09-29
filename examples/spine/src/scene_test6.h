#pragma once
#include "game.h"

struct Scene_Test6;
struct Eye {
	Scene_Test6* scene{};
	xx::Shared<xx::SpinePlayer> sp;
	spine::Bone* control{};
	XY controlOriginal{};
	XY eyeOriginalOffset{};
	static constexpr float neckFollowSpeed{ 1500.f };
	void Init(Scene_Test6* scene_);
	void Update();
	void Draw();
};

struct Scene_Test6 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	Eye eye;

	void Init();
	void Update() override;
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
