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
	title = "examples_box2dtests";
}

void Game::GLInit() {
	// init global ui
	ui.Emplace()->InitRoot(scale);
	ui->Make<xx::Label>()->Init(2, p9, a9, 32.f)("keys: ESC, [, ]");
	uiFPS = ui->Make<xx::Label>();
	uiFPS->Init(2, p7, a7, 32.f)("");
	uiText = ui->Make<xx::Label>();
	uiText->Init(2, p8, a8, 32.f)("");

	// hide hardware mouse
	//SetMousePointerVisible(false);

	// init basedata
	sgrdd.Init(64, 64);

	// begin load res
	fs.circle256 = LoadTexture("res/circle256.png");
	fs.bg_fish = LoadTexture("res/bg_fish.png");
	fs.wood1 = LoadTexture("res/wood1.png");
	fs.wood1.anchor = { 0.5f, 0.4f };
	fs.wood2 = LoadTexture("res/wood2.png");
	fs.wood2.anchor = { 0.5f, 0.5f };
	fs.wood3 = LoadTexture("res/wood3.png");
	fs.wood3.anchor = { 0.5f, 0.4f };

	fs.fishs[0] = LoadTexture("res/10.png");
	fs.fishs[1] = LoadTexture("res/110.png");
	fs.fishs[2] = LoadTexture("res/123.png");
	fs.fishs[3] = LoadTexture("res/128.png");
	fs.fishs[4] = LoadTexture("res/138.png");
	fs.fishs[5] = LoadTexture("res/14.png");
	fs.fishs[6] = LoadTexture("res/147.png");
	fs.fishs[7] = LoadTexture("res/168.png");
	fs.fishs[8] = LoadTexture("res/17.png");
	fs.fishs[9] = LoadTexture("res/18.png");
	fs.fishs[10] = LoadTexture("res/192.png");
	fs.fishs[11] = LoadTexture("res/206.png");
	fs.fishs[12] = LoadTexture("res/243.png");
	fs.fishs[13] = LoadTexture("res/244.png");
	fs.fishs[14] = LoadTexture("res/26.png");
	fs.fishs[15] = LoadTexture("res/283.png");
	fs.fishs[16] = LoadTexture("res/285.png");
	fs.fishs[17] = LoadTexture("res/288.png");
	fs.fishs[18] = LoadTexture("res/296.png");
	fs.fishs[19] = LoadTexture("res/303.png");
	fs.fishs[20] = LoadTexture("res/309.png");
	fs.fishs[21] = LoadTexture("res/402.png");
	fs.fishs[22] = LoadTexture("res/407.png");
	fs.fishs[23] = LoadTexture("res/408.png");
	fs.fishs[24] = LoadTexture("res/415.png");
	fs.fishs[25] = LoadTexture("res/421.png");
	fs.fishs[26] = LoadTexture("res/422.png");
	fs.fishs[27] = LoadTexture("res/443.png");
	fs.fishs[28] = LoadTexture("res/449.png");
	fs.fishs[29] = LoadTexture("res/458.png");
	fs.fishs[30] = LoadTexture("res/461.png");
	fs.fishs[31] = LoadTexture("res/462.png");
	fs.fishs[32] = LoadTexture("res/463.png");
	fs.fishs[33] = LoadTexture("res/464.png");
	fs.fishs[34] = LoadTexture("res/467.png");
	fs.fishs[35] = LoadTexture("res/470.png");
	fs.fishs[36] = LoadTexture("res/472.png");
	fs.fishs[37] = LoadTexture("res/51.png");
	fs.fishs[38] = LoadTexture("res/75.png");
	fs.fishs[39] = LoadTexture("res/98.png");

	fis.Adds({
		{&fs.fishs[0], _phys::_10::Init},
		{&fs.fishs[1], _phys::_110::Init},
		{&fs.fishs[2], _phys::_123::Init},
		{&fs.fishs[3], _phys::_128::Init},
		{&fs.fishs[4], _phys::_138::Init},
		{&fs.fishs[5], _phys::_14::Init},
		{&fs.fishs[6], _phys::_147::Init},
		{&fs.fishs[7], _phys::_168::Init},
		{&fs.fishs[8], _phys::_17::Init},
		{&fs.fishs[9], _phys::_18::Init},
		{&fs.fishs[10], _phys::_192::Init},
		{&fs.fishs[11], _phys::_206::Init},
		{&fs.fishs[12], _phys::_243::Init},
		{&fs.fishs[13], _phys::_244::Init},
		{&fs.fishs[14], _phys::_26::Init},
		{&fs.fishs[15], _phys::_283::Init},
		{&fs.fishs[16], _phys::_285::Init},
		{&fs.fishs[17], _phys::_288::Init},
		{&fs.fishs[18], _phys::_296::Init},
		{&fs.fishs[19], _phys::_303::Init},
		{&fs.fishs[20], _phys::_309::Init},
		{&fs.fishs[21], _phys::_402::Init},
		{&fs.fishs[22], _phys::_407::Init},
		{&fs.fishs[23], _phys::_408::Init},
		{&fs.fishs[24], _phys::_415::Init},
		{&fs.fishs[25], _phys::_421::Init},
		{&fs.fishs[26], _phys::_422::Init},
		{&fs.fishs[27], _phys::_443::Init},
		{&fs.fishs[28], _phys::_449::Init},
		{&fs.fishs[29], _phys::_458::Init},
		{&fs.fishs[30], _phys::_461::Init},
		{&fs.fishs[31], _phys::_462::Init},
		{&fs.fishs[32], _phys::_463::Init},
		{&fs.fishs[33], _phys::_464::Init},
		{&fs.fishs[34], _phys::_467::Init},
		{&fs.fishs[35], _phys::_470::Init},
		{&fs.fishs[36], _phys::_472::Init},
		{&fs.fishs[37], _phys::_51::Init},
		{&fs.fishs[38], _phys::_75::Init},
		{&fs.fishs[39], _phys::_98::Init},
		// ...
	});


	// combine all.frames & spines.framess
	xx::RectPacker tp;
	for (int32_t i = 0; i < sizeof(fs) / sizeof(xx::Frame); ++i) {
		tp.tfs.Add(((xx::Frame*)&fs)[i]);
	}
	tp.AutoPack();
	//tp.Tex().SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);


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
	if (oldScene) {
		uiText->SetText("");
		oldScene.Reset();
	}

	// draw ui
	auto c = uiColorFlag ? xx::RGBA8_Red : xx::RGBA8_Blue;
	uiColorFlag = !uiColorFlag;
	uiFPS->SetText(fpsVal).SetColor(c);
	uiText->SetColor(c);
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
