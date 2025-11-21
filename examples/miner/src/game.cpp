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
	// init basedata
	sgrdd.Init(64, 64);

	// init shader
	shaderGrass.Init();

	// begin load res
	// load wav ogg
	ss.pickaxe = LoadSoundSource("res/pickaxe.wav");
	ss.rockbreak = LoadSoundSource("res/rockbreak.ogg");
	ss.pop = LoadSoundSource("res/pop.wav");
	ss.monster1_atk = LoadSoundSource("res/monster1_atk.ogg");
	ss.monster2_atk = LoadSoundSource("res/monster2_atk.ogg");
	ss.monster3_atk = LoadSoundSource("res/monster3_atk.ogg");
	ss.monster4_atk = LoadSoundSource("res/monster4_atk.ogg");

	// load pngs
	tf.circle256 = LoadTexture("res/circle256.png");
	tf.pickaxe = LoadTexture("res/pickaxe.png");
	tf.bg1 = LoadTexture("res/bg1.png");
	for (size_t i = 0; i < tf.explosion_1_.size(); i++) {
		tf.explosion_1_[i] = LoadTexture(xx::ToStringFormat("res/explosion_1_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.airplane_.size(); i++) {
		tf.airplane_[i] = LoadTexture(xx::ToStringFormat("res/airplane_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.minecart_.size(); i++) {
		tf.minecart_[i] = LoadTexture(xx::ToStringFormat("res/minecart_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.rocks_.size(); i++) {
		auto& sub = tf.rocks_[i];
		for (size_t j = 0; j < sub.size(); j++) {
			sub[j] = LoadTexture(xx::ToStringFormat("res/rock_{0}_{1}.png", i, j));
		}
	}

	// load monster pngs & some config
	auto LoadMonsterTFs = [this](auto& tfIdle, auto& tfMove, auto& tfAtk, auto const& pathPrefix, int32_t configIndex) {
		for (size_t i = 0; i < tfIdle.size(); i++) {
			tfIdle[i] = LoadTexture(xx::ToStringFormat("{0}_idle_{1}.png", pathPrefix, i + 1));
		}
		for (size_t i = 0; i < tfMove.size(); i++) {
			tfMove[i] = LoadTexture(xx::ToStringFormat("{0}_move_{1}.png", pathPrefix, i + 1));
		}
		for (size_t i = 0; i < tfAtk.size(); i++) {
			tfAtk[i] = LoadTexture(xx::ToStringFormat("{0}_atk_{1}.png", pathPrefix, i + 1));
		}
		auto& mc = mcs[configIndex];
		mc.tfss[0] = tfIdle.data();
		mc.tfsLens[0] = tfIdle.size();
		mc.tfss[1] = tfMove.data();
		mc.tfsLens[1] = tfMove.size();
		mc.tfss[2] = tfAtk.data();
		mc.tfsLens[2] = tfAtk.size();
	};
	LoadMonsterTFs(tf.monster1_idle_, tf.monster1_move_, tf.monster1_atk_, "res/monster1", 0);
	LoadMonsterTFs(tf.monster2_idle_, tf.monster2_move_, tf.monster2_atk_, "res/monster2", 1);
	LoadMonsterTFs(tf.monster3_idle_, tf.monster3_move_, tf.monster3_atk_, "res/monster3", 2);
	LoadMonsterTFs(tf.monster4_idle_, tf.monster4_move_, tf.monster4_atk_, "res/monster4", 3);

	// combine pngs into single texture
	{
		xx::RectPacker rp;
		for (int32_t i = 0; i < sizeof(tf) / sizeof(xx::TinyFrame); ++i) {
			rp.tfs.Add((xx::TinyFrame*)&tf + i);
		}
		rp.AutoPack();
	}

	// fill collision detect data( left-top:0,0 )
	cd.monster1_atk_[3] = 1;
	cd.monster2_atk_[7] = 1;
	cd.monster3_atk_[4] = 1;
	cd.monster4_atk_[3] = 1;

	// fill monster configs
	{
		auto& mc = mcs[0];
		mc.animFPS = 12;
		mc.resRadius = 23;
		mc.moveSpeed = 30 / gg.cFps;
		mc.attackRange = 50;
		mc.aps[0] = { 0.5f, 0 };
		mc.aps[1] = { 0.5f, 0 };
		mc.aps[2] = { 0.5f, 0 };
		mc.cd = cd.monster1_atk_.data();
		mc.ss = ss.monster1_atk;
	}
	{
		auto& mc = mcs[1];
		mc.animFPS = 30;
		mc.resRadius = 40;
		mc.moveSpeed = 20 / gg.cFps;
		mc.attackRange = 26;
		mc.aps[0] = { 0.5f, 0 };
		mc.aps[1] = { 0.5f, 0 };
		mc.aps[2] = { 0.5f, 0 };
		mc.cd = cd.monster2_atk_.data();
		mc.ss = ss.monster2_atk;
	}
	{
		auto& mc = mcs[2];
		mc.animFPS = 30;
		mc.resRadius = 50;
		mc.moveSpeed = 30 / gg.cFps;
		mc.attackRange = 44;
		mc.aps[0] = { 0.5f, 1.f - 86.f / 95 };
		mc.aps[1] = { 0.5f, 1.f - 86.f / 95 };
		mc.aps[2] = { 74.f / 217, 1.f - 90.f / 108 };
		mc.cd = cd.monster3_atk_.data();
		mc.ss = ss.monster3_atk;
	}
	{
		auto& mc = mcs[3];
		mc.animFPS = 5;
		mc.resRadius = 40;
		mc.moveSpeed = 50 / gg.cFps;
		mc.attackRange = 32;
		mc.aps[0] = { 0.5f, 0.f };
		mc.aps[1] = { 0.5f, 0.f };
		mc.aps[2] = { 0.5f, 0.f };
		mc.cd = cd.monster4_atk_.data();
		mc.ss = ss.monster4_atk;
	}

	// load file data
	stbi.bg1a.Fill(LoadFileData("res/bg1a.png"));
	stbi.minecart_3.Fill(LoadFileData("res/minecart_3.png"));

	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();
}

// game loop
void Game::Update() {
	scene->Update();
	if (!minimized) {
		scene->Draw();
	}
	if (oldScene) oldScene.Reset();
}

void Game::Delay() {
#if 0
	// for power saving, fps limit
	SleepSecs(cDelta - (glfwGetTime() - time));	
#endif
}

void Game::OnResize(bool modeChanged_) {
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
#if 1
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " uiAutoUpdates.len = ", uiAutoUpdates.len
		, " delayUpdates.len = ", delayUpdates.len
	);
#endif
}
