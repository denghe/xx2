#pragma once
#include "game.h"

namespace Test6 {


	struct Scene;
	struct SceneItem {
		Scene* scene{};
		xx::Frame frame;
		xx::B2Body b2body;
		xx::B2Shape b2shape;
		XY pos{};
		float y{}, radians{};
		virtual bool Update();
		virtual void Draw();
	};

	struct Bag : SceneItem {
		static constexpr XY cHullPoints[] = { { -31.97f, 112.95f }, { -45.97f, 110.95f }, { -61.97f, 105.95f }, { -82.97f, 93.95f }, { -93.97f, 83.95f }, { -93.97f, 81.95f }, { -104.97f, 67.95f }, { -114.97f, 43.95f }, { -116.97f, 32.95f }, { -116.97f, 1.9500000000000028f },	{ -110.97f, -19.049999999999997f }, { -100.97f, -42.05f }, { -76.97f, -76.05f }, { -62.97f, -90.05f }, { -35.97f, -105.05f }, { -23.97f, -109.05f }, { -14.969999999999999f, -111.05f }, { 14.030000000000001f, -112.05f }, { 30.03f, -108.05f }, { 47.03f, -100.05f }, { 67.03f, -84.05f }, { 67.03f, -82.05f }, { 89.03f, -56.05f }, { 100.03f, -37.05f }, { 110.03f, -15.049999999999997f }, { 116.03f, 5.950000000000003f }, { 116.03f, 41.95f }, { 110.03f, 59.95f }, { 100.03f, 76.95f }, { 88.03f, 89.95f }, { 86.03f, 89.95f }, { 73.03f, 99.95f }, { 61.03f, 105.95f }, {38.03, 112.95} };
		std::array<b2Vec2, 34> b2HullPoints{};
		XY scale{};
		void Init(Scene* scene_, XY pos_, XY scale_);
		void Draw() override;
	};

	struct Rock : SceneItem {
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::B2World b2world;
		xx::Shared<Bag> bag;
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
