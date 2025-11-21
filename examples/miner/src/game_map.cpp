#include "pch.h"
#include "game_map.h"
#include "game_scene.h"

void Map::Init(Scene* scene_) {
	scene = scene_;

	float totalScale_ = 1.f;
	XYi cGridSize{ 80 * totalScale_, 15 * totalScale_ };
	scene->cRockMargin = gg.designSize / cGridSize;
	scene->cRockMarginOffsetRange = { scene->cRockMargin / 4 };
	scene->cRocksScale = 0.4f / totalScale_;
	scene->cRocksMaxCount = cGridSize.x * cGridSize.y;
	scene->cMouseCircleRadius = 128.f;
	scene->cRocksPivotOffset = { 0, -scene->cRockRadius * scene->cRocksScale };

	auto cellSize = (int32_t)std::ceilf(std::max(scene->cRockMargin.x, scene->cRockMargin.y));
	auto numCRs = gg.designSize.As<int32_t>() / cellSize + 1;
	scene->rocksGrid.Init(cellSize, numCRs.y, numCRs.x);
	scene->rocks.Reserve(scene->cRocksMaxCount);

	// pos filter
	auto& img = gg.stbi.bg1a;
	assert(img.comp == 4);
	auto s = 1.f / (gg.designSize / img.Size());

	XY basePoss[]{ 
		{ scene->cRockMargin.x * 0.5f, scene->cRockMargin.y * 0.25f }, 
		{ scene->cRockMargin.x * 0.5f, scene->cRockMargin.y * 0.75f }
	};
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			XY pos{ 
				basePos.x + scene->cRockMargin.x * x, 
				basePos.y + scene->cRockMargin.y * y 
			};
			auto ipos = (pos * s).As<int32_t>();
			auto cidx = ipos.y * img.w + ipos.x;
			if (img.At(cidx).a) {
				scene->rocksFixedPosPool.Emplace(pos);
			}
		}
	}
	assert(scene->rocksFixedPosPool.len <= scene->cRocksMaxCount);
	scene->rocksFixedPoss.AddRange(scene->rocksFixedPosPool);
}

void Map::Update() {
}

void Map::Draw() {
	// draw bg
	gg.Quad().Draw(gg.tf.bg1, gg.tf.bg1, 0, 0.5f, gg.designSize.y / gg.tf.bg1.Size().y * scene->cam.scale, 0, 0.5f);

}
