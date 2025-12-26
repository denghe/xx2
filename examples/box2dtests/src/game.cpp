#include "pch.h"
#include "game.h"
#include "scene_mainmenu.h"
Game gg;

int32_t main() {
	// enter game loop
	return gg.Run();
}

void Game::Init() {
	DisableIME();
	title = "examples_miner";
}

void Game::GLInit() {
	// init global ui
	ui.Emplace()->InitRoot(scale);
	ui->Make<xx::Label>()->Init(2, p9, a9, 32.f)("ESC: back   [: show/hide Info   ]:on/off FPS limit");
	uiFPS = ui->Make<xx::Label>();
	uiFPS->Init(2, p7, a7, 32.f)("");

	// hide hardware mouse
	//SetMousePointerVisible(false);

	// init basedata
	sgrdd.Init(64, 64);

	// begin load res
	fs.circle256 = LoadTexture("res/circle256.png");
	fs.bg_fish = LoadTexture("res/bg_fish.png");

	fs._10 = LoadTexture("res/10.png");
	fs._110 = LoadTexture("res/110.png");
	fs._123 = LoadTexture("res/123.png");
	fs._128 = LoadTexture("res/128.png");
	fs._138 = LoadTexture("res/138.png");
	fs._14 = LoadTexture("res/14.png");
	fs._147 = LoadTexture("res/147.png");
	fs._168 = LoadTexture("res/168.png");
	fs._17 = LoadTexture("res/17.png");
	fs._18 = LoadTexture("res/18.png");
	fs._192 = LoadTexture("res/192.png");
	fs._206 = LoadTexture("res/206.png");
	fs._243 = LoadTexture("res/243.png");
	fs._244 = LoadTexture("res/244.png");
	fs._26 = LoadTexture("res/26.png");
	fs._283 = LoadTexture("res/283.png");
	fs._285 = LoadTexture("res/285.png");
	fs._288 = LoadTexture("res/288.png");
	fs._296 = LoadTexture("res/296.png");
	fs._303 = LoadTexture("res/303.png");
	fs._309 = LoadTexture("res/309.png");
	fs._402 = LoadTexture("res/402.png");
	fs._407 = LoadTexture("res/407.png");
	fs._408 = LoadTexture("res/408.png");
	fs._415 = LoadTexture("res/415.png");
	fs._421 = LoadTexture("res/421.png");
	fs._422 = LoadTexture("res/422.png");
	fs._443 = LoadTexture("res/443.png");
	fs._449 = LoadTexture("res/449.png");
	fs._458 = LoadTexture("res/458.png");
	fs._461 = LoadTexture("res/461.png");
	fs._462 = LoadTexture("res/462.png");
	fs._463 = LoadTexture("res/463.png");
	fs._464 = LoadTexture("res/464.png");
	fs._467 = LoadTexture("res/467.png");
	fs._470 = LoadTexture("res/470.png");
	fs._472 = LoadTexture("res/472.png");
	fs._51 = LoadTexture("res/51.png");
	fs._75 = LoadTexture("res/75.png");
	fs._98 = LoadTexture("res/98.png");

	// combine all.frames & spines.framess
	xx::RectPacker tp;
	for (int32_t i = 0; i < sizeof(fs) / sizeof(xx::Frame); ++i) {
		tp.tfs.Add(((xx::Frame*)&fs)[i]);
	}
	tp.AutoPack();
	tp.Tex().SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);


	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();
}

// game loop
void Game::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_LEFT_BRACKET](0.2f)) {
		isShowInfo = !isShowInfo;
		xx::CoutN("show fps: ", isShowInfo ? "ON":"OFF");
	}
	if (gg.keyboard[GLFW_KEY_RIGHT_BRACKET](0.2f)) {
		isLimitFPS = !isLimitFPS;
		xx::CoutN("limit fps: ", isLimitFPS ? "ON" : "OFF");
	}

	scene->Update();
	if (!minimized) {
		scene->Draw();
	}
	if (oldScene) oldScene.Reset();

	// draw ui
	uiFPS->SetText(fpsVal);
	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
	
	// draw mouse pointer
	//Quad().DrawFrame(all.mouse_pointer, mousePos, 3.f);
}

void Game::Delay() {
	if (isLimitFPS) {
		// for power saving
		SleepSecs(cDelta - (glfwGetTime() - time));
	}
}

void Game::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	if (scene) {
		scene->OnResize(modeChanged_);
	}
}

void Game::OnFocus(bool focused_) {
	if (scene) {
		scene->OnFocus(focused_);
	}
}

void Game::Stat() {
	fpsVal = xx::ToString(drawFPS);
	if (isShowInfo) {
		xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
			, drawCall, " drawVerts = ", drawVerts
			, " uiAutoUpdates.len = ", uiAutoUpdates.len
			, " delayUpdates.len = ", delayUpdates.len
		);
	}
}
