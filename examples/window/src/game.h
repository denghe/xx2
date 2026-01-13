#pragma once
#include "pch.h"

struct Sprite {
	xx::Frame frame;
	XY pos{};
};

struct Monster;
struct Game : xx::GameBase {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };

	struct {
		std::array<xx::Frame, 3> imgs;
	} res;

	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	xx::Shared<Monster> heart;
	xx::Rnd rnd;
	xx::List<Sprite> sprites;

	void Init() override;
	void GLInit() override;
	void Update() override;
	void Delay() override;
	void Stat() override;
	void OnResize(bool modeChanged_) override;
};
extern Game gg;
