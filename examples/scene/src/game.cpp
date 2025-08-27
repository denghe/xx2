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
	res.ui_button_n = LoadTexture("res/ui_button_n.png");
	res.ui_button_h = LoadTexture("res/ui_button_h.png");

	// init cfgs
	cfg.s9bN.Emplace(xx::Scale9Config{
		.frame = res.ui_button_n,
		.texScale = { 1, 1 },
		.center = { 2, 2, 2, 2 },
		.color = xx::RGBA8_White,
		.borderScale = { 4.f },
		.txtColor = xx::RGBA8_White,
		.txtPadding = { 20, 5 },
		.txtPaddingRightBottom = { 20, 10 },
		.txtScale = { 2 },
		.iconPadding = { 5 }
		});

	cfg.s9bH.Emplace(*cfg.s9bN);
	cfg.s9bH->frame = res.ui_button_h;

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
	// for power saving
	auto d = cFrameDelay - (float)xx::NowSteadyEpochSeconds(time);
	for (; d > 0.005f; d -= 0.005f) Sleep(3);
}

void Game::OnResize() {
	if (scene) {
		scene->OnResize();
	}
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = ", drawCall, " drawVerts = ", drawVerts);
}
