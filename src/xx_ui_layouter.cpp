#pragma once
#include "xx_ui_layouter.h"

namespace xx {

	// step 1
	Layouter& Layouter::InitBegin(Shared<Node> target_, int32_t z_, XY position_, XY anchor_, float width_) {
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
	Layouter& Layouter::InitEnd() {
		EndLine();
		target->size.y = -xy.y;
		for (auto& c : target->children) {
			c->position.y += target->size.y;
		}
		target->FillTransRecursive();
		return *this;
	}

	Layouter& Layouter::EndLine(bool changeY_, float newX_, float newLineHeight_, int32_t skip_) {
		for (int32_t e = target->children.len - skip_, i = e - lineItemsCount; i < e; ++i) {
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
			for (int32_t e = target->children.len - skip_, i = e - lineItemsCount; i < e; ++i) {
				auto& o = target->children[i];
				o->position.x += x;
			}
		}
		xy.x = newX_;
		if (changeY_) {
			xy.y -= lineHeight;
		}
		lineHeight = newLineHeight_;
		lineItemsCount = 0;
		return *this;
	}

	Layouter& Layouter::EndLine(HAligns tmpHAlign_, bool changeY, float newX_) {
		auto bak = halign;
		halign = tmpHAlign_;
		EndLine(changeY, newX_);
		halign = bak;
		return *this;
	}

	Layouter& Layouter::HAlign(HAligns halign_) {
		halign = halign_;
		return *this;
	}

	Layouter& Layouter::LineSpace(float h_) {
		xy.y -= h_;
		return *this;
	}

	Layouter& Layouter::LineHeight(float h_) {
		lineHeight = h_;
		return *this;
	}

	Layouter& Layouter::Offset(XY p_) {
		assert(lineItemsCount == 0);
		xy.x = p_.x;
		xy.y = -p_.y;
		return *this;
	}

	Layouter& Layouter::LeftMargin(float v_) {
		leftMargin = v_;
		return *this;
	}

	Layouter& Layouter::DefaultLineHeight(float v_) {
		defaultLineHeight = v_;
		return *this;
	}

	// append Image
	Layouter& Layouter::Image(TinyFrame frame_
		, XY fixedSize_
		, bool keepAspect_
		, ImageRadians radians_
		, RGBA8 color_
		, VAligns valign_) {
		auto& o = target->Make<::xx::Image>()->Init(target->z, 0, 0
			, std::move(frame_), fixedSize_, keepAspect_, radians_, color_);
		Append(o, valign_);
		return *this;
	}

}
