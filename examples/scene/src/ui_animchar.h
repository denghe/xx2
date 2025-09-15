#pragma once
#include "game.h"

namespace UI {

	struct AnimChar : xx::Label {
		XY ori{};
		xx::Shaker shaker;
		int32_t _1{};

		AnimChar& Init(int32_t z_, XY position_, XY anchor_ = 0, float fontSize_ = 32, xx::RGBA8 color_ = xx::RGBA8_White) {
			xx::Label::Init(z_, position_, anchor_, fontSize_, color_);
			TryRegisterAutoUpdate();
			return *this;
		}

		int32_t Update() override {
			XX_BEGIN(_1);
			ori = position;
			shaker.Shake(3, 300 * gg.cDelta, gg.time + 9999999);
			while (true) {
				shaker.Update(gg.rnd, gg.time);
				position = ori + shaker.offset;
				FillTrans();
				XX_YIELD_I(_1);
			}
			XX_END(_1);
			return 1;
		};

	};

}
