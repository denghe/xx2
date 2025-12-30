#include "pch.h"
#include "scene_test4.h"

namespace Test4 {
	using namespace G2;

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		// [][][][][][][][][][]
		// []F.              []
		// []F.              []
		// [][][][][][][][]  []
		// []B.              []
		// []B.              []
		// [][][][][][][][][][]

		static constexpr XYi numColumnRows{ 10, 7 };
		mapSize = cCellSize * numColumnRows;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		gridBuildings.Init(cCellSize, std::ceilf(mapSize.y / cCellSize), std::ceilf(mapSize.x / cCellSize));
		gridMaterials.Init(cItemSize, std::ceilf(mapSize.y / cItemSize), std::ceilf(mapSize.x / cItemSize));

		GenWallHorizontal(0, numColumnRows.x - 1, 0);
		GenWallHorizontal(1, numColumnRows.x - 3, numColumnRows.y / 2, true);
		GenWallHorizontal(0, numColumnRows.x - 1, numColumnRows.y - 1);
		GenWallVertical(0, 1, numColumnRows.y - 2, true, true);
		GenWallVertical(numColumnRows.x - 1, 1, numColumnRows.y - 2, true, true);

		GenFactory(1, 1);
		GenFactory(1, 2);

		GenBox(1, 4);
		GenBox(1, 5);
	}
}
