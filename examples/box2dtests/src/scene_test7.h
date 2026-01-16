#pragma once
#include "game.h"

namespace Test7 {


	struct Scene;
	struct SceneBase {
		Scene* scene{};
		xx::Frame frame;
		xx::B2Body b2body;
		xx::B2Shape b2shape;
		XY pos{};
		float y{}, radians{};
		virtual bool Update();
		virtual void Draw();
	};

	struct Wall : SceneBase {
		static constexpr XY cHullPoints[] = { {240,135},{1680,135},{1680,405},{720,405},{720,675},{1680,675},{1680,945},{240,945} };
		std::array<b2Vec2, 8> b2HullPoints{};
		XY scale{};
		void Init(Scene* scene_);
		void Draw() override;
	};

	struct Rock : SceneBase {
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::B2World b2world;
		xx::Shared<Wall> wall;
		xx::List<xx::Shared<Rock>> rocks;

		float genTimer{};
		void Gen(int32_t num_);

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
