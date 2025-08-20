#pragma once
#include "pch.h"

using XY = xx::XY;
struct Game : xx::Game<Game> {
	void Init();
	xx::Task<> Task();
	void Stat();
};
extern Game gg;
