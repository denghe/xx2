#include "pch.h"
#include "game.h"
#include "scene_mainmenu.h"
Game gg;

int32_t main() {
	// enter game loop
	return gg.Run();
}

void Game::Init() {
	title = "examples_spine";
}

xx::Task<> Game::Task() {
	// begin load res
	// load spine res & parse
	auto& se = xx::gSpineEnv;

	se.Load("res/owl-pro", res.spineOwl_skel, res.spineOwl_tex);
	res.spineOwl_tex->GenerateMipmap();
	res.spineOwl_idle = res.spineOwl_skel->findAnimation("idle");
	res.spineOwl_blink = res.spineOwl_skel->findAnimation("blink");

	//se.Load("res/spineboy-ess", res_SpineBoy_skel, res_SpineBoy_tex);
	//res_SpineBoy_walk = res_SpineBoy_skel->findAnimation("walk");
	//res_SpineBoy_jump = res_SpineBoy_skel->findAnimation("jump");
	//res_SpineBoy_run = res_SpineBoy_skel->findAnimation("run");

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
