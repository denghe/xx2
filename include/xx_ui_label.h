#pragma once
#include "xx_node.h"
#include "xx_bmfont.h"
#include "xx_gamebase.h"

namespace xx {

	struct LabelChar {
		GLuint texId;
		UVRect uvRect;
		XY offset;
	};

	struct Label : Node {
		static constexpr int32_t cTypeId{ 1 };

		List<LabelChar> chars;
		Shared<BMFont> bmf = GameBase::instance->embed.font_sys;
		float fontSize{ 32 };
		float baseScale{ fontSize / bmf->fontSize };
		RGBA8 color{};

		Label& Init(int32_t z_, XY position_, XY anchor_ = 0, float fontSize_ = 32, RGBA8 color_ = RGBA8_White);

		Label& SetColor(RGBA8 color_);

		Label& SetFont(Shared<BMFont> bmf_);

		// S : literal string u8/32string [view]
		template<typename S>
		Label& SetText(S const& txt_ = {}) {
			return SetText(StrPtr(txt_), StrLen(txt_));
		}

		// C: char / char8_t / char32_t
		template<typename C>
		Label& SetText(C const* txt_, size_t len_);

		// for easy use
		template<typename S>
		Label& operator()(S const& txt_) {
			return SetText(txt_);
		}

		Label& operator()(Shared<BMFont> bmf_);

		virtual void Draw() override;

		// for rich label
		struct CalcResult {
			float width;
			int32_t len;
			bool lineEnd;
		};

		// for rich label
		// calc & return pixel width & used len & is line end
		// C: char / char8_t / char32_t
		template<typename C>
		static CalcResult Calc(float fontSize_, float maxWidth_
			, Shared<BMFont> const& bmf_, C const* txt_, int32_t txtLen_);

		template<typename S>
		static CalcResult Calc(float fontSize_, float maxWidth_, S&& txt_
			, Shared<BMFont> const& bmf_ = GameBase::instance->embed.font_sys
		) {
			return Calc(fontSize_, maxWidth_, bmf_, StrPtr(txt_), StrLen(txt_));
		}
	};

}
