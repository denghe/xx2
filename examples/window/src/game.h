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
		xx::Frame tiled9;	// pixel: 32*3 / 32*3
	} pics;
	struct {
		xx::Shared<SoLoud::Wav> bgm1;
		xx::Shared<SoLoud::Wav> heart_jump;
	} wavs;

	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	xx::Shared<Monster> heart;

	xx::Shared<xx::GLTiledTexture> tiledData;
	xx::Rnd rnd;
	XYi gridSize{ designSize.x / 32 * 2, designSize.y / 32 * 2 };
	XY mapSize{ gridSize.x * 32.f, gridSize.y * 32.f };
	XYi tiledTexSize{ (gridSize.x + 7) & ~7u, (gridSize.y + 7) & ~7u };

	XY bgOffset{};
	bool bgChanged{};

	float t{};	// for sound play

	void Init() override;
	void GLInit() override;
	void Update() override;
	void Delay() override;
	void Stat() override;
	void OnResize(bool modeChanged_) override;
};
extern Game gg;
