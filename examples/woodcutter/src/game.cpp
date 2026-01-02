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
	{
		auto LoadSpineToFrames = [](std::string filePrefix, std::string animName
			, xx::List<xx::Frame>& outFrames, xx::List<xx::SpineEventData>& outEventDatas
			, XY size_, XY offset_, XY drawScale_, float frameDelta_) {
			xx::Shared<xx::GLTexture> t;
			spine::SkeletonData* s{};
			xx::gSpineEnv.Load(filePrefix, s, t);
			auto a = s->findAnimation(animName.c_str());
			xx::SpineToFrames(outFrames, outEventDatas, s, a, size_, offset_, drawScale_, frameDelta_);
		};
		LoadSpineToFrames("res/_spine_export/t_0", "idle", treeIdles.Emplace(), spineEventDatas
				, { 90 + 90, 5 + 150 }, { 90, 5 }, 1.f, gg.cDelta * 0.5f);
		LoadSpineToFrames("res/_spine_export/t_1", "idle", treeIdles.Emplace(), spineEventDatas
				, { 95 + 95, 5 + 175 }, { 95, 5 }, 1.f, gg.cDelta * 0.5f);
		LoadSpineToFrames("res/_spine_export/t_2", "idle", treeIdles.Emplace(), spineEventDatas
				, { 130 + 130, 5 + 190 }, { 130, 5 }, 1.f, gg.cDelta * 0.5f);
		LoadSpineToFrames("res/_spine_export/t_3", "idle", treeIdles.Emplace(), spineEventDatas
			, { 150 + 150, 5 + 215 }, { 150, 5 }, 1.f, gg.cDelta * 0.5f);
		LoadSpineToFrames("res/_spine_export/t_4", "idle", treeIdles.Emplace(), spineEventDatas
			, { 155 + 155, 30 + 190 }, { 155, 30 }, 1.f, gg.cDelta * 0.5f);
		LoadSpineToFrames("res/_spine_export/t_5", "idle", treeIdles.Emplace(), spineEventDatas
			, { 140 + 140, 30 + 185 }, { 140, 30 }, 1.f, gg.cDelta * 0.5f);
		LoadSpineToFrames("res/_spine_export/t_6", "idle", treeIdles.Emplace(), spineEventDatas
			, { 125 + 125, 40 + 155 }, { 125, 40 }, 1.f, gg.cDelta * 0.5f);

		// combine all.frames & spines.framess
		xx::RectPacker tp;
		for (int32_t i = 0; i < sizeof(_pics) / sizeof(xx::Frame); ++i) {
			tp.tfs.Add(((xx::Frame*)&_pics)[i]);
		}
		for (auto& fs : treeIdles) for (auto& f : fs) tp.tfs.Add(f);
		tp.AutoPack();
		tp.Tex().SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);

		// clean up
		xx::gSpineEnv.Clear();
	}

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
