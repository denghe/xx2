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
	// load texture packer
	_pics.Load("res/");
	// load single frame
	fs.p11 = LoadTexture("res/p11.png", true);
	fs.p11.tex->SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);
	fs.p11a1 = LoadTexture("res/p11a.png", true);
	fs.p11a1.tex->SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);
	fs.p11a2 = LoadTexture("res/p11a.pnga");
	fs.p11a2.tex->SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);

	// load file data( for memory scan )
	_mask_bg_1.Fill(LoadFileData("res/_mask_bg_1.png"));

	// load spines( known issue: some delay & time duration maybe curse spine draw position.y issue )
	xx::LoadSpineToFrames("res/_spine_export/t_0", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.45f, 0.54f);	// issuefix
	xx::LoadSpineToFrames("res/_spine_export/t_1", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46f, 0.54f);	// issuefix
	xx::LoadSpineToFrames("res/_spine_export/t_2", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_3", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_4", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_5", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);
	xx::LoadSpineToFrames("res/_spine_export/t_6", "idle", treeIdles.Emplace(), spineEventDatas, 1.f, 1.f / 360, 0.46666f, 0.53333f);

	xx::LoadSpineToFrames("res/_spine_export/t_0", "idle", treeTurns.Emplace(), spineEventDatas, 1.f, 1.f / 31);	// issuefix
	xx::LoadSpineToFrames("res/_spine_export/t_1", "idle", treeTurns.Emplace(), spineEventDatas, 1.f, 1.f / 30);
	xx::LoadSpineToFrames("res/_spine_export/t_2", "idle", treeTurns.Emplace(), spineEventDatas, 1.f, 1.f / 30);
	xx::LoadSpineToFrames("res/_spine_export/t_3", "idle", treeTurns.Emplace(), spineEventDatas, 1.f, 1.f / 30);
	xx::LoadSpineToFrames("res/_spine_export/t_4", "idle", treeTurns.Emplace(), spineEventDatas, 1.f, 1.f / 30);
	xx::LoadSpineToFrames("res/_spine_export/t_5", "idle", treeTurns.Emplace(), spineEventDatas, 1.f, 1.f / 30);
	xx::LoadSpineToFrames("res/_spine_export/t_6", "idle", treeTurns.Emplace(), spineEventDatas, 1.f, 1.f / 30);

	xx::gSpineEnv.Clear();	// clean up


	// combine all.frames & spines.framess
	xx::RectPacker tp;
	for (int32_t i = 0; i < sizeof(_pics) / sizeof(xx::Frame); ++i) {
		tp.tfs.Add(((xx::Frame*)&_pics)[i]);
	}
	for (auto& fs : treeIdles) for (auto& f : fs) tp.tfs.Add(f);
	for (auto& fs : treeTurns) for (auto& f : fs) tp.tfs.Add(f);
	//tp.tfs.Add(fs.p11);
	//tp.tfs.Add(fs.p11a);
	tp.AutoPack();
	tp.Tex().SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);
	xx::CoutN(tp.Tex().size);


	// prepare collision detection data for trees
	auto FillCDs = [this](xx::Frame& f, xx::List<_phys::Circle>& cs, auto&& cd, XY of = {}) {
		auto fs = f.Size();
		auto offset = fs * 0.5f - fs * f.anchor;
		for (auto& c : cd) cs.Emplace((c.pos + offset + of).FlipY(), c.r);
	};
	FillCDs(_pics.t_[0], treeCollisionDatas.Emplace(), _phys::t_0, { 0, -3 });
	FillCDs(_pics.t_[1], treeCollisionDatas.Emplace(), _phys::t_1, { 0, -6 });
	FillCDs(_pics.t_[2], treeCollisionDatas.Emplace(), _phys::t_2, { 0, -7 });
	FillCDs(_pics.t_[3], treeCollisionDatas.Emplace(), _phys::t_3, { 0, -4 });
	FillCDs(_pics.t_[4], treeCollisionDatas.Emplace(), _phys::t_4, { 0, -9 });
	FillCDs(_pics.t_[5], treeCollisionDatas.Emplace(), _phys::t_5, { 0, -14 });
	FillCDs(_pics.t_[6], treeCollisionDatas.Emplace(), _phys::t_6, { 0, -12 });

	FillCDs(_pics.a_[0], axeCollisionData, _phys::a_1);


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
