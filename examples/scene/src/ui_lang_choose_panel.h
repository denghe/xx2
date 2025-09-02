#pragma once
#include "game.h"

struct UILangChoosePanel : xx::Node {
	Lang lang;
	std::function<void(Languages v)> onClose;
	UILangChoosePanel& Init(int32_t z_);
	void MakeUI();
};
