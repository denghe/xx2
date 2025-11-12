#pragma once
#include "game.h"

enum class AnimTypes {
	Idle,Move,Atk
};

struct Scene_Test4;
struct Monster1 {
	Scene_Test4* scene{};
	xx::TinyFrame* tfs{};
	XY pos{};
	int32_t tfsLen{};
	float tfIndex{};
	float speedScale{};
	Monster1& Init(Scene_Test4* scene_, XY pos_ = 0, float speedScale_ = 1);	// need set anim
	void SetAnim(AnimTypes t);
	bool Update();
	void Draw();
};

struct Scene_Test4 : xx::SceneBase {
	static constexpr float cUIScale{ 0.5f };
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };

	xx::List<xx::Shared<Monster1>> monsters;

	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
