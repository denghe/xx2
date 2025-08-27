#pragma once
#include "pch.h"
using XY = xx::XY;

struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cFrameDelay{ 1.f / cFps };
	static constexpr float cMaxFrameDelay{ 0.1f };
	static constexpr float cDelta{ cFrameDelay };

	xx::Shared<xx::SceneBase> scene, oldScene;	// oldScene: delay remove( after draw )
	template<typename T>
	xx::Shared<T>& MakeScene() {
		oldScene = std::move(scene);
		scene = xx::MakeShared<T>();
		return (xx::Shared<T>&)scene;
	}

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
