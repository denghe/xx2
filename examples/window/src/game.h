#pragma once
#include "pch.h"

struct Game : xx::Game<Game> {
	static constexpr float fps{ 120 }, frameDelay{ 1.f / fps }, maxFrameDelay{ 0.1f };
	void Init();
	xx::Task<> Task();
};
extern Game gg;
