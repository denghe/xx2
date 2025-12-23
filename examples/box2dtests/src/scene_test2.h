#pragma once
#include "game.h"

namespace Test2 {

	struct Scene;

	struct SceneItem1 {
		Scene* scene{};
		xx::Frame frame;
		xx::B2Body b2body;
		void Init(Scene* scene_, XY pos_);
		bool Update();
		void Draw();
	};

	struct SceneItem2 {
		Scene* scene{};
		float radius{};
		xx::B2Body b2body;
		void Init(Scene* scene_, XY pos_, float radius_);
		bool Update();
		void Draw();
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::B2World b2world;
		xx::List<xx::Shared<SceneItem1>> item1s;
		SceneItem2 item2;	// mouse

		float genTimer{};
		void Gen(int32_t num_);

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
