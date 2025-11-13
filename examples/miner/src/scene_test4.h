#pragma once
#include "game.h"

enum class AnimTypes {
	Idle,Move,Atk
};

struct Scene_Test4;
struct Rock {
	Scene_Test4* scene{};
	XY pos{};
	float radius{};
	int32_t hp{};
	Rock& Init(Scene_Test4* scene_, XY pos_, float radius_);
	void Update();
	void Draw();
};

struct Monster1 {
	Scene_Test4* scene{};
	xx::TinyFrame* tfs{};
	XY* aps{};
	xx::FromTo<XY>* cds{};
	int32_t tfsLen{};
	float tfIndex{};
	float speedScale{};
	float radius{};
	XY pos{};
	Monster1& Monster1Init(Scene_Test4* scene_, XY pos_, float radius_, float speedScale_);	// need set anim
	void SetAnim(AnimTypes t);
	bool StepAnimOnce();
	void StepAnimLoop();
	bool IsHitFrame() const;
	virtual void Update();
	virtual void Draw();
};

struct Monster2 : Monster1 {
	xx::Weak<Rock> target;
	float stepTime{};
	float attackRange{}, moveSpeed{};
	int32_t _1{};
	Monster2& Monster2Init(Scene_Test4* scene_, XY pos_, float radius_);
	bool SearchTarget();
	void Update() override;
	void Draw() override;
};

struct Scene_Test4 : xx::SceneBase {
	static constexpr float cUIScale{ 0.5f };
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };

	xx::List<xx::Shared<Monster1>> monsters;
	xx::List<xx::Shared<Rock>> rocks;

	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
