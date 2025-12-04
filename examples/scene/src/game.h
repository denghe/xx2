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
		xx::Frame
			damage_numbers,
			hpbar;
	} res;

	struct {
		xx::Frame
			blade,
			monster,
			player,
			cell_bg,
			cell_bg1,
			cell_bg2,

			helm_1,
			amulet_1,
			ring_1,
			ring_2,
			ring_3,
			ring_4,
			armor_1,
			armor_2,
			armor_3,
			weapon1_1,
			weapon1_2,
			weapon1_3,
			weapon2_1,
			boots_1,
			currency_2,
			currency_1,
			material_1,
			consumable_1,

			bug_body,
			bug_head
		;

		std::array<xx::Frame, 7> explosion_;
	} fs;

	struct {
		xx::Shared<xx::SoundSource>
			explosion
		;
	} ss;

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
