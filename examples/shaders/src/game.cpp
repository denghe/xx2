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
	title = "examples_shaders";
}

void Game::GLInit() {
	// init shaders
	shaderGrass.Init();
	shaderRadiateGoldenRays.Init();
	// ... init other shaders here

	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();
}

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
