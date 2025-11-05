#pragma once
#include "pch.h"
using XY = xx::XY;

struct Monster;
struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };

	struct {
		xx::Shared<xx::GLTexture> heart;
	} res;

	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	xx::Shared<Monster> heart;

	void Init();
	void GLInit();
	void Update();
	void Delay();
	void Stat();
	void OnResize(bool modeChanged_);
};
extern Game gg;
