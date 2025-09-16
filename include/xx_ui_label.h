﻿#pragma once
#include "xx_node.h"
#include "xx_bmfont.h"

namespace xx {

	struct LabelChar {
		GLuint texId;
		UVRect textureRect;
		XY offset;
	};

	struct Label : Node {
		static constexpr int32_t cTypeId{ 1 };

		List<LabelChar> chars;
		Ref<BMFont> bmf = GameBase_shader::GetInstance()->embed.font_sys;
		float fontSize{ 32 };
		float baseScale{ fontSize / bmf->fontSize };
		RGBA8 color{};

		Label& Init(int32_t z_, XY position_, XY anchor_ = 0, float fontSize_ = 32, RGBA8 color_ = RGBA8_White) {
			assert(typeId == cTypeId);
			z = z_;
			position = position_;
			anchor = anchor_;
			color = color_;
			fontSize = fontSize_;
			baseScale = fontSize / bmf->fontSize;
			return *this;
		}

		Label& SetFont(Ref<BMFont> bmf_) {
			bmf = std::move(bmf_);
			baseScale = fontSize / bmf->fontSize;
			return *this;
		}

		// S : literal string u8/32string [view]
		template<typename S>
		Label& SetText(S const& txt_ = {}) {
			return SetText(StrPtr(txt_), StrLen(txt_));
		}

		// S : literal string u8/32string [view]
		template<typename C>
		Label& SetText(C const* txt_, size_t len_) {
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
					c.textureRect.x = r->x;
					c.textureRect.y = r->y;
					c.textureRect.w = r->width;
					c.textureRect.h = r->height;
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

		// for easy use
		template<typename S>
		Label& operator()(S const& txt_) {
			return SetText(txt_);
		}
		Label& operator()(Ref<BMFont> bmf_) {
			return SetFont(std::move(bmf_));
		}

		virtual void Draw() override {
			if (chars.Empty()) return;
			auto& q = GameBase_shader::GetInstance()->Quad();
			RGBA8 c = { color.r, color.g, color.b, (uint8_t)(color.a * alpha) };
			float cp;
			if (enabled) cp = 1.f;
			else cp = 0.5f;
			auto s = worldScale * baseScale;
			for (auto& f : chars) {
				q.Draw(f.texId, f.textureRect, worldMinXY + f.offset * worldScale, 0, s, 0, cp, c);
			}
		}

		// for rich label
		struct CalcResult {
			float width;
			int32_t len;
			bool lineEnd;
		};

		// for rich label
		// calc & return pixel width & used len & is line end
		template<typename C>
		inline static CalcResult Calc(float fontSize_, float maxWidth_
			, Ref<BMFont> const& bmf_, C const* txt_, int32_t txtLen_) {
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

		template<typename S>
		inline static CalcResult Calc(float fontSize_, float maxWidth_, S&& txt_
			, Ref<BMFont> const& bmf_ = GameBase_shader::GetInstance()->embed.font_sys
		) {
			return Calc(fontSize_, maxWidth_, bmf_, StrPtr(txt_), StrLen(txt_));
		}
	};

}
