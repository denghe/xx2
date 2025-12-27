#pragma once
#include "game.h"

namespace Test3 {

	struct Scene;

	struct SceneItem {
		Scene* scene{};
		SceneItem* next{};
		XY pos{};
		float y{};
		float scale{}, radians{};
		int32_t indexAtContainer{ -1 };
		virtual bool Update() { return false; }
		virtual void Draw() {};
	};

	struct Wood1 : SceneItem {
		static constexpr float cDistances[]{ 1, 2, 3, 2, 1, 0, -1, -2, -1, 0, 1, 0 };
		XY offset{};
		float cos{}, sin{};
		int32_t i{}, j{};
		int32_t _1{}, _2{};
		bool shaking{};
		void ShakeA();
		void ShakeB();
		void Init(Scene* scene_, XY pos_, float scale_ = 1);
		bool Update();
		void Draw();
	};

	struct Wood2 : SceneItem {
		int32_t indexAtGrid{ -1 };
		XY offset{};
		float cos{}, sin{};
		int32_t i{};
		int32_t _1{};
		bool ready{};
		void Anim();
		void Init(Scene* scene_, XY pos_, float scale_ = 1);
		bool Update();
		void Draw();
		void Dispose();	// unsafe
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		XY mapSize{};
		xx::Grid2dCircle<SceneItem*> grid;
		Wood1 wood1;
		xx::List<xx::Shared<Wood2>> woods;

		xx::List<SceneItem*> sortContainer;				// for draw order by Y
		void SortContainerAdd(SceneItem* o);
		void SortContainerDraw();

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
