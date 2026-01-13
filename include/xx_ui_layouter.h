#pragma once
#include "xx_node.h"
#include "xx_ui_image.h"
#include "xx_bmfont.h"

namespace xx {

	struct Layouter {
		Shared<Node> target;
		XY xy{};											// content cursor

		float width{};										// total

		float lineHeight{};									// last line / current
		int32_t lineItemsCount{};							//
		HAligns halign{ HAligns::Left };					//

		float defaultLineHeight{};							// defaults
		float leftMargin{};									//

		// step 1
		Layouter& InitBegin(Shared<Node> target_, int32_t z_, XY position_, XY anchor_, float width_);
		// step 2: append content......
		// step 3
		Layouter& InitEnd();

		Layouter& EndLine(bool changeY_ = true, float newX_ = 0, float newLineHeight_ = 0, int32_t skip_ = 0);
		Layouter& EndLine(HAligns tmpHAlign_, bool changeY = true, float newX_ = 0);
		Layouter& HAlign(HAligns halign_ = HAligns::Left);
		Layouter& LineSpace(float h_);
		Layouter& LineHeight(float h_ = 0.f);
		Layouter& Offset(XY p_);
		Layouter& LeftMargin(float v_);
		Layouter& DefaultLineHeight(float v_);

		// o: Make<????>().Init(L.z, L.xy, {0,1}, ..... )
		template<typename T> requires std::derived_from<T, Node>
		T& Append(T& o, VAligns valign_ = VAligns::Center) {
			assert(o.size.x <= width);
			auto lineHeight_ = std::max(defaultLineHeight, o.size.y);
			if (lineHeight_ > lineHeight) {
				lineHeight = lineHeight_;
			}
			if (width - xy.x < o.size.x + leftMargin) {
				EndLine(true, 0, lineHeight_, 1);
			}
			else {
				xy.x += leftMargin;
			}
			o.position = xy;
			o.anchor = { 0, 1 };
			xy.x += o.size.x;
			++lineItemsCount;
			return o;
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
			, Shared<BMFont> bmf_ = GameBase::instance->embed.font_sys
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
			, VAligns valign_ = VAligns::Center);

		// ...
	};

}
