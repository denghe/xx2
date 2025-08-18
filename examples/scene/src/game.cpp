#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = U"examples_scene";
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

void Game::Update() {
	// ...
}

void Game::Delay() {
	// for power saving
	for (auto d = cFrameDelay - (float)xx::NowSteadyEpochSeconds(time); d > 0.005; d -= 0.005) {
		Sleep(3);
	}
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = ", drawCall, " drawVerts = ", drawVerts);
}
