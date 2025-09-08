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
			heart,
			gear
			;
		xx::List<xx::Ref<xx::GLTexture>> ui_flags_;
	} res;

	// config / runtime vars
	i18n::Lang lang;
	bool langSelected{};	// avoid always popup first language choose panel
	bool mute{};
	float audioVolume{ 1.f }, musicVolume{ 0.5f };
	int32_t resolutionsIndex{ 2 };
	// ...
	// todo: save & load config
	xx::Rnd rnd;
	// ...

	void Init();
	xx::Task<> Task();
	void Delay();
	void Stat();
	void OnResize();
};
extern Game gg;
