#pragma once
#include "game.h"

namespace UI {

	struct EquipBagCell : xx::Button {
		static constexpr int32_t cTypeId{ 104 };
		EquipBagCell& Init(int32_t z_, XY position_, XY anchor_, XY size_) {
			assert(typeId == cTypeId);
			z = z_;
			position = position_;
			anchor = anchor_;
			size = size_;
			cfgNormal = gg.embed.cfg_s9bN;
			cfgHighlight = gg.embed.cfg_s9bH;
			FillTrans();
			auto& cfg = GetCfg();
			Make<xx::Scale9>()->Init(z + 1, 0, 0, size, cfg);
			return *this;
		}

	};

	struct EquipBag : xx::Node {
		static constexpr int32_t cTypeId{ 103 };
		void Init(int32_t z_);
	};

}
