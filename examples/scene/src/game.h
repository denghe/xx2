#pragma once
#include "pch.h"
using XY = xx::XY;

struct Scene;
struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cFrameDelay{ 1.f / cFps };
	static constexpr float cMaxFrameDelay{ 0.1f };
	static constexpr float cDelta{ cFrameDelay };

	xx::Shared<Scene> scene, oldScene;	// oldScene: delay remove( after draw )

	struct {
		xx::Ref<xx::Scale9Config> s9bN, s9bH;
	} cfg;

	struct {
		xx::Ref<xx::GLTexture> heart, ui_button_n, ui_button_h;
	} res;

	xx::Shared<xx::Node> ui;

	void Init();
	xx::Task<> Task();
	void Delay();
	void Stat();
	void OnResize();
};
extern Game gg;


struct Monster;
struct Scene : xx::SceneBase {
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };
	xx::List<xx::Shared<Monster>> monsters;
};
