#pragma once
#include "pch.h"
using XY = xx::XY;

struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };

	xx::Shared<xx::SceneBase> scene, oldScene;	// oldScene: delay remove( after draw )
	template<typename T>
	xx::Shared<T>& MakeScene() {
		oldScene = std::move(scene);
		scene = xx::MakeShared<T>();
		return (xx::Shared<T>&)scene;
	}

	struct {
		// todo
	} res;

	// runtime vars
	xx::Rnd rnd;
	// ...

	void Init();
	xx::Task<> Task();
	void Delay();
	void Stat();
	void OnResize(bool modeChanged_);
};
extern Game gg;
