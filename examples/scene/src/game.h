#pragma once
#include "pch.h"
#include "_i18n.h"
using namespace i18n;
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
		xx::Ref<xx::GLTexture>
			heart;
	} res;

	// config / runtime vars
	i18n::Lang lang;
	bool langSelected{};
	float soundVolume{ 1.f }, musicVolume{ 0.5f };
	// ...

	// todo: save & load config

	void Init();
	xx::Task<> Task();
	void Delay();
	void Stat();
	void OnResize();
};
extern Game gg;
