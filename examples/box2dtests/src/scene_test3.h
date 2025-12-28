#pragma once
#include "game.h"

namespace Test3 {

	struct Scene;

	struct SceneItem {
		Scene* scene{};
		SceneItem* next{};
		XY pos{};
		float y{};
		float scale{}, radians{}, radius{};
		int32_t indexAtContainer{ -1 };
		int32_t indexAtGrid{ -1 };
		virtual bool Update() { return false; }
		virtual void Draw() {};
		virtual ~SceneItem() {};
	};

	struct Wall : SceneItem {
		static constexpr float cRadius{ 200.f };
		void Init(Scene* scene_, XY pos_);
		void Draw() override;
		void Dispose();	// unsafe
		~Wall() override;
	};

	struct WoodFactor : SceneItem {
		static constexpr float cRadius{ 150.f };
		static constexpr float cDistances[]{ 1, 2, 3, 2, 1, 0, -1, -2, -1, 0, 1, 0 };
		XY offset{};
		float cos{}, sin{};
		int32_t i{}, j{};
		int32_t _1{}, _2{};
		float nextGenTime{};
		bool shaking{};
		void ShakeA();
		void ShakeB();
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
		void Draw() override;
		void Dispose();	// unsafe
		~WoodFactor() override;
	};

	struct Wood : SceneItem {
		static constexpr float cRadius{ 65.f };
		XY offset{}, inc{};
		float cos{}, sin{};
		int32_t i{};
		int32_t _1{};
		bool ready{};
		void Anim();
		void Init(Scene* scene_, XY pos_);
		void PreUpdate();
		bool Update() override;
		void Draw() override;
		void Dispose();	// unsafe
		~Wood() override;
	};

	struct GridCache {
		XY pos{};
		float radius{};
		void operator=(SceneItem* p);
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		XY mapSize{};
		xx::Grid2dCircle<SceneItem*, GridCache> gridBuildings;	// for factory & wall
		xx::Grid2dCircle<SceneItem*, GridCache> gridMaterials;	// for woods, ...
		xx::List<xx::Shared<Wall>> walls;
		xx::List<xx::Shared<WoodFactor>> factories;
		xx::List<xx::Shared<Wood>> woods;			// life cycle < grid

		xx::List<SceneItem*> sortContainer;			// for draw order by Y
		void SortContainerAdd(SceneItem* o);
		void SortContainerDraw();

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
