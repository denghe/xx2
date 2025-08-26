#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = "examples_scene";
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
	// for power saving
	for (auto d = cFrameDelay - (float)xx::NowSteadyEpochSeconds(time); d > 0.005; d -= 0.005) Sleep(3);
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = ", drawCall, " drawVerts = ", drawVerts);
}

void Game::OnResize() {
	if (scene) {
		scene->OnResize();
	}
}

/**************************************************************************************/

void Scene_1::Init() {
	cam.Init(gg.scale, 1.f);

	int ny = 80;
	auto r = gg.designSize.y / ny;
	auto r_2 = r * 0.5f;
	auto nx = int(gg.designSize.x / r);

	auto x1 = -nx * 0.5f * r;
	auto x2 = nx * 0.5f * r;
	auto y1 = -ny * 0.5f * r;
	auto y2 = ny * 0.5f * r;

	for (auto y = y1; y < y2; y += r) {
		for (auto x = x1; x < x2; x += r) {
			monsters.Emplace().Emplace<Monster>()->Init(this, { x + r_2, y + r_2 }, r);
		}
	}
	monsters.Emplace().Emplace<Monster>()->Init(this, { -600, 0 }, 128);
	monsters.Emplace().Emplace<Monster>()->Init(this, { 600, 0 }, 128);
}

void Scene_1::Update() {
	auto d = float(std::min((float)gg.delta, gg.cMaxFrameDelay) * timeScale);
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
	cam.SetBaseScale(gg.scale);
}

/**************************************************************************************/


void Monster::Init(Scene* scene_, XY pos_, float radius_) {
	scene = scene_;
	pos = pos_;
	radius = radius_;
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
	auto& tex = *gg.res.tex;
	auto& cam = scene->cam;
	auto scale = _1scale * cam.scale * (radius / gg.res.tex->Width());
	gg.Quad().Draw(tex, tex.Rect(), cam.ToGLPos(pos), 0.5f, scale);
}
