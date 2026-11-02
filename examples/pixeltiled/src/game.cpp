#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Delay() {
	SleepSecs(cDelta - (glfwGetTime() - time));	// for power saving, fps limit
}

void Game::OnResize(bool modeChanged_) {
	ui->Resize(scale);
	cam.SetBaseScale(scale);
}

void Game::Stat() {
	fpsLabel->SetText(xx::ToString("fps = ", drawFPS, " breakable blocks num = ", map->breakPixelCount));
}

void Game::Init() {
	DisableIME();
	title = "examples_pixeltiled";
	windowSize = designSize = { 1920, 1080 };
}

void Game::GLInit() {
	// load res
	pics.tiled10 = LoadTexture("res/tiled10.png");

	// load maps data
	map.Emplace()->Init();
	// ...

	// init cam
	cam.Init(scale
		, std::min(gg.designSize.x / map->pixelSize.x, gg.designSize.y / map->pixelSize.y)
		, map->pixelSize * 0.5f);

	// init ui
	ui.Emplace()->InitRoot(scale);

	fpsLabel = ui->Make<xx::Label>();
	fpsLabel->Init(1, p5 - designSize * 0.5f + XY{ 30, 30 }, a1, 48)("fps = ");

	//ui->Make<xx::LabelButton>()->Init(2, p5 + XY{ 0, 60 }, a5, 48)("refresh").onClicked = [this] {
	//};
}

void Game::Update() {
	if (HandleInputs()) return;
	Logic();
	Draw();
}

bool Game::HandleInputs() {
	if (keyboard[GLFW_KEY_ESCAPE]) {
		running = false;
		return true;
	}
	if (mouse[GLFW_MOUSE_BUTTON_WHEEL_UP](0.01f) || keyboard[GLFW_KEY_Z](0.01f) || joy.btns[GLFW_GAMEPAD_BUTTON_A](0.01f)) {
		cam.SetLogicScale(cam.logicScale * 1.1f);
	}
	if (mouse[GLFW_MOUSE_BUTTON_WHEEL_DOWN](0.01f) || keyboard[GLFW_KEY_X](0.01f) || joy.btns[GLFW_GAMEPAD_BUTTON_B](0.01f)) {
		if (cam.logicScale > 0.1f) cam.SetLogicScale(cam.logicScale * 0.95f);
	}
	return false;
}

void Game::Logic() {
	// todo: mouse click to dig( light effect handle )
	if (mouse[GLFW_MOUSE_BUTTON_LEFT]) {
		auto mp = cam.ToLogicPos(mousePos);
		if (mp.x >= 0 && mp.y >= 0 && mp.x < map->pixelSize.x && mp.y < map->pixelSize.y) {
			XYi cri = mp / map->cellSize;
			map->TryDig1x1(cri);
		}
	}
	if (mouse[GLFW_MOUSE_BUTTON_RIGHT]) {
		auto mp = cam.ToLogicPos(mousePos);
		if (mp.x >= 0 && mp.y >= 0 && mp.x < map->pixelSize.x && mp.y < map->pixelSize.y) {
			XYi cri = mp / map->cellSize;
			for (int32_t x = cri.x - 2; x <= cri.x + 2; ++x) {
				for (int32_t y = cri.y - 2; y <= cri.y + 2; ++y) {
					map->TryDig1x1({ x, y });
				}
			}
		}
	}
}

void Game::Draw() {
	// ...
	map->Draw();
	DrawNode(ui);
}

void Map::Init() {
	static constexpr int32_t spacing{ 1 };
	static constexpr int32_t borderWidth{ 4 };
	static constexpr int32_t expandSize{ (spacing + borderWidth) * 2 };

	// load image data
	xx::STBImage img;
	img.Fill(gg.LoadFileData("res/map1.png"));
	assert(img.w > 3 && img.h > 3);

	// size calc
	XYi imgSize{ img.w, img.h };
	XYi contentSize{ img.w + expandSize, img.h + expandSize };
	contentSize = { (contentSize.x + 7) & ~7u, (contentSize.y + 7) & ~7u };	// align to 8
	XYi contentOffset{ contentSize / 2 - imgSize / 2 };
	int32_t spacingLeft = contentOffset.x - borderWidth;
	int32_t spacingTop = contentOffset.y - borderWidth;
	int32_t spacingRight = contentSize.x - imgSize.x - spacingLeft - borderWidth * 2;
	int32_t spacingBottom = contentSize.y - imgSize.y - spacingTop - borderWidth * 2;
	mapSize = contentSize;
	pixelSize = mapSize * cellSize;

	auto HaveBlock = [&](int32_t x_, int32_t y_) {
		if (x_ < 0 || y_ < 0 || x_ >= img.w || y_ >= img.h) return true;
		return img.At3(y_ * img.w + x_).r > 0;
		};

	// search bug( unreachable space )
	// _X_
	// XOX
	// _X_
	bool hasBug{};
	for (int32_t y = 1; y < img.h - 1; ++y) {
		for (int32_t x = 1; x < img.w - 1; ++x) {
			if (img.At3(y * img.w + x).r == 0) {
				if (HaveBlock(x, y - 1) && HaveBlock(x, y + 1) && HaveBlock(x - 1, y) && HaveBlock(x + 1, y)) {
					xx::CoutN("map bug pos: ", x, ", ", y);
					hasBug = true;
				}
			}
		}
	}
	if (hasBug) {
		xx::CoutN("map data has bug");
		assert(false);
	}

	// alloc data
	texData = std::make_unique_for_overwrite<std::array<uint8_t, 2>[]>(contentSize.x * contentSize.y);
	mapData = std::make_unique_for_overwrite<Block[]>(contentSize.x * contentSize.y);

	// fill tex & map data
	for (int32_t y = 0; y < img.h; ++y) {
		for (int32_t x = 0; x < img.w; ++x) {
			auto c = img.At3(y * img.w + x);
			auto idx = (y + contentOffset.y) * contentSize.x + (x + contentOffset.x);
			if (c.r) {
				++whitePixelCount;
				texData[idx] = { 9, 0 };
				mapData[idx].type = BlockTypes::Static;
			}
			else {
				++breakPixelCount;
				texData[idx] = { (uint8_t)gg.rnd.Next<int32_t>(0, 9), 0 };
				mapData[idx].type = BlockTypes::Breakable;
			}
		}
	}

	// fill border
	// top
	for (int32_t y = 0; y < borderWidth; ++y) {
		for (int32_t x = 0; x < contentSize.x; ++x) {
			++whitePixelCount;
			auto idx = y * contentSize.x + x;
			texData[idx] = { 9, 0 };
			mapData[idx].type = BlockTypes::Static;
		}
	}
	// bottom
	for (int32_t y = contentSize.y - borderWidth; y < contentSize.y; ++y) {
		for (int32_t x = 0; x < contentSize.x; ++x) {
			++whitePixelCount;
			auto idx = y * contentSize.x + x;
			texData[idx] = { 9, 0 };
			mapData[idx].type = BlockTypes::Static;
		}
	}
	// left
	for (int32_t y = 0 + borderWidth; y < contentSize.y - borderWidth; ++y) {
		for (int32_t x = 0; x < borderWidth; ++x) {
			++whitePixelCount;
			auto idx = y * contentSize.x + x;
			texData[idx] = { 9, 0 };
			mapData[idx].type = BlockTypes::Static;
		}
	}
	// right
	for (int32_t y = 0 + borderWidth; y < contentSize.y - borderWidth; ++y) {
		for (int32_t x = contentSize.x - borderWidth; x < contentSize.x; ++x) {
			++whitePixelCount;
			auto idx = y * contentSize.x + x;
			texData[idx] = { 9, 0 };
			mapData[idx].type = BlockTypes::Static;
		}
	}

	// fill spacing
	// top
	for (int32_t y = borderWidth; y < borderWidth + spacingTop; ++y) {
		for (int32_t x = borderWidth; x < contentSize.x - borderWidth; ++x) {
			++breakPixelCount;
			auto idx = y * contentSize.x + x;
			texData[idx] = { (uint8_t)gg.rnd.Next<int32_t>(0, 9), 0 };
			mapData[idx].type = BlockTypes::Breakable;
		}
	}
	// bottom
	for (int32_t y = contentSize.y - borderWidth - spacingBottom; y < contentSize.y - borderWidth; ++y) {
		for (int32_t x = borderWidth; x < contentSize.x - borderWidth; ++x) {
			++breakPixelCount;
			auto idx = y * contentSize.x + x;
			texData[idx] = { (uint8_t)gg.rnd.Next<int32_t>(0, 9), 0 };
			mapData[idx].type = BlockTypes::Breakable;
		}
	}
	// left
	for (int32_t y = borderWidth + spacingTop; y < contentSize.y - borderWidth - spacingBottom; ++y) {
		for (int32_t x = borderWidth; x < borderWidth + spacingLeft; ++x) {
			++breakPixelCount;
			auto idx = y * contentSize.x + x;
			texData[idx] = { (uint8_t)gg.rnd.Next<int32_t>(0, 9), 0 };
			mapData[idx].type = BlockTypes::Breakable;
		}
	}
	// right
	for (int32_t y = borderWidth + spacingTop; y < contentSize.y - borderWidth - spacingBottom; ++y) {
		for (int32_t x = contentSize.x - borderWidth - spacingRight; x < contentSize.x - borderWidth; ++x) {
			++breakPixelCount;
			auto idx = y * contentSize.x + x;
			texData[idx] = { (uint8_t)gg.rnd.Next<int32_t>(0, 9), 0 };
			mapData[idx].type = BlockTypes::Breakable;
		}
	}

	assert(whitePixelCount + breakPixelCount == contentSize.x * contentSize.y);

	// random dig 3x3
	xx::List<XYi> breakableIdxs;
	for (int32_t y = 1; y < mapSize.y - 1; ++y) {
		for (int32_t x = 1; x < mapSize.x - 1; ++x) {
			if (CheckBreakable3x3({ x, y })) {
				breakableIdxs.Emplace(x, y);
			}
		}
	}
	auto idx = gg.rnd.Next<int32_t>(0, breakableIdxs.Count());
	Dig3x3(breakableIdxs[idx]);
}

bool Map::CheckBreakable3x3(XYi cri_) {
	assert(cri_.x > 0 && cri_.y > 0 && cri_.x < mapSize.x - 1 && cri_.y < mapSize.y - 1);
	for (int32_t y = cri_.y - 1; y <= cri_.y + 1; ++y) {
		for (int32_t x = cri_.x - 1; x <= cri_.x + 1; ++x) {
			auto idx = y * mapSize.x + x;
			if (mapData[idx].type != BlockTypes::Breakable) return false;
		}
	}
	return true;
}

void Map::TryDig1x1(XYi cri_) {
	if (cri_.x < 0 || cri_.y < 0 || cri_.x >= mapSize.x || cri_.y >= mapSize.y) return;
	// set space
	auto x = cri_.x;
	auto y = cri_.y;
	auto idx = y * mapSize.x + x;
	if (mapData[idx].type != BlockTypes::Breakable) return;
	--breakPixelCount;
	mapData[idx].type = BlockTypes::Space;
	texData[idx][0] = 10;	// set to space
	texData[idx][1] = 0;	// set light effect to 0

	// light 255
	neighbors.Clear();
	TryAdd8Neighbors(x, y, 255);
	LightLeighbors(100);
	LightLeighbors(20);
}

void Map::Dig3x3(XYi cri_) {
	assert(cri_.x > 0 && cri_.y > 0 && cri_.x < mapSize.x - 1 && cri_.y < mapSize.y - 1);

	// set spaces
	for (int32_t y = cri_.y - 1; y <= cri_.y + 1; ++y) {
		for (int32_t x = cri_.x - 1; x <= cri_.x + 1; ++x) {
			auto idx = y * mapSize.x + x;
			assert(mapData[idx].type != BlockTypes::Space);
			mapData[idx].type = BlockTypes::Space;
			texData[idx][0] = 10;	// set to space
			texData[idx][1] = 0;	// set light effect to 0
			--breakPixelCount;
		}
	}

	// light 255
	neighbors.Clear();
	for (int32_t y = cri_.y - 1; y <= cri_.y + 1; ++y) {
		for (int32_t x = cri_.x - 1; x <= cri_.x + 1; ++x) {
			TryAdd8Neighbors(x, y, 255);
		}
	}
	LightLeighbors(100);
	LightLeighbors(20);
}

void Map::LightLeighbors(uint8_t c) {
	bak.Clear();
	bak.AddRange(neighbors);
	neighbors.Clear();
	for (auto& xy : bak) {
		TryAdd8Neighbors(xy.x, xy.y, c);
	}
}

void Map::TryAddNeighbor(int32_t nx, int32_t ny, uint8_t c) {
	auto& cell = texData[ny * mapSize.x + nx];
	if (cell[0] < 10 && cell[1] < c) {
		cell[1] = c;
		neighbors.Emplace(nx, ny);
	}
};

void Map::TryAdd8Neighbors(int32_t x, int32_t y, uint8_t c) {
	// search 8 neighbor, set light effect c
	// X__
	// _O_
	// ___
	if (x > 0 && y > 0) TryAddNeighbor(x - 1, y - 1, c);
	// _X_
	// _O_
	// ___
	if (y > 0) TryAddNeighbor(x, y - 1, c);
	// __X
	// _O_
	// ___
	if (x < mapSize.x - 1 && y > 0) TryAddNeighbor(x + 1, y - 1, c);
	// ___
	// XO_
	// ___
	if (x > 0) TryAddNeighbor(x - 1, y, c);
	// ___
	// _OX
	// ___
	if (x < mapSize.x - 1) TryAddNeighbor(x + 1, y, c);
	// ___
	// _O_
	// X__
	if (x > 0 && y < mapSize.y - 1) TryAddNeighbor(x - 1, y + 1, c);
	// ___
	// _O_
	// _X_
	if (y < mapSize.y - 1) TryAddNeighbor(x, y + 1, c);
	// ___
	// _O_
	// __X
	if (x < mapSize.x - 1 && y < mapSize.y - 1) TryAddNeighbor(x + 1, y + 1, c);
};

void Map::FillTex() {
	tex.Emplace(xx::LoadGLTiledTexture(texData.get(), mapSize.x, mapSize.y, mapSize));
}

void Map::Draw() {
	FillTex();
	gg.Tiled().Alloc(gg.pics.tiled10.tex, tex)->Fill(0, gg.cam.scale, cellSize);
}
