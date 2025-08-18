#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = U"examples_scene";
	showStat = true;
}

xx::Task<> Game::Task() {
	auto tex = LoadTexture("res/1.png");
	auto rect = tex->Rect();
	while (true) {
		auto& q = Quad();
		auto basePos = -windowSize / 2.f;
		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 100; ++j) {
				q.Draw(tex, rect, basePos + XY{ i * rect.h, j * rect.w }, 0);
			}
		}
		co_yield 0;
	}
}
