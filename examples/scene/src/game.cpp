#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = U"examples_scene";
}

void Game::GLInit() {
	// init shaders
}

xx::Task<> Game::Task() {
	// load res
	res.tex = LoadTexture("res/1.png");
	// ...

	// init first scene
	scene.Emplace<Scene_1>()->Init();

	// game loop
	while (true) {
		scene->Update();
		scene->Draw();
		if (oldScene) oldScene.Reset();
		co_yield 0;
	}
}

void Game::Delay() {
	for (auto d = cFrameDelay - (float)xx::NowSteadyEpochSeconds(time); d > 0.005; d -= 0.005) {
		Sleep(3);	// for power saving
	}
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = ", drawCall, " drawVerts = ", drawVerts);
}

void Game::OnResize() {
	if (scene) {
		scene->ResizeCalc();
		scene->OnResize();
	}
}

/**************************************************************************************/

void Scene_1::Init() {
	ResizeCalc();
	auto r = gg.designSize.y / 100.f;
	auto r_2 = r * 0.5f;
	for (int i = -50; i < 50; ++i) {
		for (int j = -50; j < 50; ++j) {
			monsters.Emplace().Emplace<Monster>()->Init({ i * r + r_2, j * r + r_2 }, r);
		}
	}
	monsters.Emplace().Emplace<Monster>()->Init({ -600, 0 }, 128);
	monsters.Emplace().Emplace<Monster>()->Init({ 600, 0 }, 128);
}

void Scene_1::Update() {
	auto d = float(gg.delta * timeScale);
	time += d;
	timePool += d;
	while (timePool >= gg.cDelta) {
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene_1::FixedUpdate() {
	for (auto& m : monsters) {
		m->Update();
	}
}

void Scene_1::Draw() {
	for (auto& m : monsters) {
		m->Draw();
	}
}

void Scene_1::OnResize() {
	// sync ui? cam?
}

/**************************************************************************************/


void Monster::Init(XY pos_, float radius_) {
	pos = pos_;
	radius = radius_;
	baseScale = radius / gg.res.tex->Width();
	_1scale = cAnimScaleRange.from;
}

void Monster::Update() {
	XX_BEGIN(_1);
	while (true) {
		for (; _1scale < cAnimScaleRange.to; _1scale += cAnimStepDelay) {
			XX_YIELD(_1);
		}
		for (; _1scale > cAnimScaleRange.from; _1scale -= cAnimStepDelay) {
			XX_YIELD(_1);
		}
	}
	XX_END(_1);
}

void Monster::Draw() {
	auto& t = *gg.res.tex;
	auto ss = gg.scene->scale;
	auto p = pos * ss;
	auto s = _1scale * baseScale * ss;
	gg.Quad().Draw(t, t.Rect(), p, 0.5f, s);
}
