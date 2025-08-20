#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = U"examples_window";
	windowSize = { 640, 480 };
}

xx::Task<> Game::Task() {
	auto tex = LoadTexture("res/1.png");
	auto rect = tex->Rect();
	while (true) {
		float s = 30.f;
		for (; s > 15.f; s -= delta * 20.f) {
			Quad().Draw(tex, rect, XY{}, 0.5f, s * scale);
			co_yield 0;
		}
		for (; s < 30.f; s += delta * 20.f) {
			Quad().Draw(tex, rect, XY{}, 0.5f, s * scale);
			co_yield 0;
		}
	}
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = ", drawCall, " drawVerts = ", drawVerts);
}
