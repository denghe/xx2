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
		auto q = ShaderBegin(shaderQuadInstance).Draw(tex, 1);
		q->pos = {};
		q->anchor = 0.5f;
		q->scale = 10;
		q->radians = 0;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect = tex->FullRect();
		co_yield 0;
	}
}
