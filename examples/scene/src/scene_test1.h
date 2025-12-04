#pragma once
#include "game.h"

struct Scene_Test1;
struct Bug {
	static constexpr int32_t cLen{ 10 };
	static constexpr float cStepMul{ 0.8f };
	static constexpr float cStep0{ 16.f };
	static constexpr float cScale0{ 0.3f };
	static constexpr float cSpeed{ 50.f / gg.cFps };
	inline static std::array<float, cLen> scales;
	inline static std::array<float, cLen> steps;
	inline static void StaticInit();
	Scene_Test1* scene{};
	std::array<XY, cLen> poss;
	std::array<float, cLen> radianss;
	xx::RGBA8 color{};
	void Init(Scene_Test1* scene_, XY pos_, XY tar_, xx::RGBA8 color_);
	void Update(XY tar_);
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
