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

		static constexpr XYi cWallMapSize{ 10, 7 };
		mapSize = cCellSize * cWallMapSize;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		gridBuildings.Init(cCellSize, std::ceilf(mapSize.y / cCellSize), std::ceilf(mapSize.x / cCellSize));
		gridMaterials.Init(cItemSize, std::ceilf(mapSize.y / cItemSize), std::ceilf(mapSize.x / cItemSize));

		GenWallHorizontal(0, cWallMapSize.x - 1, 0);
		GenWallHorizontal(1, cWallMapSize.x - 3, cWallMapSize.y / 2, true);
		GenWallHorizontal(0, cWallMapSize.x - 1, cWallMapSize.y - 1);
		GenWallVertical(0, 1, cWallMapSize.y - 2, true, true);
		GenWallVertical(cWallMapSize.x - 1, 1, cWallMapSize.y - 2, true, true);

		GenFactory(1, 1);
		GenFactory(1, 2);

		GenBox(1, 4);
		GenBox(1, 5);
	}
}
