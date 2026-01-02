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
	title = "examples_woodcutter";
}

void Game::GLInit() {
	// init global ui
	ui.Emplace()->InitRoot(scale);
	ui->Make<xx::Label>()->Init(2, p9, a9, 32.f)("ESC: back   [: show/hide Info   ]:on/off FPS limit");
	uiFPS = ui->Make<xx::Label>();
	uiFPS->Init(2, p7, a7, 32.f)("");

	// hide hardware mouse
	SetMousePointerVisible(false);

	// init basedata
	sgrdd.Init(64, 64);

	// begin load res
	// load texture packer
	_pics.Load("res/");
	// load file data( for memory scan )
	_mask_bg_1.Fill(LoadFileData("res/_mask_bg_1.png"));

	// load spines
	xx::LoadSpineToFrames("res/_spine_export/t_0", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_1", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_2", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_3", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_4", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_5", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_6", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);

	xx::LoadSpineToFrames("res/_spine_export/t_0", "idle", treeTurnLefts.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_1", "idle", treeTurnLefts.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_2", "idle", treeTurnLefts.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_3", "idle", treeTurnLefts.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_4", "idle", treeTurnLefts.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_5", "idle", treeTurnLefts.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_6", "idle", treeTurnLefts.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0, 0.5f);

	xx::LoadSpineToFrames("res/_spine_export/t_0", "idle", treeTurnRights.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_1", "idle", treeTurnRights.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_2", "idle", treeTurnRights.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_3", "idle", treeTurnRights.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_4", "idle", treeTurnRights.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_5", "idle", treeTurnRights.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0.5f);
	xx::LoadSpineToFrames("res/_spine_export/t_6", "idle", treeTurnRights.Emplace(), spineEventDatas, 1.f, 1.f / 30, 0.5f);

	xx::gSpineEnv.Clear();	// clean up


	// combine all.frames & spines.framess
	xx::RectPacker tp;
	for (int32_t i = 0; i < sizeof(_pics) / sizeof(xx::Frame); ++i) {
		tp.tfs.Add(((xx::Frame*)&_pics)[i]);
	}
	for (auto& fs : treeIdles) for (auto& f : fs) tp.tfs.Add(f);
	for (auto& fs : treeTurnLefts) for (auto& f : fs) tp.tfs.Add(f);
	for (auto& fs : treeTurnRights) for (auto& f : fs) tp.tfs.Add(f);
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

		// draw ui
		uiFPS->SetText(fpsVal);
		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);

		// draw mouse pointer
		Quad().DrawFrame(_pics.a_[0], mousePos);
	}
	if (oldScene) oldScene.Reset();
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
