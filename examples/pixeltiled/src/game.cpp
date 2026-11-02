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
	fpsLabel->SetText("fps = " + std::to_string(drawFPS));
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
	cam.Init(scale, 1.f, {});

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
	// tiled data to texture

}

void Game::Draw() {
	// ...
	map->Draw();
	DrawNode(ui);
}


void Map::Init() {
	static constexpr int32_t spacing{ 0 };
	static constexpr int32_t borderWidth{ 5 };
	static constexpr int32_t expandSize{ (spacing + borderWidth) * 2 };

	// load image data
	xx::STBImage img;
	img.Fill(gg.LoadFileData("res/map1.png"));


	// size calc
	XYi imgSize{ img.w, img.h };
	XYi contentSize{ img.w + expandSize, img.h + expandSize };
	contentSize = { (contentSize.x + 7) & ~7u, (contentSize.y + 7) & ~7u };	// align to 8
	XYi contentOffset{ contentSize / 2 - imgSize / 2 };
	int32_t spacingLeft = contentOffset.x - borderWidth;
	int32_t spacingTop = contentOffset.y - borderWidth;
	int32_t spacingRight = contentSize.x - imgSize.x - spacingLeft - borderWidth * 2;
	int32_t spacingBottom = contentSize.y - imgSize.y - spacingTop - borderWidth * 2;

	// store
	mapSize = contentSize;
	pixelSize = mapSize * cellSize;

	// fill tex data
	data = std::make_unique_for_overwrite<std::array<uint8_t, 2>[]>(contentSize.x * contentSize.y);
	for (int32_t y = 0; y < img.h; ++y) {
		for (int32_t x = 0; x < img.w; ++x) {
			auto c = img.At3(y * img.w + x);
			auto& d = data[y * contentSize.x + x];
			if (c.r) {
				++whitePixelCount;
				data[(y + contentOffset.y) * contentSize.x + (x + contentOffset.x)] = { 9, 255 };
			}
			else {
				++breakPixelCount;
				data[(y + contentOffset.y) * contentSize.x + (x + contentOffset.x)] = {
					(uint8_t)gg.rnd.Next<int32_t>(0, 9)
					, 255
				};
			}
		}
	}

	// fill border
	// top
	for (int32_t y = 0; y < borderWidth; ++y) {
		for (int32_t x = 0; x < contentSize.x; ++x) {
			++whitePixelCount;
			data[y * contentSize.x + x] = { 9, 255 };
		}
	}
	// bottom
	for (int32_t y = contentSize.y - borderWidth; y < contentSize.y; ++y) {
		for (int32_t x = 0; x < contentSize.x; ++x) {
			++whitePixelCount;
			data[y * contentSize.x + x] = { 9, 255 };
		}
	}
	// left
	for (int32_t y = 0 + borderWidth; y < contentSize.y - borderWidth; ++y) {
		for (int32_t x = 0; x < borderWidth; ++x) {
			++whitePixelCount;
			data[y * contentSize.x + x] = { 9, 255 };
		}
	}
	// right
	for (int32_t y = 0 + borderWidth; y < contentSize.y - borderWidth; ++y) {
		for (int32_t x = contentSize.x - borderWidth; x < contentSize.x; ++x) {
			++whitePixelCount;
			data[y * contentSize.x + x] = { 9, 255 };
		}
	}

	// fill spacing
	// top
	for (int32_t y = borderWidth; y < borderWidth + spacingTop; ++y) {
		for (int32_t x = borderWidth; x < contentSize.x - borderWidth; ++x) {
			++breakPixelCount;
			data[y * contentSize.x + x] = {
					(uint8_t)gg.rnd.Next<int32_t>(0, 9)
					, 255
			};
		}
	}
	// bottom
	for (int32_t y = contentSize.y - borderWidth - spacingBottom; y < contentSize.y - borderWidth; ++y) {
		for (int32_t x = borderWidth; x < contentSize.x - borderWidth; ++x) {
			++breakPixelCount;
			data[y * contentSize.x + x] = {
					(uint8_t)gg.rnd.Next<int32_t>(0, 9)
					, 255
			};
		}
	}
	// left
	for (int32_t y = borderWidth + spacingTop; y < contentSize.y - borderWidth - spacingBottom; ++y) {
		for (int32_t x = borderWidth; x < borderWidth + spacingLeft; ++x) {
			++breakPixelCount;
			data[y * contentSize.x + x] = {
					(uint8_t)gg.rnd.Next<int32_t>(0, 9)
					, 255
			};
		}
	}
	// right
	for (int32_t y = borderWidth + spacingTop; y < contentSize.y - borderWidth - spacingBottom; ++y) {
		for (int32_t x = contentSize.x - borderWidth - spacingRight; x < contentSize.x - borderWidth; ++x) {
			++breakPixelCount;
			data[y * contentSize.x + x] = {
					(uint8_t)gg.rnd.Next<int32_t>(0, 9)
					, 255
			};
		}
	}





	//for (int32_t y = img.h; y < contentSize.y; ++y) {
	//	for (int32_t x = 0; x < contentSize.x; ++x) {
	//		++breakPixelCount;
	//		data[y * contentSize.x + x] = { 9, 255 };
	//	}
	//}


	assert(whitePixelCount + breakPixelCount == contentSize.x * contentSize.y);

	// todo
	// search space & dig a 3*3 hole
	// beginPos = 
	// color mul calcuate


	// search bug( unreachable space )
	// _X_
	// XOX
	// _X_


	// convert data to texture
	tex.Emplace(xx::LoadGLTiledTexture(data.get(), contentSize.x, contentSize.y, contentSize));
}

void Map::Draw() {
	gg.Tiled().Alloc(gg.pics.tiled10.tex, tex)->Fill(0, drawScale * gg.cam.scale, cellSize);
}
