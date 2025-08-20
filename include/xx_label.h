#pragma once
#include "xx_gamebase_font.h"
#include "xx_node.h"

namespace xx {

	struct Label : Node {
		List<FontFrame> fs;
		RGBA8 color;

		template<typename S>
		Label& Init(int z_, XY const& position_, XY const& scale_, XY const& anchor_, RGBA8 color_, S const& txt_) {
			Init(z_, position_, scale_, anchor_, color_);
			SetText(txt_);
			return *this;
		}

		Label& Init(int z_, XY const& position_, XY const& scale_, XY const& anchor_, RGBA8 color_) {
			z = z_;
			position = position_;
			anchor = anchor_;
			scale = scale_;
			color = color_;
			return *this;
		}

		void Resize(XY const& position_, XY const& scale_) {
			position = position_;
			scale = scale_;
			FillTrans();
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
			for (auto& f : fs) {
				q.Draw(f.texId, f.textureRect, basePos, 0, worldScale, 0, 1, color);
				basePos.x += f.textureRect.w * worldScale.x;
			}
		}
	};

}
