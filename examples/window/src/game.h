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
		xx::Frame heart;	// pixel: 29 * 24
	} pics;
	struct {
		xx::Shared<SoLoud::Wav> bgm1;
		xx::Shared<SoLoud::Wav> heart_jump;
	} wavs;

	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	xx::Shared<Monster> heart;
	XY bgOffset{};
	float t{};	// for sound play

	void Init() override;
	void GLInit() override;
	void Update() override;
	void Delay() override;
	void Stat() override;
	void OnResize(bool modeChanged_) override;
};
extern Game gg;
