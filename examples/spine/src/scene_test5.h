#pragma once
#include "game.h"

struct Scene_Test5;
struct Man1 {
	Scene_Test5* scene{};
	xx::Shared<xx::SpinePlayer> sp;
	spine::Bone* neck{};
	spine::Bone* body{};
	XY neckOriginalPos{};
	XY neckOffset{};
	static constexpr float neckFollowSpeed{ 1500.f };
	void Init(Scene_Test5* scene_);
	void Update();
	void Draw();
};

struct Scene_Test5 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	Man1 man1;

	void Init();
	void Update() override;
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
