#pragma once
#include "pch.h"
using XY = xx::XY;

struct Game : xx::Game<Game> {
	xx::Shared<xx::Label> lbl;
	xx::RefGLTexture tex;
	float heartScale{};

	void Init();
	void GLInit();
	xx::Task<> Task();
	void Update();
	void Stat();
	void OnResize();
};
extern Game gg;
