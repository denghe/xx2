#pragma once
#include "game.h"

namespace G2 {
	static constexpr float cWallRadius{ 200.f };
	static constexpr float cWoodRadius{ 65.f };
	static constexpr float cHalfCellSize{ cWallRadius };
	static constexpr float cCellSize{ cHalfCellSize * 2.f };
	static constexpr float cItemSize{ cWoodRadius * 2.f };
	static constexpr float cUIScale{ 0.5f };

	struct Scene;

	struct SceneItem {
		static constexpr int32_t cTypeId{};
		Scene* scene{};
		SceneItem* next{};
		int32_t typeId{};	// fill by Init: typeId = cTypeId
		XY pos{};
		float y{};
		float scale{}, radians{}, radius{};
		int32_t indexAtContainer{ -1 };
		int32_t indexAtGrid{ -1 };
		virtual bool Update() { return false; }
		virtual void Draw() {};
		virtual void Dispose() {};	// unsafe: container.swapRemove( this )
		virtual ~SceneItem() {};
	};

	struct GridCache {
		XY pos{};
		float radius{};
		void operator=(SceneItem* p);
	};

	struct Wall;
	struct WoodFactory;
	struct Wood;
	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		XY mapSize{};
		xx::Grid2dCircle<SceneItem*, GridCache> gridBuildings;	// for factory & wall
		xx::Grid2dCircle<SceneItem*, GridCache> gridMaterials;	// for woods, ...
		xx::List<xx::Shared<Wall>> walls;
		xx::List<xx::Shared<WoodFactory>> factories;
		xx::List<xx::Shared<Wood>> woods;			// life cycle < grid

		void GenWallHorizontal(int32_t xFrom_, int32_t xTo_, int32_t y_, bool leftOverflow_ = false, bool rightOverflow_ = false);
		void GenWallVertical(int32_t x_, int32_t yFrom_, int32_t yTo_, bool topOverflow_ = false, bool bottomOverflow_ = false);
		void GenFactory(int32_t x_, int32_t y_);
		void GenBox(int32_t x_, int32_t y_);

		xx::List<SceneItem*> sortContainer;			// for draw order by Y
		void SortContainerAdd(SceneItem* o_);
		void SortContainerDraw();

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
