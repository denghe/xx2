#include "pch.h"
#include "scene_test3.h"

namespace Test3 {
	using namespace G2;

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		// [][][][][][][][][][][][][][][]
		// []B.                      B.[]
		// []                          []
		// []                          []
		// []      F.  F.  F.  F.      []
		// []                          []
		// []      F.  F.  F.  F.      []
		// []                          []
		// []                          []
		// []B.                      B.[]
		// [][][][][][][][][][][][][][][]

		static constexpr XYi cWallMapSize{ 15, 11 };
		mapSize = cCellSize * cWallMapSize;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		gridBuildings.Init(cCellSize, std::ceilf(mapSize.y / cCellSize), std::ceilf(mapSize.x / cCellSize));
		gridMaterials.Init(cItemSize, std::ceilf(mapSize.y / cItemSize), std::ceilf(mapSize.x / cItemSize));

		GenWallHorizontal(0, cWallMapSize.x - 1, 0);
		GenWallHorizontal(0, cWallMapSize.x - 1, cWallMapSize.y - 1);
		GenWallVertical(0, 1, cWallMapSize.y - 2, true, true);
		GenWallVertical(cWallMapSize.x - 1, 1, cWallMapSize.y - 2, true, true);

		GenFactory(4, 4);
		GenFactory(6, 4);
		GenFactory(8, 4);
		GenFactory(10, 4);
		GenFactory(4, 6);
		GenFactory(6, 6);
		GenFactory(8, 6);
		GenFactory(10, 6);

		GenBox(1, 1);
		GenBox(cWallMapSize.x - 2, 1);
		GenBox(1, cWallMapSize.y - 2);
		GenBox(cWallMapSize.x - 2, cWallMapSize.y - 2);
	}

}
