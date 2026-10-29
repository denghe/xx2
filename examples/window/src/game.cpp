#include "pch.h"
#include "game.h"
#include "monster.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	DisableIME();
	title = "examples_window";
	windowSize = designSize = { 1280, 800 };
}

void Game::GLInit() {
	// load res
	pics.heart = LoadTexture("res/heart.png");
	pics.tiled9 = LoadTexture("res/tiled9.png");
	wavs.heart_jump = LoadSoundSource("res/heart_jump.opus");
	wavs.bgm1 = LoadSoundSource("res/bgm1.opus");

	// init cam
	cam.Init(scale, 1.f, {});

	// init ui
	ui.Emplace()->InitRoot(scale);

	ui->Make<xx::Label>()->Init(1, p5 + XY{ 0, -69 }, a5, 48)("hi");

	ui->Make<xx::LabelButton>()->Init(2, p5 + XY{ 0, 60 }, a5, 48)("change color!!!").onClicked = [this] {
		heart->ChangeColor();
	};

	ui->Make<xx::LabelButton>()->Init(3, p5 + XY{ 0, 0 }, a5, 48)("change anim").onClicked = [this] {
		heart->ChangeAnim();
	};

	ui->Make<xx::LabelButton>()->Init(4, p5 + XY{ 0, -60 }, a5, 48)("change bg").onClicked = [this] {
		bgChanged = !bgChanged;
		if (bgChanged) {
			GenTiledBG();
		}
	};

	// init logic
	heart.Emplace()->Init(pics.heart);

	// init tiled bg
	GenTiledBG();

	// play bgm
	PlayMusic(wavs.bgm1, 0.5f);
}

void Game::GenTiledBG() {
	tiledBGScale = rnd.Next<float>(0.1f, 1.0f);
	XYi gridSize{ designSize.x / 32 / tiledBGScale, designSize.y / 32 / tiledBGScale };
	XYi tiledTexSize{ (gridSize.x + 7) & ~7u, (gridSize.y + 7) & ~7u };
	auto d = std::make_unique<std::array<uint8_t, 2>[]>(tiledTexSize.x * tiledTexSize.y);
	for (int y = 0; y < gridSize.y; ++y) {
		for (int x = 0; x < gridSize.x; ++x) {
			auto idx = (uint8_t)rnd.Next<int32_t>(0, 9);
			auto color = (uint8_t)rnd.Next<int32_t>(0, 256);
			d[y * tiledTexSize.x + x] = { idx, color };
		}
	}
	tiledData.Emplace(xx::LoadGLTiledTexture(d.get(), tiledTexSize.x, tiledTexSize.y, gridSize));
}

void Game::Update() {
#if 0
	for (int i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; ++i) {
		xx::Cout(joy.btns[i].pressed, " ");
	}
	for (int i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; ++i) {
		xx::Cout(joy.axes[i], " ");
	}
	xx::CoutN();
#endif

	// handle inputs
	if (keyboard[GLFW_KEY_ESCAPE]) {
		running = false;
		return;
	}
	if (mouse[GLFW_MOUSE_BUTTON_WHEEL_UP](0.01f)
		|| keyboard[GLFW_KEY_Z](0.01f)
		|| joy.btns[GLFW_GAMEPAD_BUTTON_A](0.01f)
		) {
		cam.SetLogicScale(cam.logicScale + 0.001f);
	}
	if (mouse[GLFW_MOUSE_BUTTON_WHEEL_DOWN](0.01f)
		|| keyboard[GLFW_KEY_X](0.01f)
		|| joy.btns[GLFW_GAMEPAD_BUTTON_B](0.01f)
		) {
		cam.SetLogicScale(cam.logicScale - 0.001f);
	}

	// logic update
	heart->Update();


	// bg update & draw
	if (bgChanged) {
		Tiled().Alloc(pics.tiled9.tex, tiledData)->Fill(0, tiledBGScale * cam.scale, 32.f);
	}
	else {
		bgOffset += XY{ 0.5f, 0.5f } * delta;
		QuadEx().DrawFrame(pics.heart
			, { 0.f, 0.f }
			, designSize.x / pics.heart.uvRect.w * cam.scale
			, 100
			, bgOffset
			, 0.f
			, 0.5f
		);
	}

	heart->Draw();
	DrawNode(ui);
}

void Game::Delay() {
#if 0
	// for power saving, fps limit
	SleepSecs(cDelta - (glfwGetTime() - time));
#endif
}

void Game::OnResize(bool modeChanged_) {
	ui->Resize(scale);
	cam.SetBaseScale(scale);
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " delayUpdates.len = ", delayUpdates.len
	);
}
