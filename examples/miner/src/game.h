#pragma once
#include "pch.h"
using XY = xx::XY;
using XYi = xx::XYi;

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
	} tf;

	struct {
		std::array<XY, 8> monster1_idle_{};
		std::array<XY, 11> monster1_move_{};
		std::array<XY, 6> monster1_atk_{};
	} ap;	// anchor point

	struct {
		std::array<xx::FromTo<XY>, 6> monster1_atk_{};
	} cd;	// collision detect( attack )

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
