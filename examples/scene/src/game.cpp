#include "pch.h"
#include "game.h"
#include "scene_mainmenu.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = "examples_scene";
}

xx::Task<> Game::Task() {
	// load res
	res.heart = LoadTexture("res/heart.png");
	res.gear = LoadTexture("res/gear.png");
	res.ui_flags_.Emplace(LoadTexture("res/ui_flags_0.png"));
	res.ui_flags_.Emplace(LoadTexture("res/ui_flags_1.png"));

	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();

	// game loop
	while (true) {
		scene->Update();
		scene->Draw();
		if (oldScene) oldScene.Reset();
		co_yield 0;
	}
}

void Game::Delay() {
#if 0
	// for power saving, fps limit
	SleepSecs(cDelta - (glfwGetTime() - time));	
#endif
}

void Game::OnResize() {
	if (scene) {
		scene->OnResize();
	}
}

void Game::Stat() {
#if 1
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " delayFuncs.len = ", delayFuncs.len
	);
#endif
}
