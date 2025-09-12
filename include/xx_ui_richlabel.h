#pragma once
#include "xx_node.h"

namespace xx {
	// todo
	struct RichLabel : Node {
		static constexpr int32_t cTypeId{ 19 };
		XY xy{};							// content cursor
		float lineHeight{};					// for last line
		int32_t lineItemsCount{};			// for last line
		HAligns halign{ HAligns::Left };	// when line end, horizontal align current line

		// step 1
		RichLabel& InitBegin(int32_t z_, XY const& position_, XY const& anchor_, float width_) {
			assert(width_ > 0);
			Node::InitDerived<RichLabel>(z_, position_, anchor_, 1, { width_, 0 });
			xy = {};
			lineHeight = 0;
			lineItemsCount = 0;
			children.Clear();
			return *this;
		}
		// step 2: fill content......

		// step 3
		RichLabel& InitEnd() {
			EndLine();
			size.y = -xy.y;
			for (auto& c : children) {
				c->position.y += size.y;
			}
			FillTransRecursive();
			return *this;
		}

		RichLabel& EndLine() {
			for (int32_t e = children.len, i = e - lineItemsCount; i < e; ++i) {
				auto& o = children[i];
				float leftHeight = lineHeight - o->size.y;
				assert(leftHeight >= 0);
				if (o->valign == VAligns::Top) {
					o->position.y = xy.y;
				}
				else if (o->valign == VAligns::Center) {
					o->position.y = xy.y - leftHeight * 0.5f;
				}
				else {	// if o->valign == VAligns::bottom
					o->position.y = xy.y - leftHeight;
				}
			}
			if (halign != HAligns::Left) {
				float left = size.x - xy.x;
				assert(left >= 0);
				float x;
				if (halign == HAligns::Right) x = left;
				else x = left * 0.5f;
				for (int32_t e = children.len, i = e - lineItemsCount; i < e; ++i) {
					auto& o = children[i];
					o->position.x += x;
				}
			}
			xy.x = 0;
			xy.y -= lineHeight;
			lineHeight = 0;
			lineItemsCount = 0;
			return *this;
		}

		RichLabel& EndLine(HAligns tmpHAlign_) {
			auto bak = halign;
			halign = tmpHAlign_;
			EndLine();
			halign = bak;
			return *this;
		}

		RichLabel& HAlign(HAligns halign_ = HAligns::Left) {
			halign = halign_;
			return *this;
		}

		RichLabel& LineSpace(float h_) {
			xy.y -= h_;
			return *this;
		}

		RichLabel& LineHeight(float h_) {
			lineHeight = h_;
			return *this;
		}

		template<typename S>
		RichLabel& Text(S const& txt_, float fontSize_
			, float lineHeight_ = 0
			, RGBA8 color_ = RGBA8_White
			, VAligns valign_ = VAligns::Center
			, Ref<BMFont> bmf_ = GameBase_shader::GetInstance()->embed.font_sys
		) {
			if (lineHeight_ == 0) lineHeight_ = fontSize_;
			auto txt = StrPtr(txt_);
			auto len = StrLen(txt_);
		LabLoop:
			if (lineHeight_ > lineHeight) {
				lineHeight = lineHeight_;
			}
			auto widthLimit = size.x - xy.x;
			auto r = Label::Calc(fontSize_, widthLimit, bmf_, txt, len);
			if (r.width > 0) {
				auto& L = Make<Label>()->Init(z, xy, { 0,1 }, fontSize_, color_).SetFont(bmf_).SetText(txt, r.len);
				assert(L.size.x == r.width);
				xy.x += r.width;
				++lineItemsCount;
			}
			if (r.lineEnd) {
				EndLine();
			}
			len -= r.len;
			txt += r.len;
			if (len > 0) goto LabLoop;
			return *this;
		}

		// ... more make xxx
	};
}
