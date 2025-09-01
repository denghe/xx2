#pragma once
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
		Ref<BMFont> bmf = GameBase_shader::GetInstance()->defaultFonts.sys;
		float fontSize{ 24 };
		float baseScale{ fontSize / bmf->fontSize };
		RGBA8 color{};

		Label& Init(int z_, XY position_, XY anchor_ = 0, XY scale_ = 1, RGBA8 color_ = RGBA8_White) {
			typeId = cTypeId;
			z = z_;
			position = position_;
			anchor = anchor_;
			scale = scale_;
			color = color_;
			return *this;
		}

		Label& SetFont(Ref<BMFont> bmf_, float fontSize_) {
			bmf = std::move(bmf_);
			fontSize = fontSize_;
			baseScale = fontSize / bmf->fontSize;
		}

		// S : literal string u8/32string [view]
		template<typename S>
		void SetText(S const& txt_, float lineWidthLimit = 0) {
			auto len = (int32_t)StrLen(txt_);
			if (!len) {
				chars.Clear();
				return;
			}
			chars.Resize(len);
			float px{}, py{}, maxpx{}, lineHeight = bmf->lineHeight * baseScale
				, fontDefaultWidth = bmf->fontSize * baseScale;
			for (int i = 0; i < len; ++i) {
				auto t = txt_[i];
				if (t == '\r') continue;
				else if (t == '\n') {
					px = 0;
					py -= lineHeight;
				}
				else if (auto r = bmf->Get(t); r) {
					auto cw = r->xadvance * baseScale;
					if (lineWidthLimit > 0) {
						if (px + cw > lineWidthLimit) {
							if (px > maxpx) {
								maxpx = px;
							}
							px = 0;
							py -= lineHeight;
						}
					}
					auto& c = chars[i];

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
		}

		void SetText() {
			chars.Clear();
		}

		virtual void Draw() override {
			if (chars.Empty()) return;
			auto& q = GameBase_shader::GetInstance()->Quad();
			RGBA8 c = { color.r, color.g, color.b, (uint8_t)(color.a * alpha) };
			auto s = worldScale * baseScale;
			for (auto& f : chars) {
				q.Draw(f.texId, f.textureRect, worldMinXY + f.offset * worldScale, 0, s, 0, 1, c);
			}
		}
	};

}
