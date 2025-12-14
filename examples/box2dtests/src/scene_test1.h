#pragma once
#include "game.h"

namespace Test1 {

	struct Scene;

	struct SceneItem1 {
		Scene* scene{};
		xx::B2Body b2body;
		xx::B2Shape b2shape;
		XY halfSize{};
		float scale{};
		int32_t _1{}, i{};
		void Init(Scene* scene_, XY pos_, XY halfSize_);
		bool Update();
		void Draw();
	};

	struct SceneItem2 {
		Scene* scene{};
		xx::B2Body b2body;
		xx::B2Shape b2shape;
		float radius{};
		float scale{};
		int32_t _1{}, i{};
		void Init(Scene* scene_, XY pos_, float radius_);
		bool Update();
		void Draw();
	};

	struct SceneEdge {
		Scene* scene{};
		xx::B2Body b2body;
		xx::B2Shape b2shape;
		XY halfSize{};
		void Init(Scene* scene_, XY pos_, XY halfSize_);
		void Update();
		void Draw();
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::B2World b2world;
		SceneEdge edge;
		xx::List<xx::Shared<SceneItem1>> item1s;
		xx::List<xx::Shared<SceneItem2>> item2s;
		//static constexpr xx::FromTo<float> cCamScaleRange{ 0.3, 1 };
		//xx::Shared<xx::Slider> uiCamScale;

		//static constexpr xx::FromTo<int32_t> cGenAirplaneNumPerFrameRange{ 1, 1000 };
		//int32_t cGenAirplaneNumPerFrame{ 1 };
		//xx::Shared<xx::Slider> uiGenAirplaneNumPerFrame;

		float genTimer{};
		void Gen(int32_t num_);

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
