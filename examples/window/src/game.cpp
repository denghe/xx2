#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = U"examples_window";
	windowSize = { 350, 260 };
	showStat = true;
}

xx::Task<> Game::Task() {
	auto tex = LoadTexture("res/1.png");
	auto rect = tex->Rect();
	while (true) {
		for (float scale = 10.f; scale > 5.f; scale -= delta * 10.f) {
			Quad().Draw(tex, rect, XY{}, 0.5f, scale);
			co_yield 0;
		}
		for (float scale = 5.f; scale < 10.f; scale += delta * 10.f) {
			Quad().Draw(tex, rect, XY{}, 0.5f, scale);
			co_yield 0;
		}
	}
}
