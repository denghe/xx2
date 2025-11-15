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

	// combine pngs into single texture
	{
		xx::RectPacker rp;
		for (int32_t i = 0; i < sizeof(tf) / sizeof(xx::TinyFrame); ++i) {
			rp.tfs.Add((xx::TinyFrame*)&tf + i);
		}
		rp.AutoPack();
	}

	//// fill anchor point data
	//for (size_t i = 0; i < tf.monster1_idle_.size(); i++) {
	//	ap.monster1_idle_[i] = { 0.5f, 0.f };
	//}
	//for (size_t i = 0; i < tf.monster1_move_.size(); i++) {
	//	ap.monster1_move_[i] = { 0.5f, 0.f };
	//}
	//for (size_t i = 0; i < tf.monster1_atk_.size(); i++) {
	//	ap.monster1_atk_[i] = { 0.5f, 0.f };
	//}

	// fill collision detect data( left-top:0,0 )
	cd.monster1_atk_[3] = { {80, 24}, {96, 39} };

	cd.monster2_atk_[13] = { {50, 57}, {67, 73} };


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
