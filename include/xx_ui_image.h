#pragma once
#include "xx_node.h"
#include "xx_frame.h"

namespace xx {

	enum class ImageRadians : int32_t {
		Zero = 0,		// 0'
		PiDiv2 = 1,		// 90'
		Pi = 2,			// 180'
		NegPiDiv2 = -1,	// -90'
		NegPi = -2		// -180'
	};

	struct Image : Node {
		static constexpr int32_t cTypeId{ 2 };

		TinyFrame frame;
		XY fixedScale{};
		RGBA8 color{};
		float radians{};

		Image& Init(int32_t z_, XY position_, XY anchor_
			, TinyFrame frame_
			, XY fixedSize_ = 0
			, bool keepAspect_ = true
			, ImageRadians radians_ = ImageRadians::Zero
			, RGBA8 color_ = RGBA8_White);

		void Draw() override;
	};

}
