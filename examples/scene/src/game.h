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
		xx::TinyFrame
			blade,
			damage_numbers,
			hpbar,
			monster,
			player
		;

		xx::List<xx::TinyFrame>
			explosion_
		;

		xx::Ref<xx::SoundSource>
			explosion
		;

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
	void OnResize(bool modeChanged_);
};
extern Game gg;
