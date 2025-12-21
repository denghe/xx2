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

std::array<std::array<xx::Frame, sizeof(all::rock_0_) / sizeof(xx::Frame)>, 9>& Game::all_rocks_() {
	return (std::array<std::array<xx::Frame, sizeof(all::rock_0_) / sizeof(xx::Frame)>, 9>&)all.rock_0_;
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

	// init shader
	shaderGrass.Init();

	// begin load res
	// load wav ogg
	ss.pickaxe = LoadSoundSource("res/pickaxe.wav");
	ss.rockbreak = LoadSoundSource("res/rockbreak.ogg");
	ss.pop = LoadSoundSource("res/pop.wav");
	ss.miner1_atk = LoadSoundSource("res/miner1_atk.ogg");
	ss.miner2_atk = LoadSoundSource("res/miner2_atk.ogg");
	ss.miner3_atk = LoadSoundSource("res/miner3_atk.ogg");
	ss.miner4_atk = LoadSoundSource("res/miner4_atk.ogg");

	// load texture packer
	all.Load("res/");

	// fill miner some config
	auto ConfigMiner = [this](auto& tfIdle, auto& tfMove, auto& tfAtk, int32_t configIndex) {
		auto& mc = mcs[configIndex];
		mc.fss[0] = tfIdle.data();
		mc.fsLens[0] = tfIdle.size();
		mc.fss[1] = tfMove.data();
		mc.fsLens[1] = tfMove.size();
		mc.fss[2] = tfAtk.data();
		mc.fsLens[2] = tfAtk.size();
	};
	ConfigMiner(all.miner1_idle_, all.miner1_move_, all.miner1_atk_, 0);
	ConfigMiner(all.miner2_idle_, all.miner2_move_, all.miner2_atk_, 1);
	ConfigMiner(all.miner3_idle_, all.miner3_move_, all.miner3_atk_, 2);
	ConfigMiner(all.miner4_idle_, all.miner4_move_, all.miner4_atk_, 3);

	// set hit frame flags
	cd.miner1_atk_[3] = 1;
	cd.miner2_atk_[7] = 1;
	cd.miner3_atk_[4] = 1;
	cd.miner4_atk_[3] = 1;

	// fill miner some configs
	{
		auto& mc = mcs[0];
		mc.animFPS = 12;
		mc.resRadius = 23;
		mc.moveSpeed = 30 / gg.cFps;
		mc.attackRange = 50;
		mc.cd = cd.miner1_atk_.data();
		mc.ss = ss.miner1_atk;
	}
	{
		auto& mc = mcs[1];
		mc.animFPS = 30;
		mc.resRadius = 40;
		mc.moveSpeed = 20 / gg.cFps;
		mc.attackRange = 26;
		mc.cd = cd.miner2_atk_.data();
		mc.ss = ss.miner2_atk;
	}
	{
		auto& mc = mcs[2];
		mc.animFPS = 30;
		mc.resRadius = 50;
		mc.moveSpeed = 30 / gg.cFps;
		mc.attackRange = 44;
		mc.cd = cd.miner3_atk_.data();
		mc.ss = ss.miner3_atk;
	}
	{
		auto& mc = mcs[3];
		mc.animFPS = 5;
		mc.resRadius = 40;
		mc.moveSpeed = 50 / gg.cFps;
		mc.attackRange = 32;
		mc.cd = cd.miner4_atk_.data();
		mc.ss = ss.miner4_atk;
	}

	// load file data( for memory scan )
	stbi.bg1a.Fill(LoadFileData("res/bg1a.png"));
	stbi.minecart_3.Fill(LoadFileData("res/minecart_3.png"));


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
		// grass
		LoadSpineToFrames("res/grass_4", "idle", spines.idles.Emplace(), spines.eventDatas
				, { 275 + 275, 25 + 350 }, { 275, 25 }, 0.2f, gg.cDelta * 0.5f);
		// bird1
		LoadSpineToFrames("res/bird1", "attack", spines.attacks.Emplace(), spines.eventDatas
			, { 300 + 160, 10 + 330 }, { 300, 10 }, 0.4f, gg.cDelta);
		// snail1
		LoadSpineToFrames("res/snail1", "attack", spines.attacks.Emplace(), spines.eventDatas
			, { 370 + 200, 25 + 350 }, { 370, 25 }, 0.26f, gg.cDelta);
		//// bug1
		//LoadSpineToFrames("res/bug1", "attack", spines.attacks.Emplace(), spines.eventDatas
		//	, { 475 + 300, 20 + 660 }, { 475, 20 }, 0.2f, gg.cDelta);
		// creature1
		LoadSpineToFrames("res/creature1", "attack", spines.attacks.Emplace(), spines.eventDatas
			, { 330 + 140, 5 + 390 }, { 330, 5 }, 0.32f, gg.cDelta);
		// ghost1
		LoadSpineToFrames("res/ghost1", "attack", spines.attacks.Emplace(), spines.eventDatas
			, { 340 + 200, 10 + 385 }, { 340, 10 }, 0.32f, gg.cDelta);
		// food1
		LoadSpineToFrames("res/food1", "attack", spines.attacks.Emplace(), spines.eventDatas
			, { 375 + 200, 15 + 440 }, { 375, 15 }, 0.28f, gg.cDelta);
		// food2
		LoadSpineToFrames("res/food2", "attack", spines.attacks.Emplace(), spines.eventDatas
			, { 400 + 200, 15 + 450 }, { 400, 15 }, 0.28f, gg.cDelta);

		//// miners
		//static constexpr float cScale{ 0.6f };
		//std::array<SpineFrameConfig, spines.N> cfgs;
		//cfgs[1 - 1] = {{ 275 + 135, 10 + 400 }, { 275, 10 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[2 - 1] = { { 280 + 175, 5 + 420 }, { 280, 5 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[3 - 1] = { { 325 + 180, 5 + 400 }, { 325, 5 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[4 - 1] = { { 335 + 170, 10 + 440 }, { 335, 10 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[5 - 1] = { { 325 + 170, 10 + 440 }, { 325, 10 }, cScale * 0.412f, gg.cDelta * 2.f };
		//cfgs[6 - 1] = { { 275 + 125, 10 + 390 }, { 275, 10 }, cScale * 0.412f, gg.cDelta * 2.f };
		//cfgs[7 - 1] = { { 200 + 150, 10 + 370 }, { 200, 10 }, cScale * 0.412f, gg.cDelta * 2.f };
		//cfgs[8 - 1] = { { 440 + 300, 20 + 550 }, { 440, 20 }, cScale * 0.26f, gg.cDelta * 2.f };
		//cfgs[9 - 1] = { { 275 + 200, 20 + 425 }, { 275, 20 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[10 - 1] = { { 380 + 110, 20 + 470 }, { 380, 20 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[11 - 1] = { { 335 + 160, 20 + 440 }, { 335, 20 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[12 - 1] = { { 450 + 190, 10 + 580 }, { 450, 10 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[13 - 1] = { { 390 + 190, 20 + 450 }, { 390, 20 }, cScale * 0.312f, gg.cDelta * 2.f };
		//cfgs[14 - 1] = { { 640 + 200, 10 + 850 }, { 640, 10 }, cScale * 0.212f, gg.cDelta * 2.f };
		//cfgs[15 - 1] = { { 1550 + 830, 160 + 1450 }, { 1550, 160 }, cScale * 0.1f, gg.cDelta * 2.f};

		//for (auto i = 0; i < spines.N; ++i) {
		//	auto& c = cfgs[i];
		//	xx::Shared<xx::GLTexture> t;
		//	spine::SkeletonData* s{};
		//	xx::gSpineEnv.Load(xx::ToString("res/miner", i + 1), s, t);
		//	auto a = s->findAnimation("attack");
		//	xx::SpineToFrames(spines.attacks[i], spines.eventDatas,	s, a, c.size, c.offset, c.scale, c.delta);
		//	a = s->findAnimation("idle");
		//	xx::SpineToFrames(spines.idles[i], spines.eventDatas, s, a, c.size, c.offset, c.scale, c.delta);
		//}

		// combine all.frames & spines.framess
		xx::RectPacker tp;
		for (int32_t i = 0; i < sizeof(all) / sizeof(xx::Frame); ++i) {
			tp.tfs.Add((xx::TinyFrame*)&((xx::Frame*)&all)[i]);
		}
		for (auto& fs : spines.idles) for (auto& f : fs) tp.tfs.Add(f);
		for (auto& fs : spines.attacks) for (auto& f : fs) tp.tfs.Add(f);
		tp.AutoPack();
		tp.Tex().SetParm(GL_LINEAR, GL_CLAMP_TO_EDGE);

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
		Quad().DrawFrame(all.mouse_pointer, mousePos, 3.f);
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
