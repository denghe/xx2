#pragma once
#include "xx_frame.h"

namespace xx {

	struct Scale9Config {
		TinyFrame frame;
		XY texScale{ 1, 1 };
		UVRect center{};
		RGBA8 color{ RGBA8{ 0x5f, 0x15, 0xd9, 0xff } };
		float borderScale{ 1.f };
		RGBA8 txtColor{ RGBA8_White };
		XY txtPadding{ 20, 5 };
		XY txtPaddingRightBottom{ 20, 10 };
		float txtScale{ 1 };
		float iconPadding{ 5 };

		XX_INLINE XY GetCornerSize() const {
			return { float(frame.textureRect.w - center.w) * borderScale, float(frame.textureRect.h - center.h) * borderScale };
		}

		XX_INLINE XY GetCornerScaledSize() const {
			return GetCornerSize() * texScale;
		}
	};

}
