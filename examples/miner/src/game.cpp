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
	auto& se = xx::gSpineEnv;
	for (auto i = 0; i < spines.N; ++i) {
		auto fn = xx::ToString("res/miner", i + 1);
		se.Load(fn, spines.skels[i], spines.texs[i]);
		spines.anims_attack[i] = spines.skels[i]->findAnimation("attack");
		spines.anims_idle[i] = spines.skels[i]->findAnimation("idle");
	}

	// todo: convert spines to frames & event handle

	// todo: combine all.tex & spines.tex


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
	Quad().DrawFrame(all.mouse_pointer, mousePos, 3.f);
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
