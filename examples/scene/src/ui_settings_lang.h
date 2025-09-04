#pragma once
#include "game.h"

namespace UI {

	struct Settings_Lang : xx::Node {
		static constexpr int32_t cTypeId{ 102 };
		i18n::Lang lang;
		void Init(int32_t z_);
		void MakeUI();
	};

}
