#pragma once
#include "pch.h"
#include "xx_scenebase.h"

struct Res {
	// global res list
	xx::RefGLTexture tex;
	// ...
};


using XY = xx::XY;
struct Scene;
struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cFrameDelay{ 1.f / cFps };
	static constexpr float cMaxFrameDelay{ 0.1f };
	static constexpr float cDelta{ cFrameDelay };
	xx::Shared<Scene> scene, oldScene;	// oldScene for delay remove
	Res res;

	void Init();
	void GLInit();
	xx::Task<> Task();
	void Delay();
	void Stat();
	void OnResize();
};
extern Game gg;


struct Monster;
struct Scene : xx::SceneBase {
	float time{}, timePool{}, timeScale{ 1 };
	xx::List<xx::Shared<Monster>> monsters;
};


struct Monster {
	static constexpr xx::FromTo<float> cAnimScaleRange{ .5f, 1.f };
	static constexpr float cAnimDuration{ 0.5f };
	static constexpr float cAnimStepDelay{ cAnimScaleRange.Sub() / (Game::cFps * cAnimDuration) };
	XY pos{};
	float radius{}, baseScale{};
	int32_t _1{};	// for coroutine logic
	float _1scale{};
	void Init(XY pos_, float radius_);
	void Update();
	void Draw();
};


struct Scene_1 : Scene {
	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize() override;
};
