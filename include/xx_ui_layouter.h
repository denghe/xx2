#pragma once
#include "xx_node.h"

namespace xx {

	struct Layouter {
		Shared<Node> target;
		XY xy{};							// content cursor
		float width{};
		float lineHeight{};					// for last line
		int32_t lineItemsCount{};			// for last line
		HAligns halign{ HAligns::Left };	// for last line

		// step 1
		Layouter& InitBegin(Shared<Node> target_, int32_t z_, XY position_, XY anchor_, float width_) {
			assert(width_ > 0);
			assert(target_ && target_->children.Empty());
			target = std::move(target_);
			target->Init(z_, position_, anchor_, 1, { width_, 0 });
			xy = {};
			width = width_;
			lineHeight = 0;
			lineItemsCount = 0;
			return *this;
		}

		// step 2: append content......

		// step 3
		Layouter& InitEnd() {
			EndLine();
			target->size.y = -xy.y;
			for (auto& c : target->children) {
				c->position.y += target->size.y;
			}
			target->FillTransRecursive();
			return *this;
		}

		Layouter& EndLine(bool changeY_ = true, float newX_ = 0) {
			for (int32_t e = target->children.len, i = e - lineItemsCount; i < e; ++i) {
				auto& o = target->children[i];
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
				float left = width - xy.x;
				assert(left >= 0);
				float x;
				if (halign == HAligns::Right) x = left;
				else x = left * 0.5f;
				for (int32_t e = target->children.len, i = e - lineItemsCount; i < e; ++i) {
					auto& o = target->children[i];
					o->position.x += x;
				}
			}
			xy.x = newX_;
			if (changeY_) {
				xy.y -= lineHeight;
			}
			lineHeight = 0;
			lineItemsCount = 0;
			return *this;
		}

		Layouter& EndLine(HAligns tmpHAlign_, bool changeY = true, float newX_ = 0) {
			auto bak = halign;
			halign = tmpHAlign_;
			EndLine(changeY, newX_);
			halign = bak;
			return *this;
		}

		Layouter& HAlign(HAligns halign_ = HAligns::Left) {
			halign = halign_;
			return *this;
		}

		Layouter& LineSpace(float h_) {
			xy.y -= h_;
			return *this;
		}

		Layouter& LineHeight(float h_) {
			lineHeight = h_;
			return *this;
		}

		Layouter& Offset(XY p_) {
			assert(lineItemsCount == 0);
			xy.x = p_.x;
			xy.y = -p_.y;
			return *this;
		}

		// o: Make<????>().Init(L.z, L.xy, {0,1}, ..... )
		template<typename T>
		Layouter& Append(T& o, float lineHeight_ = 0, VAligns valign_ = VAligns::Center) {
			assert(o.size.x <= width);
			if (lineHeight_ == 0) lineHeight_ = o.size.y;
			if (lineHeight_ > lineHeight) {
				lineHeight = lineHeight_;
			}
			if (width - xy.x < o.size.x) {
				EndLine();
			}
			xy.x += o.size.x;
			++lineItemsCount;
			return *this;
		}

		// append Labels ( text word wrap )
		// char mode: children.len == begin index
		template<typename T = Label, bool charMode = false, typename S>
		Layouter& Text(S const& txt_
			, float fontSize_
			, float lineHeight_ = 0
			, RGBA8 color_ = RGBA8_White
			, VAligns valign_ = VAligns::Center
			, float newX_ = 0
			, Ref<BMFont> bmf_ = GameBase_shader::GetInstance()->embed.font_sys
		) {
			if (lineHeight_ == 0) lineHeight_ = fontSize_;
			auto txt = StrPtr(txt_);
			auto len = StrLen(txt_);
			if (!len) return *this;
		LabLoop:
			if (lineHeight_ > lineHeight) {
				lineHeight = lineHeight_;
			}
			auto widthLimit = width - xy.x;
			auto r = T::Calc(fontSize_, widthLimit, bmf_, txt, charMode ? 1 : len);
			if (r.width > 0) {
				auto& L = target->Make<T>()->Init(target->z, xy, { 0, 1 }, fontSize_, color_)
					.SetFont(bmf_).SetText(txt, r.len);
				assert(L.size.x == r.width);
				xy.x += r.width;
				++lineItemsCount;
			}
			if (r.lineEnd) {
				EndLine(true, newX_);
			}
			len -= r.len;
			txt += r.len;
			if (len > 0) goto LabLoop;
			return *this;
		}

		// append Image
		Layouter& Image(TinyFrame frame_
			, XY fixedSize_ = 0
			, bool keepAspect_ = true
			, ImageRadians radians_ = ImageRadians::Zero
			, RGBA8 color_ = RGBA8_White
			, float lineHeight_ = 0, VAligns valign_ = VAligns::Center) {
			auto& o = target->Make<::xx::Image>()->Init(target->z, xy, { 0, 1 }
				, std::move(frame_), fixedSize_, keepAspect_, radians_, color_);
			return Append(o, lineHeight_, valign_);
		}

		// ...
	};

}
