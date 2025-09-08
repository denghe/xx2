#pragma once
#include "game.h"

namespace UI {

	struct EquipBagCell : xx::Node {
		static constexpr int32_t cTypeId{ 104 };
	};

	struct EquipBag : xx::Node {
		static constexpr int32_t cTypeId{ 103 };
		void Init(int32_t z_);
	};

}
