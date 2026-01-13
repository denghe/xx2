#pragma once
#include "pch.h"
#include <xx_grid2d_circle.h>
#include "shader_grass.h"
#include "all.h"

struct MinerConfig {
	float animFPS, resRadius, moveSpeed, attackRange;
	int32_t fsLens[3]; // aIdleLen, aMoveLen, aAtkLen;
	xx::Frame* fss[3];	// aIdle, * aMove, * aAtk;
	char* cd;	// for atk. share aAtkLen
	xx::Weak<SoLoud::Wav> ss;
};

struct SpineFrameConfig {
	XY size{}, offset{};
	float scale{}, delta{};
};

struct Game : xx::GameBase {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };
	bool isShowInfo{ true }, isLimitFPS{ true };
	std::string fpsVal;
	xx::Shared<xx::Node> ui;
	xx::Weak<xx::Label> uiFPS;

	xx::Shared<xx::SceneBase> scene, oldScene;	// oldScene: delay remove( after draw )
	template<typename T>
	xx::Shared<T>& MakeScene() {
		oldScene = std::move(scene);
		scene = xx::MakeShared<T>();
		return (xx::Shared<T>&)scene;
	}

	// for bg
	xx::Shader_Grass shaderGrass;
	xx::Shader_Grass& Grass() { return ShaderBegin(shaderGrass); }

	all all;
	std::array<std::array<xx::Frame, sizeof(all::rock_0_) / sizeof(xx::Frame)>, 9>& all_rocks_();	// ref to all.rocks_?.  rock_?_?

	struct {
		std::array<char, 6> miner1_atk_{};
		std::array<char, 28> miner2_atk_{};
		std::array<char, 10> miner3_atk_{};
		std::array<char, 6> miner4_atk_{};
	} cd;	// collision detect

	std::array<MinerConfig, 4> mcs;

	struct {
		xx::Shared<SoLoud::Wav>
			pickaxe, rockbreak, pop
			, miner1_atk
			, miner2_atk
			, miner3_atk
			, miner4_atk;
	} ss;

	struct {
		xx::STBImage bg1a;
		xx::STBImage minecart_3;
	} stbi;

	struct {
		xx::List<xx::List<xx::Frame>> idles;	// grass
		xx::List<xx::List<xx::Frame>> attacks;	// creatures
		xx::List<xx::SpineEventData> eventDatas;
	} spines;

	// runtime vars
	xx::Rnd rnd;
	xx::SpaceGridRingDiffuseData sgrdd;
	// ...

	void Init() override;
	void GLInit() override;
	void Update() override;
	void Delay() override;
	void Stat() override;
	void OnResize(bool modeChanged_) override;
	void OnFocus(bool focused_) override;
};
extern Game gg;
