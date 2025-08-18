#pragma once
#include "pch.h"
using XY = xx::XY;
struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cFrameDelay{ 1.f / cFps };
	static constexpr float cMaxFrameDelay{ 0.1f };

	// more cfg here
	void Init();
	xx::Task<> Task();
	void Update();
	void Delay();
	void Stat();
};
extern Game gg;
