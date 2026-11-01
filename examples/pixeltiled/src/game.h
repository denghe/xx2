#pragma once
#include "pch.h"

struct Map {
	static constexpr XY cellSize{ 32, 32 };
	std::unique_ptr<std::array<uint8_t, 2>[]> data;
	xx::Shared<xx::GLTiledTexture> tex;	// load from data
	float drawScale{ 1.f };
	XY mapSize{};
	XY pixelSize{};
	XY beginPos{};
	int32_t whitePixelCount{};
	int32_t breakPixelCount{};
	void Init();
	void Draw();
};

struct Game : xx::GameBase {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };

	struct {
		xx::Frame tiled10;	// pixel: 32*10 * 32
	} pics;
	xx::Shared<Map> map;

	xx::Shared<xx::Node> ui;
	xx::Weak<xx::Label> fpsLabel;
	
	xx::Camera cam;

	xx::Rnd rnd;

	XY bgOffset{};
	bool bgChanged{};

	void Init() override;
	void GLInit() override;
	void Update() override;
	void Delay() override;
	void Stat() override;
	void OnResize(bool modeChanged_) override;

	bool HandleInputs();
	void Logic();
	void Draw();
};
extern Game gg;
