#pragma once
#include "game.h"

struct Scene_Test3;
struct Airplane {
	Scene_Test3* scene{};
	XY const* track{};
	int32_t trackLen{};
	int32_t trackPointIndex{};
	float radians{};
	xx::TinyFrame tf;
	void Init(Scene_Test3* scene_);
	bool Update();
	void Draw();
};

struct Scene_Test3 : xx::SceneBase {
	static constexpr float cUIScale{ 0.5f };
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };

	static constexpr xx::FromTo<float> cCamScaleRange{ 0.3, 1 };
	xx::Shared<xx::Slider> uiCamScale;

	static constexpr xx::FromTo<int32_t> cGenAirplaneNumPerFrameRange{ 1, 1000 };
	int32_t cGenAirplaneNumPerFrame{ 1 };
	xx::Shared<xx::Slider> uiGenAirplaneNumPerFrame;

	xx::List<std::pair<float, xx::List<XY>>> tracks;
	xx::List<xx::Shared<Airplane>> airplanes;

	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
