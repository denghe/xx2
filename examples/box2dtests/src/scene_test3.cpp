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

		static constexpr XYi numColumnRows{ 15, 11 };
		mapSize = cCellSize * numColumnRows;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		gridBuildings.Init(cCellSize, std::ceilf(mapSize.y / cCellSize), std::ceilf(mapSize.x / cCellSize));
		gridMaterials.Init(cItemSize, std::ceilf(mapSize.y / cItemSize), std::ceilf(mapSize.x / cItemSize));

		GenWallHorizontal(0, numColumnRows.x - 1, 0);
		GenWallHorizontal(0, numColumnRows.x - 1, numColumnRows.y - 1);
		GenWallVertical(0, 1, numColumnRows.y - 2, true, true);
		GenWallVertical(numColumnRows.x - 1, 1, numColumnRows.y - 2, true, true);

		GenFactory(4, 4);
		GenFactory(6, 4);
		GenFactory(8, 4);
		GenFactory(10, 4);
		GenFactory(4, 6);
		GenFactory(6, 6);
		GenFactory(8, 6);
		GenFactory(10, 6);

		GenBox(1, 1);
		GenBox(numColumnRows.x - 2, 1);
		GenBox(1, numColumnRows.y - 2);
		GenBox(numColumnRows.x - 2, numColumnRows.y - 2);
	}

}
