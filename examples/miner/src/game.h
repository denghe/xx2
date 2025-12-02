#pragma once
#include "pch.h"
using XY = xx::XY;
using XYi = xx::XYi;

struct MonsterConfig {
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
		xx::Frame circle256, pickaxe, bg1, mouse_pointer;
		std::array<xx::Frame, 8> explosion_1_;
		std::array<xx::Frame, 7> airplane_;
		std::array<xx::Frame, 3> talent_;
		std::array<xx::Frame, 2> minecart_;
		std::array<std::array<xx::Frame, 6>, 9> rocks_;	// rock_?_?

		std::array<xx::Frame, 8> monster1_idle_;
		std::array<xx::Frame, 11> monster1_move_;
		std::array<xx::Frame, 6> monster1_atk_;

		std::array<xx::Frame, 32> monster2_idle_;
		std::array<xx::Frame, 45> monster2_move_;
		std::array<xx::Frame, 28> monster2_atk_;

		std::array<xx::Frame, 30> monster3_idle_;
		std::array<xx::Frame, 4> monster3_move_;
		std::array<xx::Frame, 10> monster3_atk_;

		std::array<xx::Frame, 6> monster4_idle_;
		std::array<xx::Frame, 5> monster4_move_;
		std::array<xx::Frame, 7> monster4_atk_;
	} fs;

	struct {
		std::array<char, 6> monster1_atk_{};

		std::array<char, 28> monster2_atk_{};

		std::array<char, 10> monster3_atk_{};

		std::array<char, 6> monster4_atk_{};
	} cd;	// collision detect

	std::array<MonsterConfig, 4> mcs;

	struct {
		xx::Shared<xx::SoundSource>
			pickaxe, rockbreak, pop
			, monster1_atk
			, monster2_atk
			, monster3_atk
			, monster4_atk;
	} ss;

	struct {
		xx::STBImage bg1a;
		xx::STBImage minecart_3;
	} stbi;

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
