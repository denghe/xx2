#pragma once
#include "game.h"

namespace UI {

	struct PropsItem : xx::Node {
		static constexpr int32_t cTypeId{ 107 };
	};

	struct LogItem : xx::Node {
		static constexpr int32_t cTypeId{ 106 };
	};

	struct PropsLog : xx::Node {
		static constexpr int32_t cTypeId{ 105 };
		void Init(int32_t z_);
	};

}
