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
	// load image data
	xx::STBImage img;
	img.Fill(gg.LoadFileData("res/map1.png"));
	mapSize = { (img.w + 7) & ~7u, (img.h + 7) & ~7u };
	pixelSize = { img.w * cellSize.x, img.h * cellSize.y };

	// fill tex data
	data = std::make_unique_for_overwrite<std::array<uint8_t, 2>[]>(mapSize.x * mapSize.y);
	for (int32_t y = 0; y < img.h; ++y) {
		for (int32_t x = 0; x < img.w; ++x) {
			auto c = img.At3(y * img.w + x);
			if (c.r) {
				++whitePixelCount;
				data[y * mapSize.x + x] = { 9, 255 };
			}
			else {
				++breakPixelCount;
				data[y * mapSize.x + x] = {
					(uint8_t)gg.rnd.Next<int32_t>(0, 9)
					, 255
				};
			}
		}
	}
	// fill the rest of the data with white pixels
	if (img.w < mapSize.x) {
		for (int32_t y = 0; y < img.h; ++y) {
			for (int32_t x = img.w; x < mapSize.x; ++x) {
				++whitePixelCount;
				data[y * mapSize.x + x] = { 9, 255 };
			}
		}
	}
	if (img.h < mapSize.y) {
		for (int32_t y = img.h; y < mapSize.y; ++y) {
			for (int32_t x = 0; x < mapSize.x; ++x) {
				++whitePixelCount;
				data[y * mapSize.x + x] = { 9, 255 };
			}
		}
	}

	// todo
	// search space & dig a 3*3 hole
	// beginPos = 
	// color mul calcuate

	// convert data to texture
	tex.Emplace(xx::LoadGLTiledTexture(data.get(), mapSize.x, mapSize.y, mapSize));
}

void Map::Draw() {
	gg.Tiled().Alloc(gg.pics.tiled10.tex, tex)->Fill(0, drawScale * gg.cam.scale, cellSize);
}
