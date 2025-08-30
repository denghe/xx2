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
	res.ui_button = LoadTexture("res/ui_button.png");
	res.ui_button_h = LoadTexture("res/ui_button_h.png");
	res.ui_button_n = LoadTexture("res/ui_button_n.png");
	res.ui_checkbox_0 = LoadTexture("res/ui_checkbox_0.png");
	res.ui_checkbox_1 = LoadTexture("res/ui_checkbox_1.png");
	res.ui_imgbtn_h = LoadTexture("res/ui_imgbtn_h.png");
	res.ui_imgbtn_n = LoadTexture("res/ui_imgbtn_n.png");
	res.ui_dropdownlist_icon = LoadTexture("res/ui_dropdownlist_icon.png");
	res.ui_dropdownlist_head = LoadTexture("res/ui_dropdownlist_head.png");
	res.ui_panel = LoadTexture("res/ui_panel.png");
	res.ui_slider_bar = LoadTexture("res/ui_slider_bar.png");
	res.ui_slider_block = LoadTexture("res/ui_slider_block.png");

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

	cfg.s9bg.Emplace(*cfg.s9bN);
	cfg.s9bg->frame = res.ui_panel;

	cfg.sbar.Emplace(*cfg.s9bN);
	cfg.sbar->frame = res.ui_slider_bar;

	cfg.sblock.Emplace(*cfg.s9bN);
	cfg.sblock->frame = res.ui_slider_block;

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
	auto d = cDelta - (float)xx::NowSteadyEpochSeconds(time);
	for (; d > 0.005f; d -= 0.005f) Sleep(3);
}

void Game::OnResize() {
	if (scene) {
		scene->OnResize();
	}
}

void Game::Stat() {
	//xx::CoutN("drawFPS = ", drawFPS, " drawCall = ", drawCall, " drawVerts = ", drawVerts);
}
