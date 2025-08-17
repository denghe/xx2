#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	this->title = U"examples_window";
}

xx::Task<> Game::Task() {
	auto tex = LoadTexture("res/1.png");
	while (true) {
		ShaderBegin(shaderQuadInstance).Draw(tex, {}, {}, 0.5f, 10.f);
		co_yield 0;
	}
}
