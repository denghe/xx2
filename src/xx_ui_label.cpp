#pragma once
#include "xx_ui_label.h"
#include "xx_gamebase.h"

namespace xx {

	Label& Label::Init(int32_t z_, XY position_, XY anchor_, float fontSize_, RGBA8 color_) {
		assert(typeId == cTypeId);
		z = z_;
		position = position_;
		anchor = anchor_;
		color = color_;
		fontSize = fontSize_;
		baseScale = fontSize / bmf->fontSize;
		return *this;
	}

	Label& Label::SetColor(RGBA8 color_) {
		color = color_;
		return *this;
	}

	Label& Label::SetFont(Shared<BMFont> bmf_) {
		bmf = std::move(bmf_);
		baseScale = fontSize / bmf->fontSize;
		return *this;
	}

	Label& Label::operator()(Shared<BMFont> bmf_) {
		return SetFont(std::move(bmf_));
	}

	void Label::Draw() {
		if (chars.Empty()) return;
		auto& q = GameBase::instance->Quad();
		RGBA8 c = { color.r, color.g, color.b, (uint8_t)(color.a * alpha) };
		float cp;
		if (enabled) cp = 1.f;
		else cp = 0.5f;
		auto s = worldScale * baseScale;
		for (auto& f : chars) {
			q.Draw(f.texId, f.uvRect, worldMinXY + f.offset * worldScale, 0, s, 0, cp, c);
		}
	}

}
