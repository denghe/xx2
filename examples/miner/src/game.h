#pragma once
#include "pch.h"
using XY = xx::XY;
using XYi = xx::XYi;

struct MonsterConfig {
	float animFPS, resRadius, moveSpeed, attackRange;
	XY aps[3];	// anchor points for idle, move, atk
	int32_t tfsLens[3]; // aIdleLen, aMoveLen, aAtkLen;
	xx::TinyFrame* tfss[3];	// aIdle, * aMove, * aAtk;
	xx::FromTo<XY>* cd;	// for atk. share aAtkLen
};

struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };

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

	struct {
		xx::TinyFrame circle256;
		xx::TinyFrame pickaxe;
		std::array<xx::TinyFrame, 8> explosion_1_;
		std::array<xx::TinyFrame, 7> airplane_;
		std::array<std::array<xx::TinyFrame, 6>, 9> rocks_;	// rock_?_?

		std::array<xx::TinyFrame, 8> monster1_idle_;
		std::array<xx::TinyFrame, 11> monster1_move_;
		std::array<xx::TinyFrame, 6> monster1_atk_;

		std::array<xx::TinyFrame, 63> monster2_idle_;
		std::array<xx::TinyFrame, 90> monster2_move_;
		std::array<xx::TinyFrame, 56> monster2_atk_;

		std::array<xx::TinyFrame, 30> monster3_idle_;
		std::array<xx::TinyFrame, 4> monster3_move_;
		std::array<xx::TinyFrame, 10> monster3_atk_;
	} tf;

	struct {
		std::array<xx::FromTo<XY>, 6> monster1_atk_{};

		std::array<xx::FromTo<XY>, 56> monster2_atk_{};

		std::array<xx::FromTo<XY>, 10> monster3_atk_{};
	} cd;	// collision detect( attack )

	std::array<MonsterConfig, 3> mcs;

	struct {
		xx::Shared<xx::SoundSource> pickaxe, rockbreak, pop;
	} ss;

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
};
extern Game gg;
