#include "pch.h"

namespace xx {

	void EffectText::Init(Rnd& rnd_, XY pos_, XY vec_, RGBA8 color_, XY scale_, int32_t value_, bool includeSignal_) {
		// calculate move frame inc with random speed
		auto _1_mag = 1.f / std::sqrtf(vec_.x * vec_.x + vec_.y * vec_.y);
		inc = vec_ * _1_mag * rnd_.Next<float>(cMoveSpeedMin, cMoveSpeedMax);
		data.color = color_;
		a = color_.a;
		alpha = 1;
		scale = scale_;

		auto len = data.FillTxt(value_, includeSignal_);
		pos_.x -= len * 0.5f * (GameBase::instance->embed.png_numbers.size.x / 12) * scale_.x;
		pos = pos_;
	}

	int32_t EffectText::Update(float time_) {
		XX_BEGIN(_1);
		// move away
		timeout = time_ + cMoveDurationSeconds;
		while(time_ <= timeout) {
			pos += inc;
			XX_YIELD_I(_1);
		}
		// fade out
		for (; alpha >= 0; alpha -= cFadeOutStep) {
			data.color.a = uint8_t(a * alpha);
			XX_YIELD_I(_1);
		}
		data.color.a = 0;
		XX_END(_1);
		return 1;
	}

	void EffectText::Draw(Shader_Numbers& shader_, Camera& cam_) {
		data.pos = cam_.ToGLPos(pos);
		data.scale = cam_.scale * scale;
		shader_.Draw(data);
	}

	void EffectTextManager::Init(Camera* cam_, int32_t cap_) {
		cam = cam_;
		ens.Reserve(cap_);
	}

	void EffectTextManager::Add(XY pos_, XY vec_, RGBA8 color_, XY scale_, int32_t value_, bool includeSignal_) {
		ens.Emplace().Init(rnd, pos_, vec_, color_, scale_, value_, includeSignal_);
	}

	bool EffectTextManager::Update(float time_) {
		int32_t n{};
		for (int32_t i = 0, e = ens.Count(); i < e; ++i) {
			if (ens[i].Update(time_)) {
				++n;
			}
		}
		ens.PopMulti(n);
		return ens.Empty();
	}

	void EffectTextManager::Draw() {
		auto& shader = GameBase::instance->Numbers();
		for (int32_t i = 0, e = ens.Count(); i < e; ++i) {
			ens[i].Draw(shader, *cam);
		}
	}

	void EffectTextManager::Clear() {
		ens.Clear();
	}

};
