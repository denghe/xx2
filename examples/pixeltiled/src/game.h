#pragma once
#include "pch.h"

enum class BlockTypes : int32_t {
	Space,
	Static,
	Breakable,
};

struct Block {
	BlockTypes type{};
};

struct Map {
	static constexpr XY cellSize{ 32, 32 };
	std::unique_ptr<Block[]> mapData;
	std::unique_ptr<std::array<uint8_t, 2>[]> texData;
	xx::Shared<xx::GLTiledTexture> tex;	// load from data
	XYi mapSize{};
	XY pixelSize{};
	XY beginPos{};
	float minScale{};
	int32_t whitePixelCount{};
	int32_t breakPixelCount{};
	xx::List<XYi> neighbors, bak;
	void Init();
	bool CheckBreakable3x3(XYi cri_);	// return true mean all 3x3 blocks are breakable
	void TryDig1x1(XYi cri_);
	void Dig3x3(XYi cri_);
	void DigAll();
	void LightLeighbors(uint8_t c_);
	void TryAddNeighbor(int32_t nx_, int32_t ny_, uint8_t c_);
	void TryAdd8Neighbors(int32_t nx_, int32_t ny_, uint8_t c_);
	void FillTex();
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
