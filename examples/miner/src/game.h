#pragma once
#include "pch.h"
using XY = xx::XY;
using XYi = xx::XYi;

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
		xx::TinyFrame circle256;
		std::array<xx::TinyFrame, 8> explosion_1_;
		std::array<xx::TinyFrame, 1> pickaxe_;
		std::array<xx::TinyFrame, 2> rock_;	// base rock: a, b
		std::array<std::array<xx::TinyFrame, 6>, 8> rocks_;	// rock_?_?
	} res;

	struct {
		xx::Shared<xx::SoundSource> pickaxe, rockbreak;
	} ss;

	// runtime vars
	xx::Rnd rnd;
	xx::SpaceGridRingDiffuseData sgrdd;
	// ...

	void Init();
	void GLInit();
	void Update();
	void Delay();
	void Stat();
	void OnResize(bool modeChanged_);
};
extern Game gg;
