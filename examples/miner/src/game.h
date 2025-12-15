#pragma once
#include "pch.h"
#include "all.h"
using XY = xx::XY;
using XYi = xx::XYi;

struct MinerConfig {
	float animFPS, resRadius, moveSpeed, attackRange;
	int32_t fsLens[3]; // aIdleLen, aMoveLen, aAtkLen;
	xx::Frame* fss[3];	// aIdle, * aMove, * aAtk;
	char* cd;	// for atk. share aAtkLen
	xx::Weak<xx::SoundSource> ss;
};

struct Game : xx::Game<Game> {
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
	XX_INLINE xx::Shader_Grass& Grass() { return ShaderBegin(shaderGrass); }

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
		xx::Shared<xx::SoundSource>
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
		static constexpr int32_t N{ 13 };
		std::array<xx::Shared<xx::GLTexture>, N> texs;
		std::array<spine::SkeletonData*, N> skels;
		std::array<spine::Animation*, N> anims_attack;
		std::array<spine::Animation*, N> anims_idle;
	} spines;

	// runtime vars
	xx::Rnd rnd;
	xx::SpaceGridRingDiffuseData sgrdd;
	// ...

	void Init();
	void GLInit();
	void Update();
	void Delay();
	void Stat();
	void OnResize(bool modeChanged_);
	void OnFocus(bool focused_);
};
extern Game gg;
