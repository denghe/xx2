#pragma once
#include "pch.h"

struct Game : xx::GameBase {
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

	// runtime vars
	xx::Rnd rnd;
	// ...

	void Init() override;
	void GLInit() override;
	void Update() override;
	void Delay() override;
	void Stat() override;
	void OnResize(bool modeChanged_) override;
};
extern Game gg;
