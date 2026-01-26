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

	void Label::Clear() {
		chars.Clear();
	}

	template<typename C>
	Label& Label::SetText(C const* txt_, size_t len_) {
		chars.Clear();
		if (!len_) {
			return *this;
		}
		float px{}, py{}, maxpx{}, lineHeight = bmf->lineHeight * baseScale
			, fontDefaultWidth = bmf->fontSize * baseScale;
		for (int32_t i = 0; i < len_; ++i) {
			auto t = txt_[i];
			if (t == '\r' || t == '\n') continue;
			if (auto r = bmf->Get(t); r) {
				auto cw = r->xadvance * baseScale;
				auto& c = chars.Emplace();
				c.offset.x = px + r->xoffset * baseScale;
				c.offset.y = py - r->yoffset * baseScale;
				c.uvRect.x = r->x;
				c.uvRect.y = r->y;
				c.uvRect.w = r->width;
				c.uvRect.h = r->height;
				c.texId = r->texId;

				px += cw;
			}
			else {
				px += fontDefaultWidth;
			}
		}
		size = { std::max(px, maxpx), -py + lineHeight };
		FillTrans();
		return *this;
	}

	template Label& Label::SetText<char>(char const*, size_t);
	template Label& Label::SetText<char8_t>(char8_t const*, size_t);
	template Label& Label::SetText<char32_t>(char32_t const*, size_t);


	template<typename C>
	Label::CalcResult Label::Calc(float fontSize_, float maxWidth_
		, Shared<BMFont> const& bmf_, C const* txt_, int32_t txtLen_) {
		assert(txtLen_ > 0);
		auto baseScale = fontSize_ / bmf_->fontSize;
		float px{};
		int32_t i{};
		for (; i < txtLen_; ++i) {
			auto t = txt_[i];
			if (t == '\r') continue;
			else if (t == '\n') {
				return { px, i + 1, true };
			}
			auto r = bmf_->Get(t);
			float cw;
			if (r) cw = r->xadvance * baseScale;
			else cw = fontSize_;
			if (maxWidth_ > 0 && px + cw > maxWidth_) {
				return { px, i, true };
			}
			px += cw;
		}
		return { px, i, false };
	}

	template Label::CalcResult Label::Calc<char>(float, float, Shared<BMFont> const&, char const*, int32_t);
	template Label::CalcResult Label::Calc<char8_t>(float, float, Shared<BMFont> const&, char8_t const*, int32_t);
	template Label::CalcResult Label::Calc<char32_t>(float, float, Shared<BMFont> const&, char32_t const*, int32_t);
}
