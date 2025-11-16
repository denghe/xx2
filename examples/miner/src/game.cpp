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

	// load pngs
	tf.circle256 = LoadTexture("res/circle256.png");
	tf.pickaxe = LoadTexture("res/pickaxe.png");
	for (size_t i = 0; i < tf.explosion_1_.size(); i++) {
		tf.explosion_1_[i] = LoadTexture(xx::ToStringFormat("res/explosion_1_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.airplane_.size(); i++) {
		tf.airplane_[i] = LoadTexture(xx::ToStringFormat("res/airplane_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.rocks_.size(); i++) {
		auto& sub = tf.rocks_[i];
		for (size_t j = 0; j < sub.size(); j++) {
			sub[j] = LoadTexture(xx::ToStringFormat("res/rock_{0}_{1}.png", i, j));
		}
	}
	//// monster1
	for (size_t i = 0; i < tf.monster1_idle_.size(); i++) {
		tf.monster1_idle_[i] = LoadTexture(xx::ToStringFormat("res/monster1_idle_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.monster1_move_.size(); i++) {
		tf.monster1_move_[i] = LoadTexture(xx::ToStringFormat("res/monster1_move_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.monster1_atk_.size(); i++) {
		tf.monster1_atk_[i] = LoadTexture(xx::ToStringFormat("res/monster1_atk_{0}.png", i + 1));
	}
	//// monster2
	for (size_t i = 0; i < tf.monster2_idle_.size(); i++) {
		tf.monster2_idle_[i] = LoadTexture(xx::ToStringFormat("res/monster2_idle_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.monster2_move_.size(); i++) {
		tf.monster2_move_[i] = LoadTexture(xx::ToStringFormat("res/monster2_move_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.monster2_atk_.size(); i++) {
		tf.monster2_atk_[i] = LoadTexture(xx::ToStringFormat("res/monster2_atk_{0}.png", i + 1));
	}
	//// monster3
	for (size_t i = 0; i < tf.monster3_idle_.size(); i++) {
		tf.monster3_idle_[i] = LoadTexture(xx::ToStringFormat("res/monster3_idle_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.monster3_move_.size(); i++) {
		tf.monster3_move_[i] = LoadTexture(xx::ToStringFormat("res/monster3_move_{0}.png", i + 1));
	}
	for (size_t i = 0; i < tf.monster3_atk_.size(); i++) {
		tf.monster3_atk_[i] = LoadTexture(xx::ToStringFormat("res/monster3_atk_{0}.png", i + 1));
	}

	// combine pngs into single texture
	{
		xx::RectPacker rp;
		for (int32_t i = 0; i < sizeof(tf) / sizeof(xx::TinyFrame); ++i) {
			rp.tfs.Add((xx::TinyFrame*)&tf + i);
		}
		rp.AutoPack();
	}

	// fill collision detect data( left-top:0,0 )
	cd.monster1_atk_[3] = { {80, 24}, {96, 39} };

	cd.monster2_atk_[13] = { {50, 57}, {67, 73} };

	cd.monster3_atk_[4] = { {157, 49}, {212, 77} };

	// fill monster configs
	{
		auto& mc = mcs[0];
		mc.animFPS = 12;
		mc.resRadius = 23;
		mc.moveSpeed = 20 / gg.cFps;
		mc.attackRange = 50;
		mc.aps[0] = { 0.5f, 0 };
		mc.aps[1] = { 0.5f, 0 };
		mc.aps[2] = { 0.5f, 0 };
		mc.tfss[0] = tf.monster1_idle_.data();
		mc.tfsLens[0] = tf.monster1_idle_.size();
		mc.tfss[1] = tf.monster1_move_.data();
		mc.tfsLens[1] = tf.monster1_move_.size();
		mc.tfss[2] = tf.monster1_atk_.data();
		mc.tfsLens[2] = tf.monster1_atk_.size();
		mc.cd = cd.monster1_atk_.data();
	}
	{
		auto& mc = mcs[1];
		mc.animFPS = 30;
		mc.resRadius = 40;
		mc.moveSpeed = 10 / gg.cFps;
		mc.attackRange = 26;
		mc.aps[0] = { 0.5f, 0 };
		mc.aps[1] = { 0.5f, 0 };
		mc.aps[2] = { 0.5f, 0 };
		mc.tfss[0] = tf.monster2_idle_.data();
		mc.tfsLens[0] = tf.monster2_idle_.size();
		mc.tfss[1] = tf.monster2_move_.data();
		mc.tfsLens[1] = tf.monster2_move_.size();
		mc.tfss[2] = tf.monster2_atk_.data();
		mc.tfsLens[2] = tf.monster2_atk_.size();
		mc.cd = cd.monster2_atk_.data();
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
		mc.tfss[0] = tf.monster3_idle_.data();
		mc.tfsLens[0] = tf.monster3_idle_.size();
		mc.tfss[1] = tf.monster3_move_.data();
		mc.tfsLens[1] = tf.monster3_move_.size();
		mc.tfss[2] = tf.monster3_atk_.data();
		mc.tfsLens[2] = tf.monster3_atk_.size();
		mc.cd = cd.monster3_atk_.data();
	}

	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();
}

// game loop
void Game::Update() {
	scene->Update();
	scene->Draw();
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

void Game::Stat() {
#if 1
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " uiAutoUpdates.len = ", uiAutoUpdates.len
		, " delayUpdates.len = ", delayUpdates.len
	);
#endif
}
