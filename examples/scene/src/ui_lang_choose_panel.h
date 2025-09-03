#pragma once
#include "game.h"

struct UILangChoosePanel : xx::Node {
	static constexpr int32_t cTypeId{101};
	Lang lang;
	std::function<void(Languages v)> onClose;
	UILangChoosePanel& Init(int32_t z_);
	void MakeUI();
};
