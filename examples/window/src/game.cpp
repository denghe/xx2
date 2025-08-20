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
	auto tex = LoadTexture("res/1.png");
	auto rect = tex->Rect();
	while (true) {
		float s = 20.f;
		for (; s > 15.f; s -= delta * 20.f) {
			Quad().Draw(*tex, rect, XY{}, 0.5f, s * scale, 0, 1, xx::RGBA8_Red);
			co_yield 0;
		}
		for (; s < 20.f; s += delta * 20.f) {
			Quad().Draw(*tex, rect, XY{}, 0.5f, s * scale, 0, 1, xx::RGBA8_Red);
			co_yield 0;
		}
	}
}

void Game::Update() {
	DrawNode(lbl);
}

void Game::OnResize() {
	lbl->Resize(p5, 2 * scale);
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = ", drawCall, " drawVerts = ", drawVerts);
}
