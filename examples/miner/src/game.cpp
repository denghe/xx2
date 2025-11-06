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

	// begin load res
	// load wav ogg
	snds.pickaxe = LoadSoundSource("res/pickaxe.wav");
	snds.rockbreak = LoadSoundSource("res/rockbreak.ogg");

	// load pngs
	res.circle256 = LoadTexture("res/circle256.png");
	res.pickaxe_[0] = LoadTexture("res/pickaxe_0.png");
	res.rock_[0] = LoadTexture("res/rock_a.png");
	res.rock_[1] = LoadTexture("res/rock_b.png");
	for (size_t i = 0; i < res.rocks_.size(); i++) {
		auto& sub = res.rocks_[i];
		for (size_t j = 0; j < sub.size(); j++) {
			sub[j] = LoadTexture(xx::ToStringFormat("res/rock_{0}_{1}.png", i, j));
		}
	}
	// combine pngs into single texture
	{
		xx::RectPacker rp;
		rp.tfs.Add(&res.pickaxe_[0]);
		rp.tfs.Add(&res.rock_[0]);
		rp.tfs.Add(&res.rock_[1]);
		for (auto& sub : res.rocks_) {
			for (auto& tf : sub) {
				rp.tfs.Add(&tf);
			}
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
