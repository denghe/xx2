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
	res.circle256 = LoadTexture("res/circle256.png");
	res.pickaxe = LoadTexture("res/pickaxe.png");
	for (size_t i = 0; i < res.explosion_1_.size(); i++) {
		res.explosion_1_[i] = LoadTexture(xx::ToStringFormat("res/explosion_1_{0}.png", i + 1));
	}
	for (size_t i = 0; i < res.rocks_.size(); i++) {
		auto& sub = res.rocks_[i];
		for (size_t j = 0; j < sub.size(); j++) {
			sub[j] = LoadTexture(xx::ToStringFormat("res/rock_{0}_{1}.png", i, j));
		}
	}
	// combine pngs into single texture
	{
		xx::RectPacker rp;
		for (int32_t i = 0; i < sizeof(res) / sizeof(xx::TinyFrame); ++i) {
			rp.tfs.Add((xx::TinyFrame*)&res + i);
		}
		rp.AutoPack();
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
