#pragma once
#include "pch.h"
#include "_i18n.h"
#include "sav.h"
using namespace i18n;
using XY = xx::XY;

struct Game : xx::Game<Game>, Sav {
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

	// runtime vars
	i18n::Lang lang;
	xx::Rnd rnd;
	// ...

	void Init();
	void GLInit();
	xx::Task<> Task();
	void Delay();
	void Stat();
	void OnResize();
};
extern Game gg;
