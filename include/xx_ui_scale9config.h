#pragma once
#include "xx_frame.h"

namespace xx {

	struct Scale9Config {
		TinyFrame frame;
		XY textureScale{ 2, 2 };
		UVRect center{ 2, 2, 2, 2 };
		RGBA8 color{ RGBA8_White };
		Paddings paddings{ 10, 20, 8, 20 };
	};

}
