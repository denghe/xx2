#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = U"examples_window";
	windowSize = designSize = { 800, 600 };
}

void Game::GLInit() {
	texs.heart = LoadTexture("res/heart.png");
	texs.ui_button_n = LoadTexture("res/ui_button_n.png");
	texs.ui_button_h = LoadTexture("res/ui_button_h.png");

	cfgs.btnNormal.Emplace(xx::Scale9SpriteConfig{
		.frame = texs.ui_button_n,
		.texScale = { 1, 1 },
		.center = { 2, 2, 2, 2 },
		.color = { 255, 255, 255, 255 },
		.borderScale = { 4.f },
		.txtColor = { xx::RGBA8_White },
		.txtPadding = { 20, 5 },
		.txtPaddingRightBottom = { 20, 10 },
		.txtScale = { 2 },
		.iconPadding = { 5 }
	});

	cfgs.btnHighlight.Emplace();
	*cfgs.btnHighlight = *cfgs.btnNormal;
	cfgs.btnHighlight->frame = texs.ui_button_h;

	ui.Emplace()->InitRoot(scale);

	ui->MakeChildren<xx::Label>()->Init(1, p5, a5, 2)
		.SetText("hi");

	ui->MakeChildren<xx::LabelButton>()->Init(2, p5 + XY{ 0, -50 }, a5
		, cfgs.btnNormal, cfgs.btnHighlight
		, "asdf");

	heartScale = 20.f;
}

xx::Task<> Game::Task() {
	while (true) {
		for (heartScale = 20.f; heartScale > 15.f; heartScale -= delta * 20.f) {
			co_yield 0;
		}
		for (heartScale = 15.f; heartScale < 20.f; heartScale += delta * 20.f) {
			co_yield 0;
		}
	}
}

void Game::Update() {
	Quad().Draw(*texs.heart, texs.heart->Rect(), XY{}, 0.5f
		, heartScale * scale, 0, 1, xx::RGBA8_Red);
	DrawNode(ui);
}

void Game::OnResize() {
	ui->Resize(scale);
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts);
}
