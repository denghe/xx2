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
	pics.Load("res/pics.pvr.zst", false);

	fishsFramePhysMaps.Adds({
		{&pics._10, _phys::_10::Init},
		{&pics._110, _phys::_110::Init},
		{&pics._123, _phys::_123::Init},
		{&pics._128, _phys::_128::Init},
		{&pics._138, _phys::_138::Init},
		{&pics._14, _phys::_14::Init},
		{&pics._147, _phys::_147::Init},
		{&pics._168, _phys::_168::Init},
		{&pics._17, _phys::_17::Init},
		{&pics._18, _phys::_18::Init},
		{&pics._192, _phys::_192::Init},
		{&pics._206, _phys::_206::Init},
		{&pics._243, _phys::_243::Init},
		{&pics._244, _phys::_244::Init},
		{&pics._26, _phys::_26::Init},
		{&pics._283, _phys::_283::Init},
		{&pics._285, _phys::_285::Init},
		{&pics._288, _phys::_288::Init},
		{&pics._296, _phys::_296::Init},
		{&pics._303, _phys::_303::Init},
		{&pics._309, _phys::_309::Init},
		{&pics._402, _phys::_402::Init},
		{&pics._407, _phys::_407::Init},
		{&pics._408, _phys::_408::Init},
		{&pics._415, _phys::_415::Init},
		{&pics._421, _phys::_421::Init},
		{&pics._422, _phys::_422::Init},
		{&pics._443, _phys::_443::Init},
		{&pics._449, _phys::_449::Init},
		{&pics._458, _phys::_458::Init},
		{&pics._461, _phys::_461::Init},
		{&pics._462, _phys::_462::Init},
		{&pics._463, _phys::_463::Init},
		{&pics._464, _phys::_464::Init},
		{&pics._467, _phys::_467::Init},
		{&pics._470, _phys::_470::Init},
		{&pics._472, _phys::_472::Init},
		{&pics._51, _phys::_51::Init},
		{&pics._75, _phys::_75::Init},
		{&pics._98, _phys::_98::Init},
		// ...
	});

	rocksFramePhysMaps.Adds({
		{&pics.rock1_[0], _phys_rocks::rock1_1::Init},
		{&pics.rock1_[1], _phys_rocks::rock1_2::Init},
		{&pics.rock1_[2], _phys_rocks::rock1_3::Init},
		{&pics.rock1_[3], _phys_rocks::rock1_4::Init},
		{&pics.rock1_[4], _phys_rocks::rock1_5::Init},
		{&pics.rock1_[5], _phys_rocks::rock1_6::Init},
		// ...
	});

	//// combine all.frames & spines.framess
	//xx::RectPacker tp;
	//for (int32_t i = 0; i < sizeof(fs) / sizeof(xx::Frame); ++i) {
	//	tp.tfs.Add(((xx::Frame*)&fs)[i]);
	//}
	//tp.AutoPack();
	////tp.Tex().SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);


	SetDefaultBlendPremultipliedAlpha(true);


	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();
}

// game loop
void Game::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_LEFT_BRACKET](0.2f)) {
		isShowInfo = !isShowInfo;
		xx::CoutN("show fps: ", isShowInfo ? "ON" : "OFF");
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
	gg.SetBlendPremultipliedAlpha(false);
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
