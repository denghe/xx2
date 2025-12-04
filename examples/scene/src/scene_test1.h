#pragma once
#include "game.h"

struct Bug {
	static constexpr int32_t cLen{ 10 };
	static constexpr float cStepMul{ 0.8f };
	static constexpr float cStep0{ 32.f };
	inline static std::array<float, cLen> scales;
	inline static std::array<float, cLen> steps;
	inline static void StaticInit();
	std::array<XY, cLen> poss;
	std::array<float, cLen> radianss;
	void Init(XY pos_, float radians_);
	void Update();
	void Draw();
};

struct Scene_Test1 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;

	float time{}, timePool{}, timeScale{ 1 };

	xx::List<xx::Shared<Bug>> bugs;

	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
