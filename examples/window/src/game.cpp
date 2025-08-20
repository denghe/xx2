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

xx::Task<> Game::Task() {
	lbl.Emplace()->Init(1, p5, 2 * scale, a5, xx::RGBA8_White, "hi");
	tex = LoadTexture("res/1.png");
	while (true) {
		heartScale = 20.f;
		for (; heartScale > 15.f; heartScale -= delta * 20.f) {
			co_yield 0;
		}
		for (; heartScale < 20.f; heartScale += delta * 20.f) {
			co_yield 0;
		}
	}
}

void Game::Update() {
	Quad().Draw(*tex, tex->Rect(), XY{}, 0.5f, heartScale * scale, 0, 1, xx::RGBA8_Red);
	DrawNode(lbl);
}

void Game::OnResize() {
	lbl->Resize(p5, 2 * scale);
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = ", drawCall, " drawVerts = ", drawVerts);
}
