#pragma once
#include "game.h"

namespace Test1 {

	struct Scene;

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		//static constexpr xx::FromTo<float> cCamScaleRange{ 0.3, 1 };
		//xx::Shared<xx::Slider> uiCamScale;

		//static constexpr xx::FromTo<int32_t> cGenAirplaneNumPerFrameRange{ 1, 1000 };
		//int32_t cGenAirplaneNumPerFrame{ 1 };
		//xx::Shared<xx::Slider> uiGenAirplaneNumPerFrame;

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
