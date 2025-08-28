#pragma once
#include "xx_node.h"

namespace xx {

	struct Label : Node {
		static constexpr int32_t cTypeId{ 1 };

		List<FontFrame> fs;
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

		// S : literal string u8/32string [view]
		template<typename S>
		void SetText(S const& txt_) {
			auto len = (int32_t)StrLen(txt_);
			if (!len) {
				fs.Clear();
				return;
			}
			fs.Resize(len);
			auto& fm = GameBase_font::GetInstance()->fontMaker;
			size = { 0, (float)fm.canvasSize.y };
			for (int i = 0; i < len; ++i) {
				fs[i] = fm[txt_[i]];
				size.x += fs[i].textureRect.w;
			}
			FillTrans();
		}

		void SetText() {
			fs.Clear();
		}

		virtual void Draw() override {
			if (fs.Empty()) return;
			auto& q = GameBase_shader::GetInstance()->Quad();
			auto basePos = worldMinXY;
			RGBA8 c = { color.r, color.g, color.b, (uint8_t)(color.a * alpha) };
			for (auto& f : fs) {
				q.Draw(f.texId, f.textureRect, basePos, 0, worldScale, 0, 1, c);
				basePos.x += f.textureRect.w * worldScale.x;
			}
		}
	};

}
