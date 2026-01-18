#pragma once
#include <xx_shader_numbers.h>
#include <xx_camera.h>
#include <xx_queue.h>

namespace xx {
	struct EffectText {
		constexpr static float cMoveDurationSeconds{ 0.5f };
		constexpr static float cMoveSpeedMin{ 50 / 120.f };
		constexpr static float cMoveSpeedMax{ 100 / 120.f };

		constexpr static float cFadeOutDurationSeconds{ 0.2f };
		constexpr static float cFadeOutStep = 1.f / (cFadeOutDurationSeconds * 120.f);

		Shader_NumbersData data;	// include pos, scale, color
		int32_t _1{};
		XY pos{}, inc{}, scale{};
		float timeout{};
		float a{}, alpha{};

		void Init(Rnd& rnd_, XY pos_, XY vec_, RGBA8 color_, XY scale_, int32_t value_, bool includeSignal_ = false);
		int32_t Update(float time_);
		void Draw(Shader_Numbers& shader_, Camera& cam_);
	};

	template<>
	struct IsPod<EffectText, void> : std::true_type {};

	struct EffectTextManager {
		Queue<EffectText> ens;
		Rnd rnd{};
		Camera* cam{};

		void Init(Camera* cam_, int32_t cap_);
		void Add(XY pos_, XY vec_, RGBA8 color_, XY scale_, int32_t value_, bool includeSignal_ = false);
		bool Update(float time_);
		void Draw();
		void Clear();
	};
}
