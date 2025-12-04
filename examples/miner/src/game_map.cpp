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
	auto& f = gg.all.bg1;
	gg.Quad().DrawFrame(f, 0, gg.designSize.y / f.Size().y * scene->cam.scale, 0, 0.5f);

}



// todo: show player/scene properties ui
// todo: when talent levelup, change some properties

// properties / talents:
//	scene: mining site / timer
//		how long can it be mined
//		+init rock numbers
//		"drop rate" table for all rock types
//		xxx% chance to spawn 1 rock when a rock is mined
//		xxx% chance to add 1 second to timer when a rock is mined
//		spawn 1 rock every 0.x second
//		
//	player: damage area( mouse circle )
//		size increase
//		attack speed increase
//		damage increase
// 
//  miner: little human/monsters
//		search delay reduce
//		movement speed increase
//		damage increase( base, factor )
//		attack speed increase
//		critical chance / damage factor
//	porter: little human/monsters / unmanned drone
//  gambler: flip a coin likely
//	special miner/porter/gambler:
//		todo( supernatural? magic? ... )
// 
//  artifacts: ( from chests )
//		damage area AI controller: ( auto change pos every ? second )
//			level up effect: reduce the duration of stay
//		harvest: ( more ores from rocks )
//			level up effect: increase ores quantity ???%
//		discount coupon:
//			level up effect: reduce the price of miner summon by ??%
//		sandglass: ( can change game's time scale )
//			level up effect: more buttons of "time scale change" available
//		pile driver: ( full screen range attack )
//			level up effect: reduce cast delay, + damage
//		sacred ox horn: reduces the durability of all rocks by ???%
//			level up effect: +???%
//		hormone: speedup for all miner
//			level up effect: +???%
//		... more

// talent: config + data
// config: node ( id/index, parentId, [children], type, level, levelup condition, pos, info panel )
// data: [id+level]...
// colors: 
// can't levelup: red
// can levelup: green
// levelup max: blue
